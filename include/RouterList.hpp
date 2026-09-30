#include <brazier/Core>
#include "App/Controllers/NodeController.hpp"


using Request = http::request<http::string_body>;
using Response = http::response<http::string_body>;
using Params = std::unordered_map<std::string, std::string>;

using namespace brazier;

void registerRoutes() {
    auto nodeController = std::make_shared<NodeController>();

    R(GET, "/nodes/titles", nodeController, getAllTitles);

}