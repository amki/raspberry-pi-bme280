#include "httpsock.hpp"

HTTPSock::HTTPSock(std::string host, std::string path) {
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void HTTPSock::sendBMEData(float temperature, float pressure, float humidity) {
    std::string data;
    time_t t = time(0);
    data = "{\"i\":\"23eb27b8\",\"t\":"+std::to_string(temperature)+",\"p\":"+std::to_string(pressure)+",\"h\":"+std::to_string(humidity)+"}";
    std::cout << data << std::endl;

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *slist=NULL;
        slist = curl_slist_append(slist, "Content-Type: application/json"); 
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_URL, "https://env.epow0.org/api/bme280");
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        std::cout << readBuffer << std::endl;
    }
}
