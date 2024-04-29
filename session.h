#pragma once

#include <boost/asio.hpp>
#include <optional>

class Session : public std::enable_shared_from_this<Session> {
   private:
    struct Record {
        uint64_t id;
        std::string name;
    };
    using Table = std::vector<Record>;

    struct SessionContext {
        std::string errorMessage;
        std::string answer;
    };

   public:
    using Tcp = boost::asio::ip::tcp;
    Session(boost::asio::io_service& ios);
    ~Session();

    Tcp::socket& getSocket();
    void startAsyncRead();
    void handleRead(std::shared_ptr<Session>& s, const boost::system::error_code& e, size_t bytes);

   private:
    bool cmd_(SessionContext& ctx, const std::string& cmd);

    bool insert_(SessionContext& ctx, const std::vector<std::string>& insertVec);
    bool truncate_(SessionContext& ctx, const std::string& tableName);
    bool intersection_(SessionContext& ctx);
    bool symmetricDifference_(SessionContext& ctx);

    bool select_(SessionContext& ctx, const std::string& tableName);
    bool selectAll_(SessionContext& ctx);
    bool delete_(SessionContext& ctx, const std::string& tableName, const std::string& idStr);

    std::optional<uint64_t> stringToUint64(SessionContext& ctx, const std::string& s);
    Session::Table* getTable(const std::string& tableName, SessionContext* ctx = nullptr);
    bool isIdExists(const Table& table, uint64_t id);
    std::string getName(const std::string& tableName, uint64_t id);

    Tcp::socket socket_;
    std::vector<char> data_;
    std::unordered_map<std::string, Table> database_;
};
