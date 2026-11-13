#include "App/Controllers/UserController.hpp"

UserController::UserController() {
    try {
        db_ptr = std::make_shared<brazier::Database>(
            brazier::ConfigManager::getGlobal().get("database.host", "localhost"),
            brazier::ConfigManager::getGlobal().get("database.port", "5432"),
            brazier::ConfigManager::getGlobal().get("database.username", "postgres"),
            brazier::ConfigManager::getGlobal().get("database.password", ""),
            brazier::ConfigManager::getGlobal().get("database.database", "postgres"));
    }
    catch (std::exception& e) {
        brazier::Logger::log(e.what(), "ERROR");
    }
}

boost::asio::awaitable<void> UserController::create(const Request& req, Response& res, const Params&) {
    try {
        json body = json::parse(req.body());

        std::string username = body.value("username", std::string{});
        std::string email = body.value("email", std::string{});
        std::string password = body.value("password", std::string{});
        bool is_admin = body.value("is_admin", false);

        if (username.empty() || email.empty() || password.empty()) {
            res.result(http::status::unprocessable_entity);
            res.body() = json({
                {"error", "username, email and password are required"}
            }).dump();
            co_return;
        }

        if (!brazier::Validator::email(email)) {
            res.result(http::status::unprocessable_entity);
            res.body() = json({ {"error", "invalid email"} }).dump();
            co_return;
        }

        if (!brazier::Validator::password(password)) {
            res.result(http::status::unprocessable_entity);
            res.body() = json({ {"error", "password validation"} }).dump();
            co_return;
        }

        auto [hashed_password, salt_bytes] = co_await brazier::Hash::awaitableHash(password, {});
        if (hashed_password.empty())
            throw std::runtime_error("Password hashing failed");

        std::string salt_hex = brazier::Hash::bytesToHexString(salt_bytes);

        std::map<std::string, std::string> data;
        data["username"] = username;
        data["email"] = email;
        data["hashed_password"] = hashed_password;
        data["salt"] = salt_hex;        
        data["is_admin"] = is_admin ? "true" : "false";

        if (!User::create(data, false, db_ptr)->save())
            throw std::runtime_error("User creating error");

        res.result(http::status::created);
        res.body() = json({ {"status", "created"} }).dump();
        co_return;
    }
    catch (const json::parse_error&) {
        res.result(http::status::bad_request);
        res.body() = json({ {"error", "invalid json"} }).dump();
        co_return;
    }
    catch (const std::exception& e) {
        brazier::Logger::log("UserController::create error: " + std::string(e.what()), "ERROR");
        res.result(http::status::internal_server_error);
        res.body() = json({ {"error", e.what()} }).dump();
        co_return;
    }
}
