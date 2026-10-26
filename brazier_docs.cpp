#include "BrazierDocs.h"

using namespace brazier;

int main()
{
    try {
        ConfigManager::initGlobal();

        Server server(
            ConfigManager::getGlobal().get("server.host", "0.0.0.0"),
            ConfigManager::getGlobal().get("server.port", 3501));

        Database db(
            ConfigManager::getGlobal().get("database.host", "localhost"),
            ConfigManager::getGlobal().get("database.port", "5432"),
            ConfigManager::getGlobal().get("database.username", "postgres"),
            ConfigManager::getGlobal().get("database.password", ""),
            ConfigManager::getGlobal().get("database.database", "postgres"));

        std::unique_ptr<MigrationManager> manager = nullptr;

        manager = std::make_unique<MigrationManager>(db);
        
        manager->migrateAll<
            MigrationNodesCreate>();

        registerRoutes();

        if (!server.initialize()) return 1;
        server.run();
        return 0;
    }
    catch (const std::exception& e) {
        Logger::log(e.what(), "ERROR");
    }
    catch (...) {
        brazier::Logger::log("UNKNOWN ERROR", "ERROR");
    }
}
