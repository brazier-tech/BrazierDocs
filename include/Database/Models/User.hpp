#pragma once

#include <brazier/orm.hpp>
#include <string>
#include <map>

class User : public brazier::Model<User> {
public:

    User() = default;
    User(std::shared_ptr<brazier::Database> db) : Model<User>(db) {}

    static inline std::string table_name = "users";
    static inline std::vector<std::string> fillable = {
        "username", "email", "hashed_password", "salt", "is_admin"
    };
    static inline std::vector<std::string> fields = {
        "id", "username", "email", "hashed_password", "salt", "is_admin"
    };
    static inline std::string primary_key = "id";

    std::string generate_salt(size_t bytes = 16);
    std::string hash(const std::string& password, const std::string& salt);
    bool verify(const std::string& password, const std::string& salt, const std::string& stored_hash);

};