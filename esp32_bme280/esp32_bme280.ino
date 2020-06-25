#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "WiFi.h"
#include <WiFiClientSecure.h>

const char* le_root_ca= \
     "-----BEGIN CERTIFICATE-----\n" \
     "MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n" \
     "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
     "DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n" \
     "SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n" \
     "GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n" \
     "AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n" \
     "q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n" \
     "SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n" \
     "Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n" \
     "a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n" \
     "/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n" \
     "AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n" \
     "CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n" \
     "bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n" \
     "c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n" \
     "VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n" \
     "ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n" \
     "MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n" \
     "Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n" \
     "AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n" \
     "uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n" \
     "wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n" \
     "X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n" \
     "PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n" \
     "KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n" \
     "-----END CERTIFICATE-----\n";

const char* wifiPassword = "derkleinekrebskrebstnichtmehr";
char* wifiSSID;

const char*  server = "env.epow0.org";
String chipId;

bool doReconnect = false;
int failedConnects = 0;

Adafruit_BME280 bme;
WiFiClientSecure client;

void connectWifi() {
    doReconnect = false;
    failedConnects = 0;
    do {  
      Serial.println("WIFI pre-config done, starting scan...");
      // WiFi.scanNetworks will return the number of networks found
      int n = WiFi.scanNetworks();
      if (n < 1) {
          Serial.println("WIFI: no networks found");
          delay(1000);
      } else {
          wifiSSID = pickWifiNetwork(n);
      }
    } while(wifiSSID == "");
    WiFi.begin(wifiSSID,wifiPassword);
    int cnt = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        cnt++;
        if(cnt > 15) {
            Serial.println("Abort waiting and retry.");
            doReconnect = true;
            break;
        }
    }

    if(doReconnect == false) {
        Serial.println("WiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.println("");
    }
}

char* pickWifiNetwork(int n) {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
        if(WiFi.SSID(i).startsWith("thanksforallthe")) {
            Serial.println("Picking "+WiFi.SSID(i)+".");
            return (char*)WiFi.SSID(i).c_str();
        }
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
    }
    return "";
}

void WifiDisconnect(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.println("WiFi disconnected. Retrying...");
    wifiSSID = "";
    doReconnect = true;
}

void setup() {
    Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    
    client.setCACert(le_root_ca);

    uint64_t id = ESP.getEfuseMac();
    unsigned long long1 = (unsigned long)((id & 0xFFFF0000) >> 16 );
    unsigned long long2 = (unsigned long)((id & 0x0000FFFF));
    chipId = (String(long1, HEX) + String(long2, HEX));
    
    connectWifi();

    WiFi.onEvent(WifiDisconnect, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
    
    Serial.println("Initialize BME280 Sensor...");
    unsigned status = bme.begin(0x76);
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
}

void loop() {
    if(doReconnect == true) {
        connectWifi();
    }
    float temp = bme.readTemperature();
    float pres = bme.readPressure() / 100.0F;
    float humi = bme.readHumidity();
    /*
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" C");
    Serial.print("Pressure: ");
    Serial.print(pres);
    Serial.println(" hPa");
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.println(" %");
    Serial.println();
    Serial.println("Send this to the server...");
    */
    if (!client.connect(server, 443)) {
        Serial.println("Connection failed!");
        failedConnects++;
        if(failedConnects > 10) {
            Serial.println("Shit!");
            doReconnect = true;
        }
    } else {
        failedConnects = 0;
        String body = String("{\"i\":\""+chipId+"\""+
            ",\"t\":"+String(temp)+
            ",\"p\":"+String(pres)+
            ",\"h\":"+String(humi)+"}");
        Serial.println("Connected to server!");
        // Make a HTTP request:
        client.println("POST /api/bme280 HTTP/1.1");
        client.println("Host: env.epow0.org");
        client.println("Connection: close");
        client.println("Content-Length: "+String(body.length()));
        client.println("Content-Type: application/json");
        client.println();
        client.println(body);
        client.println();
    
        while (client.connected()) {
          String line = client.readStringUntil('\n');
          if (line == "\r") {
            //Serial.println("headers received");
            break;
          }
        }
        // if there are incoming bytes available
        // from the server, read them and print them:
        while (client.available()) {
          char c = client.read();
          //Serial.write(c);
        }
        client.stop();
    }
    delay(10000);
}
