#pragma once

#include <brazier/DB>
#include <string>
#include <map>

class Node : public brazier::Model<Node> {
public:

    Node() = default;
    Node(const std::shared_ptr<brazier::Database>& db) : Model<Node>(db) {}

    static inline std::string table_name = "nodes";
    static inline std::vector<std::string> fillable = {
        "parent_id", "title", "slug", "content", "sort_order", "created_at", "updated_at"
    };
    static inline std::vector<std::string> fields = {
        "id", "parent_id", "title", "slug", "content", "sort_order", "created_at", "updated_at"
    };
    static inline std::string primary_key = "id";

    nlohmann::json toTitlesJson() const;

    static std::string getAllTitlesJson();
    
    static bool deleteTree(const std::string& rootId);
    void delete_();
};