#include "session.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "utils.h"

using namespace boost::asio;

static constexpr size_t BUFFER_SIZE = 1024;

Session::Session(io_service& ios) : socket_(ios) { data_.resize(BUFFER_SIZE); }

Session::~Session() {}

ip::tcp::socket& Session::getSocket() { return socket_; }

void Session::startAsyncRead() {
    socket_.async_read_some(buffer(data_, BUFFER_SIZE),
                             boost::bind(&Session::handleRead, this, shared_from_this(), placeholders::error,
                                         placeholders::bytes_transferred));
}

void Session::handleRead(std::shared_ptr<Session>& s, const boost::system::error_code& e, size_t bytes) {
    std::string clientAddress = utils::getSocketAddress(s);

    if (e) {
        if (e == error::connection_reset || e == error::eof) {
            std::cout << "Disconnected " << clientAddress << std::endl;
        } else {
            std::cout << "Can't read data from client " << clientAddress << ": " << e.message() << std::endl;
        }
        return;
    }

    auto commands = utils::splitString(std::string(data_.begin(), data_.begin() + bytes), '\n');
    for (const auto& command : commands) {
        SessionContext ctx;

        bool res = cmd_(ctx, command);

        std::string answer;
        if (res) {
            answer = "OK";

            if (!ctx.answer.empty()) {
                answer += "\n" + ctx.answer;
            }
        } else {
            answer = ctx.errorMessage;
        }
        answer += "\n";

        s->getSocket().write_some(buffer(answer, answer.size()));
    }

    std::fill(data_.begin(), data_.end(), '\0');
    startAsyncRead();
}

bool Session::cmd_(SessionContext& ctx, const std::string& cmd) {
    auto v = utils::splitString(cmd, ' ');
    size_t vSize = v.size();
    if (vSize == 0) {
        ctx.errorMessage = "Invalid format!";
        return false;
    }

    std::string commandType = v.front();
    utils::stringToLower(commandType);

    if (commandType == "select" && vSize == 2) {
        return select_(ctx, v[1]);
    } else if (commandType == "selectall") {
        return selectAll_(ctx);
    } else if (commandType == "insert" && vSize == 4) {
        return insert_(ctx, v);
    } else if (commandType == "delete" && vSize == 3) {
        return delete_(ctx, v[1], v[2]);
    } else if (commandType == "truncate" && vSize == 2) {
        return truncate_(ctx, v[1]);
    } else if (commandType == "intersection" && vSize == 1) {
        return intersection_(ctx);
    } else if (commandType == "symmetric_difference" && vSize == 1) {
        return symmetricDifference_(ctx);
    } else {
        ctx.errorMessage = "Invalid command: " + commandType;
    }

    return false;
}

bool Session::insert_(SessionContext& ctx, const std::vector<std::string>& insertVec) {
    std::string tableName = insertVec[1];

    uint64_t id = 0;
    if (auto a = utils::stringToUint64(insertVec[2]); a) {
        id = a.value();
    } else {
        ctx.errorMessage = "Invalid ID!";
        return false;
    }

    std::string name = insertVec[3];

    auto it = database_.find(tableName);
    if (it != database_.end()) {
        bool found = false;
        for (const Record& record : it->second) {
            found = record.id == id;
            if (found) {
                ctx.errorMessage = "ERR duplicate " + insertVec[2];
                return false;
            }
        }

        it->second.emplace_back(Record{id, name});
    } else {
        database_[tableName].emplace_back(Record{id, name});
    }

    return true;
}

bool Session::truncate_(SessionContext& ctx, const std::string& tableName) {
    Table* tbl = getTable(tableName, &ctx);
    if (!tbl) {
        return false;
    }

    database_.erase(tableName);
    return true;
}

bool Session::intersection_(SessionContext& ctx) {
    size_t tableCount = database_.size();
    if (tableCount == 1) {
        ctx.errorMessage = "ERR for this operator you need to have more one table";
        return false;
    }
    --tableCount;

    const Table& firstTable = database_.begin()->second;

    std::unordered_map<uint64_t, std::vector<std::string>> mTemp;

    for (auto it = std::next(database_.begin()); it != database_.end(); ++it) {
        for (const auto& record : firstTable) {
            if (isIdExists(it->second, record.id)) {
                mTemp[record.id].emplace_back(it->first);
            }
        }
    }

    for (auto it = mTemp.begin(); it != mTemp.end(); ++it) {
        if (it->second.size() != tableCount) {
            continue;
        }

        ctx.answer += std::to_string(it->first) + "," + getName(database_.begin()->first, it->first) + ",";

        for (const std::string& tableName : it->second) {
            ctx.answer += getName(tableName, it->first) + ",";
        }

        utils::stringRmRight(ctx.answer, 1);
        ctx.answer += "\n";
    }

    return true;
}

bool Session::symmetricDifference_(SessionContext& ctx) {
    if (database_.size() != 2) {
        ctx.errorMessage = "Support symmetric difference for two tables only";
        return false;
    }

    Table& tableA = database_.begin()->second;
    Table& tableB = std::next(database_.begin())->second;

    auto lPrint = [&ctx](uint64_t id, const std::string& nameA, const std::string& nameB) {
        ctx.answer += std::to_string(id) + "\t" + nameA + "\t" + nameB + "\n";
    };

    ctx.answer = "ID\t" + database_.begin()->first + "\t" + std::next(database_.begin())->first + "\n";

    for (const Record& record : tableA) {
        if (!isIdExists(tableB, record.id)) {
            lPrint(record.id, record.name, std::string());
        }
    }

    for (const Record& record : tableB) {
        if (!isIdExists(tableA, record.id)) {
            lPrint(record.id, std::string(), record.name);
        }
    }

    return true;
}

bool Session::select_(SessionContext& ctx, const std::string& tableName) {
    Table* tbl = getTable(tableName, &ctx);
    if (!tbl) {
        return false;
    }

    ctx.answer += tableName + ":\n";

    if (tbl->empty()) {
        ctx.answer += "Table \"" + tableName + "\" is empty";
    } else {
        for (const auto& record : (*tbl)) {
            ctx.answer += std::to_string(record.id) + "\t" + record.name + "\n";
        }
    }

    return true;
}

bool Session::selectAll_(SessionContext& ctx) {
    for (const auto& table : database_) {
        if (!select_(ctx, table.first)) {
            return false;
        }
        ctx.answer += "\n";
    }
    return true;
}

bool Session::delete_(SessionContext& ctx, const std::string& tableName, const std::string& idStr) {
    Table* tbl = getTable(tableName, &ctx);
    if (!tbl) {
        return false;
    }

    auto id = stringToUint64(ctx, idStr);
    if (!id) {
        return false;
    }

    bool found = false;

    for (size_t i = 0, c = tbl->size(); i < c; ++i) {
        Record& record = (*tbl)[i];
        found = record.id == id;

        if (found) {
            auto itBeg = tbl->begin();
            std::advance(itBeg, i);
            tbl->erase(itBeg);
            break;
        }
    }

    if (!found) {
        ctx.errorMessage = "Record with id " + idStr + " not found";
        return false;
    }

    return true;
}

std::optional<uint64_t> Session::stringToUint64(SessionContext& ctx, const std::string& s) {
    uint64_t id = 0;
    if (auto a = utils::stringToUint64(s); a) {
        id = a.value();
    } else {
        ctx.errorMessage = "Invalid ID";
        return std::nullopt;
    }

    return id;
}

Session::Table* Session::getTable(const std::string& tableName, SessionContext* ctx) {
    auto it = database_.find(tableName);
    if (it == database_.end()) {
        if (ctx) {
            ctx->errorMessage = "Table \"" + tableName + "\" not found";
        }
        return nullptr;
    }
    return &it->second;
}

bool Session::isIdExists(const Table& table, uint64_t id) {
    for (const auto& record : table) {
        if (record.id == id) {
            return true;
        }
    }
    return false;
}

std::string Session::getName(const std::string& tableName, uint64_t id) {
    Table* table = getTable(tableName);
    for (const auto& record : (*table)) {
        if (record.id == id) {
            return record.name;
        }
    }
    return std::string();
}
