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

#define STUCK_TIME 600000//ms
#define STUCK_MUTE 20000//ms
#define DIFF_K  0.05//*100%

bool mHold = false;
bool wasMotion = false;
long motionTime = 0;
bool stuckMute = false;
long stuckMuteTime = 0;

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
    http.begin(serverName);
    delay(100);

    sprintf(&payload[0], "{\
    \"sn\": \"%s\",\
    \"kid\": \"%s\",\
    \"body\":\
    [{ \"LoggerName\": \"PIR\", \"MeasuredData\": [{ \"Name\": \"motion\",\"Value\": %d }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" } \
    ]}", SN, kid, data.lastMotion, myId);
    Serial.println();
    Serial.println(payload);
    Serial.println();

    http.addHeader("Content-Type", "application/json");

    int ret = http.POST(payload);
    //kontrola responsu
    if(ret != 200){
      Serial.printf("ret: %d", ret);
    } else {
      Serial.println("OK");
    }

    delay(100);

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
    [{ \"LoggerName\": \"M1\", \"MeasuredData\": [{ \"Name\": \"smoke\",\"Value\": %.2f }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" } \
    ]}", SN, kid, data.bmeSmoke, myId);
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
    ]}", SN, kid, data.lastPressure, myId);
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


int diffAdv(ap_node_t data, ap_node_t oldData) {
    bool stuck = false;
    int ret = 0;


    http.begin(serverName);
    delay(100);

    #ifdef DOOR
    if (!mHold) {
      if (stuckMute) {
        if ((millis() - stuckMuteTime) >= STUCK_MUTE) {
          stuckMute = false;
        }
      }

      if (data.lastMotion && (!stuckMute)) {
        if (!wasMotion) {
          wasMotion = true;
          motionTime = millis();
          Serial.println("mot");
        }else{
          wasMotion = false;
          stuckMute = true;
          stuckMuteTime = millis();
        }
      }

      if (wasMotion && ((millis() - motionTime) >= STUCK_TIME)) {
        stuck = true;
        wasMotion = false;
      }
    }

    mHold = data.lastMotion;
    #endif

    if (data.lastMotion != oldData.lastMotion) {
      sprintf(&payload[0], "{\
      \"sn\": \"%s\",\
      \"kid\": \"%s\",\
      \"body\":\
      [{ \"LoggerName\": \"PIR\", \"MeasuredData\": [{ \"Name\": \"motion\",\"Value\": %d }, { \"Name\": \"stuck\",\"Value\": %d }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" } \
      ]}", SN, kid, data.lastMotion, stuck, myId);
      Serial.println();
      Serial.println(payload);
      Serial.println();

      http.addHeader("Content-Type", "application/json");

      int ret = http.POST(payload);
      //kontrola responsu
      if(ret != 200){
        Serial.printf("ret: %d", ret);
      } else {
        Serial.println("OK");
      }

      delay(100);

      http.addHeader("Content-Type", "application/json");

      ret = http.POST(payload);
      //kontrola responsu
      if(ret != 200){
        Serial.printf("ret: %d", ret);
      } else {
        Serial.println("OK");
      }

      delay(100);
    }

    if (stuck) {
      sprintf(&payload[0], "{\
      \"sn\": \"%s\",\
      \"kid\": \"%s\",\
      \"body\":\
      [{ \"LoggerName\": \"PIR\", \"MeasuredData\": [{ \"Name\": \"stuck\",\"Value\": %d }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" } \
      ]}", SN, kid, stuck, myId);
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
    }

    if (diffCheckF(data.bmeSmoke, oldData.bmeSmoke)) {
      sprintf(&payload[0], "{\
      \"sn\": \"%s\",\
      \"kid\": \"%s\",\
      \"body\":\
      [{ \"LoggerName\": \"M1\", \"MeasuredData\": [{ \"Name\": \"smoke\",\"Value\": %.2f }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" } \
      ]}", SN, kid, data.bmeSmoke, myId);
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
    }
    
    if (diffCheck(data.lastGas, oldData.lastGas)) {
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
    }

    if (diffCheck(data.lastLight, oldData.lastLight)) {
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
    }

    if (diffCheck(data.lastPressure, oldData.lastPressure)) {
      sprintf(&payload[0], "{\
      \"sn\": \"%s\",\
      \"kid\": \"%s\",\
      \"body\":\
      [{ \"LoggerName\": \"FSR\", \"MeasuredData\": [{ \"Name\": \"pressure\",\"Value\": %d }], \"ServiceData\": [], \"DebugData\": [], \"DeviceId\": \"%s\" } \
      ]}", SN, kid, data.lastPressure, myId);
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
    }

    http.end();

    return ret;
}

bool diffCheck(uint8_t val, uint8_t oldVal) {
  return (((val - oldVal) >= (DIFF_K * val)) || ((oldVal - val) >= (DIFF_K * val)));
}

bool diffCheckF(float val, float oldVal) {
  return (((val - oldVal) >= (DIFF_K * val)) || ((oldVal - val) >= (DIFF_K * val)));
}