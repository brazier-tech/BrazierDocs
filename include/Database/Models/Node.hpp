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

    std::map<std::string, std::string> attributes;

    nlohmann::json toTitlesJson() const {
        auto get = [this](const std::string& key) -> std::string {
            auto it = attributes.find(key);
            return it != attributes.end() ? it->second : std::string{};
            };

        std::string pid = get("parent_id");

        return {
            {"id",         get("id")},
            {"parent_id",  pid.empty() ? nlohmann::json(nullptr) : nlohmann::json(pid)},
            {"title",      get("title")},
            {"slug",       get("slug")},
            {"sort_order", get("sort_order")}
        };
    }

    static nlohmann::json getAllTitlesJson() {
        auto nodes = Node::query()
            .Select({ "id", "parent_id", "title", "slug", "sort_order" })
            .get();

        std::string out;
        out.reserve(nodes.size() * 256);
        out += '[';
        bool first = true;
        for (const auto& n : nodes) {
            if (!first) out += ',';
            first = false;
            out += n->toTitlesJson().dump();
        }
        out += ']';
        return out;
    }

};