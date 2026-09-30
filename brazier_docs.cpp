#include "BrazierDocs.h"

using namespace std;

int main()
{
	try {
        registerRoutes();

        brazier::ConfigManager::initGlobal();

        brazier::Server server(
            brazier::global_config->get("server.host", "0.0.0.0"),
            brazier::global_config->get("server.port", 3501));

        brazier::Database db;
        auto manager = std::make_shared<brazier::MigrationManager>(db);
        manager->migrateAll<MigrationNodesCreate>();

        if (!server.initialize()) {
            return 1;
        }
        server.run();
        return 0;
	}
    catch (std::exception e) {
        brazier::Logger::log(e.what(), "ERROR");
    }
    catch (...) {
        brazier::Logger::log("UNKNOWN ERROR", "ERROR");
    }
}
