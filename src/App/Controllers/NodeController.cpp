#include "App/Controllers/NodeController.hpp"

boost::asio::awaitable<void> NodeController::create(const Request& req, Response& res, const Params&) {
    try {
        json body = json::parse(req.body());

        std::string title = body.value("title", std::string{});
        std::string slug = body.value("slug", std::string{});
        std::string content = body.value("content", std::string{});
        std::string parent_id = body.value("parent_id", std::string{});
        std::string sort_order = std::to_string(body.value("sort_order", 0.0));

        if (title.empty() || slug.empty()) {
            res.result(http::status::unprocessable_entity);
            res.body() = json({ {"error", "title and slug are required"} }).dump();
            co_return;
        }

        std::map<std::string, std::string> data;
        data["title"] = title;
        data["slug"] = slug;
        data["sort_order"] = sort_order;
        if (!content.empty())   data["content"] = content;
        if (!parent_id.empty()) data["parent_id"] = parent_id;

        if (!Node::create(data)->save()) throw std::runtime_error("Node creating error");

        res.result(http::status::created);
        res.body() = json({ {"status", "created"} }).dump();
        co_return;
    }
    catch (const json::parse_error& e) {
        res.result(http::status::bad_request);
        res.body() = json({ {"error", "invalid json"} }).dump();
        co_return;
    }
    catch (const std::exception& e) {
        brazier::Logger::log("NodeController::create error: " + std::string(e.what()), "ERROR");
        res.result(http::status::internal_server_error);
        res.body() = json({ {"error", e.what()} }).dump();
        co_return;
    }
}

boost::asio::awaitable<void> NodeController::getAllTitles(const Request& req, Response& res, const Params&) {
    try {
        res.body() = Node::getAllTitlesJson();
        res.result(http::status::ok);
        co_return;
    }
    catch (const std::exception& e) {
        res.body() = json({ {"error", e.what()} }).dump();
        res.result(http::status::internal_server_error);
        co_return;
    }
}

boost::asio::awaitable<void> NodeController::show(const Request& req, Response& res, const Params& params) {
    try {
        std::string id;
        if (params.empty()) {
            json body = json::parse(req.body());
            id = body.value("id", std::string{});
        }
        else {
            id = params.at("id");
        }

        if (id.empty()) {
            res.result(http::status::bad_request);
            res.body() = json({ {"error", "id is required"} }).dump();
            co_return;
        }

        auto node = Node::where("id = '" + id + "'").first();
        if (!node) {
            res.result(http::status::not_found);
            res.body() = json({ {"error", "not found"} }).dump();
            co_return;
        }

        res.result(http::status::ok);
        res.body() = node->toJson().dump();
        co_return;
    }
    catch (const std::exception& e) {
        res.result(http::status::internal_server_error);
        res.body() = json({ {"error", e.what()} }).dump();
        co_return;
    }
}

boost::asio::awaitable<void> NodeController::update(const Request& req, Response& res, const Params& params) {
    try {
        std::string id;
        if (params.empty()) {
            json body = json::parse(req.body());
            id = body.value("id", std::string{});
        }
        else {
            id = params.at("id");
        }

        if (id.empty()) {
            res.result(http::status::bad_request);
            res.body() = json({ {"error", "id is required"} }).dump();
            co_return;
        }

        auto node = Node::where("id = '" + id + "'").first();
        if (!node) {
            res.result(http::status::not_found);
            res.body() = json({ {"error", "not found"} }).dump();
            co_return;
        }

        json body = json::parse(req.body());

        if (!validateUpdate(body, id, res)) co_return;

        bool changed = false;

        if (body.contains("title")) {
            node->setAttribute("title", body["title"]);
            changed = true;
        }

        if (body.contains("slug")) {
            node->setAttribute("slug", body["slug"]);
            changed = true;
        }

        if (body.contains("content")) {
            node->setAttribute("content", body["content"]);
            changed = true;
        }

        if (body.contains("sort_order")) {
            node->setAttribute("sort_order", std::to_string(body["sort_order"].get<double>()));
            changed = true;
        }

        if (body.contains("parent_id") && !body["parent_id"].is_null()) {
            std::string pid = body["parent_id"].get<std::string>();
            if (!pid.empty()) {
                node->setAttribute("parent_id", pid);
                changed = true;
            }
        }

        if (!changed) {
            res.result(http::status::bad_request);
            res.body() = json({ {"error", "no fields to update"} }).dump();
            co_return;
        }

        if (!node->save()) throw std::runtime_error("Node updating error");

        res.result(http::status::ok);
        res.body() = node->toJson().dump();
        co_return;
    }
    catch (const json::parse_error& e) {
        res.result(http::status::bad_request);
        res.body() = json({ {"error", "invalid json"} }).dump();
        co_return;
    }
    catch (const std::exception& e) {
        brazier::Logger::log("NodeController::update error: " + std::string(e.what()), "ERROR");
        res.result(http::status::internal_server_error);
        res.body() = json({ {"error", e.what()} }).dump();
        co_return;
    }
}

boost::asio::awaitable<void> NodeController::delete_(const Request& req, Response& res, const Params& params) {
    try {
        std::string id;
        if (params.empty()) {
            json body = json::parse(req.body());
            id = body.value("id", std::string{});
        }
        else {
            id = params.at("id");
        }

        if (id.empty()) {
            res.result(http::status::bad_request);
            res.body() = json({ {"error", "id is required"} }).dump();
            co_return;
        }

        auto node = Node::where("id = '" + id + "'").first();
        if (!node) {
            res.result(http::status::not_found);
            res.body() = json({ {"error", "not found"} }).dump();
            co_return;
        }

        if (!Node::deleteTree(id)) {
            res.result(http::status::internal_server_error);
            res.body() = json({ {"error", "failed to delete tree"} }).dump();
            co_return;
        }

        res.result(http::status::no_content);
        co_return;
    }
    catch (const json::parse_error& e) {
        res.result(http::status::bad_request);
        res.body() = json({ {"error", "invalid json"} }).dump();
        co_return;
    }
    catch (const std::exception& e) {
        brazier::Logger::log("NodeController::delete_ error: " + std::string(e.what()), "ERROR");
        res.result(http::status::internal_server_error);
        res.body() = json({ {"error", e.what()} }).dump();
        co_return;
    }
}

bool NodeController::validateUpdate(const json& body,
    const std::string& id,
    Response& res) {
    if (body.contains("title")) {
        if (!body["title"].is_string() || body["title"].get<std::string>().empty()) {
            res.result(http::status::unprocessable_entity);
            res.body() = json({ {"error", "invalid title"} }).dump();
            return false;
        }
    }

    if (body.contains("slug")) {
        if (!body["slug"].is_string() || !is_valid_slug(body["slug"])) {
            res.result(http::status::unprocessable_entity);
            res.body() = json({ {"error", "invalid slug"} }).dump();
            return false;
        }
        std::string newSlug = body["slug"];
        auto clash = Node::where("slug = '" + newSlug + "' AND id <> '" + id + "'").first();
        if (clash) {
            res.result(http::status::conflict);
            res.body() = json({ {"error", "slug already exists"} }).dump();
            return false;
        }
    }

    if (body.contains("content")) {
        if (!body["content"].is_string()) {
            res.result(http::status::unprocessable_entity);
            res.body() = json({ {"error", "content must be string"} }).dump();
            return false;
        }
    }

    if (body.contains("sort_order")) {
        if (!body["sort_order"].is_number()) {
            res.result(http::status::unprocessable_entity);
            res.body() = json({ {"error", "sort_order must be number"} }).dump();
            return false;
        }
    }

    if (body.contains("parent_id") && !body["parent_id"].is_null()) {
        std::string pid = body["parent_id"].get<std::string>();
        if (!pid.empty()) {
            if (pid == id) {
                res.result(http::status::unprocessable_entity);
                res.body() = json({ {"error", "node cannot be its own parent"} }).dump();
                return false;
            }
            auto parent = Node::where("id = '" + pid + "'").first();
            if (!parent) {
                res.result(http::status::not_found);
                res.body() = json({ {"error", "parent not found"} }).dump();
                return false;
            }
        }
    }

    return true;
}

bool NodeController::is_valid_slug(const std::string& s) {
    static const std::regex re("^[a-z0-9]+(-[a-z0-9]+)*$");
    return !s.empty() && s.size() <= 255 && std::regex_match(s, re);
}