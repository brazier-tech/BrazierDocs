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
        "username", "email", "hashed_password", "salt"
    };
    static inline std::vector<std::string> fields = {
        "id", "username", "email", "hashed_password", "salt"
    };
    static inline std::string primary_key = "id";


};