#include <brazier/orm.hpp>
#include <brazier/Core>

using namespace brazier;

int main()
{
    try {
        ConfigManager::initGlobal();

        Database db(
            ConfigManager::getGlobal().get("database.host", "localhost"),
            ConfigManager::getGlobal().get("database.port", "5432"),
            ConfigManager::getGlobal().get("database.username", "postgres"),
            ConfigManager::getGlobal().get("database.password", ""),
            ConfigManager::getGlobal().get("database.database", "postgres"));

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