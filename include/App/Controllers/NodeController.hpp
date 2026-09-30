#pragma once

#include <brazier/Core>
#include <brazier/DB>
#include <brazier/Http>
#include <nlohmann/json.hpp>
#include "../../Database/Models/Node.hpp"

namespace beast = boost::beast;
namespace http = beast::http;

using json = nlohmann::json;

class NodeController : public brazier::Controller {
public:
    using Request = http::request<http::string_body>;
    using Response = http::response<http::string_body>;

    boost::asio::awaitable<void> create(const Request& req, Response& res, const Params& params);
    boost::asio::awaitable<void> getAllTitles(const Request& req, Response& res, const Params& params);
    boost::asio::awaitable<void> index(const Request& req, Response& res, const Params& params);
    boost::asio::awaitable<void> update(const Request& req, Response& res, const Params& params);
    boost::asio::awaitable<void> delete_(const Request& req, Response& res, const Params& params);

};
