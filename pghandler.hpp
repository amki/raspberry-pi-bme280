#include <iostream>
#include <pqxx/pqxx>
#include <ctime>

class PgHandler {
    private:
        pqxx::connection *conn;
    public:
        PgHandler(std::string connectionString);
        void insertBMEData(float temperature, float pressure, float humidity);
};

