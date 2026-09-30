#include <brazier/Core>
#include "../App/Http/Controllers/ProfileController.hpp"


using Request = http::request<http::string_body>;
using Response = http::response<http::string_body>;
using Params = std::unordered_map<std::string, std::string>;

using namespace brazier;

void registerRoutes() {
    auto profileController = std::make_shared<ProfileController>();

    R(POST, "/register", userController, register_);

}