#include "pghandler.hpp"

PgHandler::PgHandler(std::string connectionString) {
    try {
        conn = new pqxx::connection(connectionString);
        if (conn->is_open()) {
            std::cout << "Opened database successfully: " << conn->dbname() << std::endl;
        } else {
            std::cout << "Can't open database" << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void PgHandler::insertBMEData(float temperature, float pressure, float humidity) {
     pqxx::work txn(*conn);
     std::ostringstream stream;
     time_t t = time(0);
     stream << "INSERT INTO bmedata (\"temperature\",\"pressure\",\"humidity\",\"datetime\") VALUES (" << temperature << "," << pressure << "," <<humidity << ", to_timestamp(" << t << "))";
     pqxx::result r = txn.exec(stream.str());
     txn.commit();
}
