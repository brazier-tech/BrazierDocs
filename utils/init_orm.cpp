#include <brazier/orm.hpp>
#include <brazier/Core>

using namespace brazier;

int main()
{
    try {
        ConfigManager::initGlobal();

        Database db(
            global_config->get("database.host", "localhost"),
            global_config->get("database.port", "5432"),
            global_config->get("database.username", "postgres"),
            global_config->get("database.password", ""),
            global_config->get("database.database", "postgres"));

		try {
			MigrationManager::init(db);
		}
        catch (const std::exception& e) {
			Logger::log("Error occurred while initializing migrations: " + std::string(e.what()), "ERROR");
        }
    }
    catch (const std::exception& e) {
        Logger::log(e.what(), "ERROR");
    }
    catch (...) {
        Logger::log("UNKNOWN ERROR", "ERROR");
    }
}