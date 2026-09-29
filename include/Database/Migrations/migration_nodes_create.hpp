#pragma once

#include <brazier/DB>

using namespace brazier;

class MigrationNodesCreate : public BaseMigration<MigrationNodesCreate> {
public:
    static std::vector<std::string> up() {
        SQLSchemaBuilder builder("nodes");
        std::vector<std::string> queries;

        queries.push_back(builder
            .AddColumn("id UUID PRIMARY KEY DEFAULT gen_random_uuid()")
            .AddColumn("parent_id UUID NULL")
            .AddColumn("title VARCHAR(255) NOT NULL")
            .AddColumn("slug VARCHAR(255) NOT NULL")
            .AddColumn("content TEXT")
            .AddColumn("sort_order REAL NOT NULL DEFAULT 0")
            .AddColumn("created_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP")
            .AddColumn("updated_at TIMESTAMPTZ DEFAULT CURRENT_TIMESTAMP")
            .CreateTable());

        queries.push_back("ALTER TABLE nodes ADD CONSTRAINT fk_nodes_parent "
            "FOREIGN KEY (parent_id) REFERENCES nodes(id) ON DELETE CASCADE;");

        queries.push_back(builder.AddIndex("idx_nodes_parent_id", { "parent_id" }));
        queries.push_back(builder.AddIndex("idx_nodes_slug", { "slug" }));
        queries.push_back(builder.AddIndex("idx_nodes_sort", { "parent_id", "sort_order" }));

        return queries;
    }

    static std::string down() {
        SQLSchemaBuilder builder("nodes");
        return builder.DropTable();
    }
};