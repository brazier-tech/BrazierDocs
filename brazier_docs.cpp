#include "BrazierDocs.h"

using namespace brazier;

int main()
{
    try {
        ConfigManager::initGlobal();

        Server server(
            global_config->get("server.host", "0.0.0.0"),
            global_config->get("server.port", 3501));

        Database db(
            global_config->get("database.host", "localhost"),
            global_config->get("database.port", "5432"),
            global_config->get("database.username", "postgres"),
            global_config->get("database.password", ""),
            global_config->get("database.database", "postgres"));

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
