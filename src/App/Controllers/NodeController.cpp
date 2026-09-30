#pragma once

#include "../../../include/App/Controllers/NodeController.hpp"

boost::asio::awaitable<void> NodeController::create(const Request& req, Response& res, const Params& params) {
	json body = json::parse(req.body());

}

boost::asio::awaitable<void> NodeController::getAllTitles(const Request& req, Response& res, const Params&) {
    try {
        auto nodes;

        json out = json::array();
        for (const auto& n : rows) {
            json item = {
                {"id",        n.attributes.count("id") ? n.attributes.at("id") : ""},
                {"parent_id", n.attributes.count("parent_id") ? n.attributes.at("parent_id") : nullptr},
                {"title",     n.attributes.count("title") ? n.attributes.at("title") : ""},
                {"slug",      n.attributes.count("slug") ? n.attributes.at("slug") : ""},
                {"sort_order",n.attributes.count("sort_order") ? n.attributes.at("sort_order") : "0"}
            };
            if (item["parent_id"] == "") item["parent_id"] = nullptr;
            out.push_back(std::move(item));
        }

        res.status(200).json(out);
    }
    catch (const std::exception& e) {
        res.status(500).json({ {"error", e.what()} });
    }
}

boost::asio::awaitable<void> NodeController::index(const Request& req, Response& res, const Params& params) {
	json body = json::parse(req.body());
}

boost::asio::awaitable<void> NodeController::update(const Request& req, Response& res, const Params& params) {
	json body = json::parse(req.body());
}

boost::asio::awaitable<void> NodeController::delete_(const Request& req, Response& res, const Params& params) {
	json body = json::parse(req.body());
}

