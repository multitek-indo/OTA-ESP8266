#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <WiFiManager.h>

const char* firmwareUrl = "https://github.com/YOUR_USERNAME/YOUR_REPO/releases/latest/download/firmware.bin"; // Ganti dengan URL firmware di GitHub

void setup() {
    Serial.begin(115200);
    WiFiManager wifiManager;
    wifiManager.autoConnect("ESP8266-OTA");
    
    Serial.println("Connected to WiFi");
    
    // Mulai OTA Update
    WiFiClient client;
    Serial.println("Checking for firmware update...");
    t_httpUpdate_return ret = ESPhttpUpdate.update(client, firmwareUrl);
    
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
}

void loop() {
    // Tidak ada kode dalam loop karena OTA update dilakukan di setup
}
