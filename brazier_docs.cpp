#include "BrazierDocs.h"

using namespace std;

int main()
{
    try {
        brazier::ConfigManager::initGlobal();

        brazier::Server server(
            brazier::global_config->get("server.host", "0.0.0.0"),
            brazier::global_config->get("server.port", 3501));

        brazier::Database db(
            brazier::global_config->get("database.host", "localhost"),
            brazier::global_config->get("database.port", "5432"),
            brazier::global_config->get("database.username", "postgres"),
            brazier::global_config->get("database.password", ""),
            brazier::global_config->get("database.database", "postgres"));

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
        brazier::Logger::log(e.what(), "ERROR");
    }
    catch (...) {
        brazier::Logger::log("UNKNOWN ERROR", "ERROR");
    }
}
