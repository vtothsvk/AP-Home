#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <String.h>
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

#endif