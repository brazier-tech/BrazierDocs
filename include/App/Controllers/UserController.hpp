#pragma once

#include <brazier/Core>
#include <brazier/orm.hpp>
#include <brazier/Http>
#include <nlohmann/json.hpp>
#include <brazier\App\Http\Helpers\Validator.hpp>
#include "../../Database/Models/User.hpp"

namespace beast = boost::beast;
namespace http = beast::http;

using json = nlohmann::json;

class UserController : public brazier::Controller {
public:
    using Request = http::request<http::string_body>;
    using Response = http::response<http::string_body>;
    
    UserController();

    static inline std::shared_ptr<brazier::Database> db_ptr = nullptr;

    boost::asio::awaitable<void> register_(const Request& req, Response& res, const Params& params);
    boost::asio::awaitable<void> delete_(const Request& req, Response& res, const Params& params);
};
