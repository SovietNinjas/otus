#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class Database {
   public:
    struct Context {
        std::string errorMessage;
        std::string answer;
    };

    bool cmd_(Context& ctx, const std::string& cmd);

    bool insert(Context& ctx, const std::vector<std::string>& insertVec);
    bool truncate(Context& ctx, const std::string& tableName);
    bool intersection(Context& ctx);
    bool symmetricDifference(Context& ctx);

    bool select(Context& ctx, const std::string& tableName);
    bool selectAll(Context& ctx);
    bool delete_(Context& ctx, const std::string& tableName, const std::string& idStr);

   private:
    struct Record {
        uint64_t id;
        std::string name;
    };

    using Table = std::vector<Record>;

    std::optional<uint64_t> stringToUint64_(Context& ctx, const std::string& s);
    Table* getTable_(const std::string& tableName, Context* ctx = nullptr);
    bool isIdExists_(const Table& table, uint64_t id);
    std::string getName_(const std::string& tableName, uint64_t id);

    std::unordered_map<std::string, Table> database_;
};
