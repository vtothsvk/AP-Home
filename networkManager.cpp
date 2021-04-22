#include "networkManager.h"

const char* serverName = "http://raspberrypi.local:1880/niceBridge";
char payload[1024];
long cStart;

const IPAddress apIP(192, 168, 4, 1);
char apSSID[13] = "nice-AP-1-";
boolean settingMode;
String ssidList;
String wifi_ssid;
String wifi_password;

bool needConnect = true;

HTTPClient http;

void runWifiManager() {
    apSSID[10] = SN[14];
    apSSID[11] = SN[15];

    //WiFiSettings.connect();
    WiFi.mode(WIFI_STA);
    WiFiManager wm;
    bool res;

    res = wm.autoConnect(apSSID, "nicelife2021"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("Connected...");
    }

    //configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

int advertiseData(ap_node_t data) {
    /*sprintf(&payload[0], "\
    [ { \"LoggerName\": \"PIR\", \"Timestamp\": %ld, \"MeasuredData\": [{ \"Name\": \"motion\",\"Value\": %d }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" }, \
    { \"LoggerName\": \"M1\", \"Timestamp\": %ld, \"MeasuredData\": [{ \"Name\": \"smoke\",\"Value\": %.2f }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" }, \
    { \"LoggerName\": \"M2\", \"Timestamp\": %ld, \"MeasuredData\": [{ \"Name\": \"gas\",\"Value\": %.2f }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" }, \
    { \"LoggerName\": \"LDR\", \"Timestamp\": %ld, \"MeasuredData\": [{ \"Name\": \"light\",\"Value\": %d }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" }, \
    { \"LoggerName\": \"FSR\", \"Timestamp\": %ld, \"MeasuredData\": [{ \"Name\": \"pressure\",\"Value\": %d }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" }, \
    { \"LoggerName\": \"BME280\", \"Timestamp\": %ld, \"MeasuredData\": [{ \"Name\": \"temperature\",\"Value\": %.2f }, { \"Name\": \"humidity\",\"Value\": %.2f }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" } \
    ]", epoch, data.lastMotion, myId, epoch, data.lastSmoke, myId, epoch, data.lastGas, myId, epoch, data.lastLight, myId, epoch, data.lastPressure, myId, epoch, data.lastTemperature, data.lastHumidity, myId);
    Serial.println(payload);*/

    sprintf(&payload[0], "{\
    \"sn\": \"%s\",\
    \"kid\": \"%s\",\
    \"body\":\
    [{ \"LoggerName\": \"PIR\", \"MeasuredData\": [{ \"Name\": \"motion\",\"Value\": %d }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" } \
    ]}", SN, kid, data.lastMotion, myId);
    Serial.println();
    Serial.println(payload);
    Serial.println();

    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    int ret = http.POST(payload);
    //kontrola responsu
    if(ret != 200){
      Serial.printf("ret: %d", ret);
    } else {
      Serial.println("OK");
    }

    delay(100);

    sprintf(&payload[0], "{\
    \"sn\": \"%s\",\
    \"kid\": \"%s\",\
    \"body\":\
    [{ \"LoggerName\": \"M1\", \"MeasuredData\": [{ \"Name\": \"smoke\",\"Value\": %d }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" } \
    ]}", SN, kid, data.lastSmoke, myId);
    Serial.println();
    Serial.println(payload);
    Serial.println();

    http.addHeader("Content-Type", "application/json");
    ret = http.POST(payload);
    //kontrola responsu
    if(ret != 200){
      Serial.printf("ret: %d", ret);
    } else {
      Serial.println("OK");
    }

    delay(100);

    sprintf(&payload[0], "{\
    \"sn\": \"%s\",\
    \"kid\": \"%s\",\
    \"body\":\
    [{ \"LoggerName\": \"M2\", \"MeasuredData\": [{ \"Name\": \"gas\",\"Value\": %d }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" } \
    ]}", SN, kid, data.lastGas, myId);
    Serial.println();
    Serial.println(payload);
    Serial.println();

    http.addHeader("Content-Type", "application/json");
    ret = http.POST(payload);
    //kontrola responsu
    if(ret != 200){
      Serial.printf("ret: %d", ret);
    } else {
      Serial.println("OK");
    }

    delay(100);

    sprintf(&payload[0], "{\
    \"sn\": \"%s\",\
    \"kid\": \"%s\",\
    \"body\":\
    [{ \"LoggerName\": \"LDR\", \"MeasuredData\": [{ \"Name\": \"light\",\"Value\": %d }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" } \
    ]}", SN, kid, data.lastLight, myId);
    Serial.println();
    Serial.println(payload);
    Serial.println();

    http.addHeader("Content-Type", "application/json");
    ret = http.POST(payload);
    //kontrola responsu
    if(ret != 200){
      Serial.printf("ret: %d", ret);
    } else {
      Serial.println("OK");
    }

    delay(100);

    sprintf(&payload[0], "{\
    \"sn\": \"%s\",\
    \"kid\": \"%s\",\
    \"body\":\
    [{ \"LoggerName\": \"FSR\", \"MeasuredData\": [{ \"Name\": \"pressure\",\"Value\": %d }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" } \
    ]}", SN, kid, data.lastMotion, myId);
    Serial.println();
    Serial.println(payload);
    Serial.println();

    http.addHeader("Content-Type", "application/json");
    ret = http.POST(payload);
    //kontrola responsu
    if(ret != 200){
      Serial.printf("ret: %d", ret);
    } else {
      Serial.println("OK");
    }

    delay(100);

    sprintf(&payload[0], "{\
    \"sn\": \"%s\",\
    \"kid\": \"%s\",\
    \"body\":\
    [{ \"LoggerName\": \"BME280\", \"MeasuredData\": [{ \"Name\": \"temperature\",\"Value\": %.2f }, { \"Name\": \"humidity\",\"Value\": %.2f }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" } \
    ]}", SN, kid, data.lastTemperature, data.lastHumidity, myId);
    Serial.println();
    Serial.println(payload);
    Serial.println();

    http.addHeader("Content-Type", "application/json");
    ret = http.POST(payload);
    //kontrola responsu
    if(ret != 200){
      Serial.printf("ret: %d", ret);
    } else {
      Serial.println("OK");
    }

    http.end();

    return ret;
}