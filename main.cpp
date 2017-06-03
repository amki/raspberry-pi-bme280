#include <iostream>
#include <thread>

#include "lib/pistache/include/endpoint.h"
#include "sense.h"
#include "pghandler.hpp"

using namespace Net;

class HelloHandler : public Http::Handler {
public:

    HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) {
        if ((request.resource() == "/" || request.resource() == "/index.html")) {
                Http::serveFile(response, "public/index.html");
        } else if (request.resource() == "/code.js") {
                Http::serveFile(response, "public/code.js");
        } else if(request.resource() == "/api/sense") {
            std::ostringstream strstream;
            strstream << "Hallo";
            strstream << "Temperature is: " << ((float)data.v_comp_temp_s32[0])/100 << std::endl;
            strstream << "Pressure is: " << ((float)data.v_comp_press_u32[0])/256 << std::endl;
            strstream << "Humidity is: " << ((float)data.v_comp_humidity_u32[0])/1024 << std::endl;
            response.send(Http::Code::Ok, strstream.str());
        }
    }
};

void init_http() {
    Net::Address addr(Net::Ipv4::any(), Net::Port(8088));

    auto opts = Http::Endpoint::options().threads(1);
    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(std::make_shared<HelloHandler>());
    server.serve();
}

int main(int argc, char* argv[]) {
    std::cout << "HALLO" << std::endl;

    PgHandler pgH("dbname = sensei user = sensei password = ohCou2rei9ouL0ae hostaddr = 127.0.0.1 port = 5432");

    std::thread t_bme280(init_bme280);
    init_http();

    t_bme280.join();

}

