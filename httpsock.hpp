#include <iostream>
#include <ctime>
#include <sstream>

#include <string>
#include <curl/curl.h>

class HTTPSock {
    private:
    public:
        HTTPSock(std::string Host, std::string path);
        void sendBMEData(float temperature, float pressure, float humidity);
};

