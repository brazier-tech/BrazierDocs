#pragma once

#include <brazier/DB>
#include <string>
#include <map>

class User : public Model<User> {
public:

    User() = default;
    User(const std::shared_ptr<Database>& db) : Model<User>(db) {}

    static inline std::string table_name = "users";
    static inline std::vector<std::string> fillable = {

    };
    static inline std::vector<std::string> fields = {
        "id",
    };
    static inline std::string primary_key = "id";

    std::map<std::string, std::string> attributes;
}