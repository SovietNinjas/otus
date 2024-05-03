#include "database.h"

#include "utils.h"

bool Database::cmd_(Context& ctx, const std::string& cmd) {
    auto v = utils::splitString(cmd, ' ');
    size_t vSize = v.size();

    auto ok = vSize != 0;

    if (ok) {
        std::string commandType = v.front();
        utils::stringToLower(commandType);

        if (commandType == "select" && vSize == 2) {
            ok = select(ctx, v[1]);
        } else if (commandType == "selectall") {
            ok = selectAll(ctx);
        } else if (commandType == "insert" && vSize == 4) {
            ok = insert(ctx, v);
        } else if (commandType == "delete" && vSize == 3) {
            ok = delete_(ctx, v[1], v[2]);
        } else if (commandType == "truncate" && vSize == 2) {
            ok = truncate(ctx, v[1]);
        } else if (commandType == "intersection" && vSize == 1) {
            ok = intersection(ctx);
        } else if (commandType == "symmetric_difference" && vSize == 1) {
            ok = symmetricDifference(ctx);
        } else {
            ctx.errorMessage = "Invalid command: " + commandType;
            ok = false;
        }
    } else {
        ctx.errorMessage = "Invalid format!";
    }

    return ok;
}

bool Database::insert(Context& ctx, const std::vector<std::string>& insertVec) {
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

bool Database::truncate(Context& ctx, const std::string& tableName) {
    Table* tbl = getTable_(tableName, &ctx);
    if (!tbl) {
        return false;
    }

    database_.erase(tableName);
    return true;
}

bool Database::intersection(Context& ctx) {
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
            if (isIdExists_(it->second, record.id)) {
                mTemp[record.id].emplace_back(it->first);
            }
        }
    }

    for (auto it = mTemp.begin(); it != mTemp.end(); ++it) {
        if (it->second.size() != tableCount) {
            continue;
        }

        ctx.answer += std::to_string(it->first) + "," + getName_(database_.begin()->first, it->first) + ",";

        for (const std::string& tableName : it->second) {
            ctx.answer += getName_(tableName, it->first) + ",";
        }

        utils::stringRmRight(ctx.answer, 1);
        ctx.answer += "\n";
    }

    return true;
}

bool Database::symmetricDifference(Context& ctx) {
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
        if (!isIdExists_(tableB, record.id)) {
            lPrint(record.id, record.name, std::string());
        }
    }

    for (const Record& record : tableB) {
        if (!isIdExists_(tableA, record.id)) {
            lPrint(record.id, std::string(), record.name);
        }
    }

    return true;
}

bool Database::select(Context& ctx, const std::string& tableName) {
    Table* tbl = getTable_(tableName, &ctx);
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

bool Database::selectAll(Context& ctx) {
    for (const auto& table : database_) {
        if (!select(ctx, table.first)) {
            return false;
        }
        ctx.answer += "\n";
    }
    return true;
}

bool Database::delete_(Context& ctx, const std::string& tableName, const std::string& idStr) {
    Table* tbl = getTable_(tableName, &ctx);
    if (!tbl) {
        return false;
    }

    auto id = stringToUint64_(ctx, idStr);
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

std::optional<uint64_t> Database::stringToUint64_(Context& ctx, const std::string& s) {
    uint64_t id = 0;
    if (auto a = utils::stringToUint64(s); a) {
        id = a.value();
    } else {
        ctx.errorMessage = "Invalid ID";
        return std::nullopt;
    }

    return id;
}

Database::Table* Database::getTable_(const std::string& tableName, Context* ctx) {
    auto it = database_.find(tableName);
    if (it == database_.end()) {
        if (ctx) {
            ctx->errorMessage = "Table \"" + tableName + "\" not found";
        }
        return nullptr;
    }
    return &it->second;
}

bool Database::isIdExists_(const Table& table, uint64_t id) {
    for (const auto& record : table) {
        if (record.id == id) {
            return true;
        }
    }
    return false;
}

std::string Database::getName_(const std::string& tableName, uint64_t id) {
    Table* table = getTable_(tableName);
    for (const auto& record : (*table)) {
        if (record.id == id) {
            return record.name;
        }
    }
    return std::string();
}
