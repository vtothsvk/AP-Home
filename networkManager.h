#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <string.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <ESP8266HTTPClient.h>
//#include "WiFiSettings.h"
#include "authCredentials.h"
#include "AP_Nurse.h"
#include "WiFiManager.h"
#include "AP_Nurse_config.h"

void runWifiManager(void);
int advertiseData(ap_node_t data);
int diffAdv(ap_node_t data, ap_node_t oldData);
bool diffCheck(uint8_t val, uint8_t oldVal, float K);
bool diffCheckF(float val, float oldVal, float K);

#endif