#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiManager.h>
#include <WiFiClientSecureBearSSL.h>

const char* firmwareUrl = "https://raw.githubusercontent.com/multitek-indo/OTA-ESP8266/main/v1.0.6.bin"; // Gunakan URL GitHub Releases

void setup() {
    Serial.begin(115200);
    WiFiManager wifiManager;
    wifiManager.autoConnect("ESP8266-OTA");
    
    Serial.println("Connected to WiFi");
    
    if (WiFi.status() == WL_CONNECTED) {
        std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
        client->setInsecure(); // Abaikan sertifikat SSL

        Serial.println("Checking for firmware update...");
        
        HTTPClient http;
        http.begin(*client, firmwareUrl);
        int httpCode = http.GET();
        
        Serial.printf("HTTP Response Code: %d\n", httpCode);
        
        if (httpCode == HTTP_CODE_OK) {
            Serial.println("Starting OTA update...");
            t_httpUpdate_return ret = ESPhttpUpdate.update(*client, firmwareUrl);
            
            switch (ret) {
                case HTTP_UPDATE_FAILED:
                    Serial.printf("Update failed! Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                    break;
                case HTTP_UPDATE_NO_UPDATES:
                    Serial.println("No new updates available.");
                    break;
                case HTTP_UPDATE_OK:
                    Serial.println("Update successful! Restarting...");
                    ESP.restart();
                    break;
            }
        } else {
            Serial.println("Failed to fetch firmware file. Check URL or server!");
        }
        
        http.end();
    } else {
        Serial.println("WiFi not connected, skipping update.");
    }
}

void loop() {
    // Tidak ada kode dalam loop karena OTA update dilakukan di setup
}
