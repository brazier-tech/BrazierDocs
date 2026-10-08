#include "Database/Models/Node.hpp"

nlohmann::json Node::toTitlesJson() const {
    auto get = [this](const std::string& key) -> std::string {
        try { return getAttribute(key); }
        catch (...) { return std::string{}; }
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

std::string Node::getAllTitlesJson() {
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

bool Node::deleteTree(const std::string& rootId) {
    auto db = brazier::orm::active_db();
    PGconn* conn = db->getConnection();
    if (!conn) {
        brazier::Logger::log("deleteTree: no db connection", "ERROR");
        return false;
    }

    std::string sql = "...";

    try {
        db->execute(sql);
        return true;
    }
    catch (const std::exception& e) {
        brazier::Logger::log("deleteTree error: " + std::string(e.what()), "ERROR");
        return false;
    }
}

void Node::delete_() {
    try {
        auto id = getAttribute("id");
        if (id.empty()) {
            brazier::Logger::log("delete_: ID attribute is missing", "ERROR");
            return;
        }
        deleteTree(id);
    }
    catch (const std::exception& e) {
        brazier::Logger::log("delete_ error: " + std::string(e.what()), "ERROR");
    }
}