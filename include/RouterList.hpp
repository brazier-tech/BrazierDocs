#include <brazier/Core>
#include "App/Controllers/NodeController.hpp"
#include "App/Controllers/UserController.hpp"

using Request = http::request<http::string_body>;
using Response = http::response<http::string_body>;
using Params = std::unordered_map<std::string, std::string>;

using namespace brazier;

void registerRoutes() {
    auto nodeController = std::make_shared<NodeController>();
    auto userController = std::make_shared<UserController>();

    R(GET, "/nodes/titles", nodeController, getAllTitles);
    R(POST, "/node/create", nodeController, create);
    R(GET, "/node/show", nodeController, show);
    R(GET, "/node/show/:id", nodeController, show);
    R(PUT, "/node/update", nodeController, update);
    R(DELETE_, "/node/delete", nodeController, delete_);
    R(DELETE_, "/node/delete/:id", nodeController, delete_);

    R(POST, "/user/create", userController, create);
}