#include "pghandler.hpp"

PgHandler::PgHandler(std::string connectionString) {
    try {
        pqxx::connection C(connectionString);
        if (C.is_open()) {
            std::cout << "Opened database successfully: " << C.dbname() << std::endl;
        } else {
            std::cout << "Can't open database" << std::endl;
        }
        C.disconnect ();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
