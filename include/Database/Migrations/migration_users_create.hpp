#pragma once

#include <brazier/orm.hpp>

using namespace brazier;

class MigrationUsersCreate : public BaseMigration<MigrationUsersCreate> {
public:
    static std::vector<std::string> up() {

        SQLSchemaBuilder builder("users");
        std::vector<std::string> queries;

        queries.push_back(builder
            .AddColumn("id SERIAL PRIMARY KEY")
            .AddColumn("username VARCHAR(255) NOT NULL")
            .AddColumn("email VARCHAR(255) NOT NULL")
            .AddColumn("hashed_password VARCHAR(255) NOT NULL")
            .AddColumn("salt VARCHAR(255) NOT NULL")
            .AddColumn("is_admin BOOLEAN NOT NULL")
            .CreateTable());

        return queries;
    }

    static std::vector<std::string> down() {
        SQLSchemaBuilder builder("users");
        return { builder.DropTable() };
    }
};