#pragma once

#include "../../../include/App/Controllers/NodeController.hpp"

boost::asio::awaitable<void> NodeController::create(const Request& req, Response& res, const Params& params) {
	json body = json::parse(req.body());
    co_return;
}

boost::asio::awaitable<void> NodeController::getAllTitles(const Request& req, Response& res, const Params&) {
    try {
        res.body() = Node::getAllTitlesJson().dump();
        res.result(http::status::ok);
        co_return;
    }
    catch (const std::exception& e) {
        res.body() = json({ {"error", e.what()} }).dump();
        res.result(http::status::internal_server_error);
        co_return;
    }
}

boost::asio::awaitable<void> NodeController::index(const Request& req, Response& res, const Params& params) {
	json body = json::parse(req.body());
    co_return;
}

boost::asio::awaitable<void> NodeController::update(const Request& req, Response& res, const Params& params) {
	json body = json::parse(req.body());
    co_return;
}

boost::asio::awaitable<void> NodeController::delete_(const Request& req, Response& res, const Params& params) {
	json body = json::parse(req.body());
    co_return;
}

