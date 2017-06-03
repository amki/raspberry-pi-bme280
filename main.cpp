#include <iostream>
#include <thread>

#include "sense.h"

int main(int argc, char* argv[]) {
    std::cout << "HALLO" << std::endl;

    init_bme280();
    std::thread t_database(init_db);

    t_database.join();
}

