#ifndef AP_Nurse_h
#define AP_Nurse_h

#include <Arduino.h>
#include <Wire.h>
#include "AP_Nurse_types.h"
#include "AP_Nurse_Home_Pinout.h"
#include "Sensor_enable.h"
#include "PCF8591.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

class AP_Nurse{
public:
    explicit AP_Nurse();
    uint8_t getLastAlert();
    bool getLastMotion();
    uint8_t getLastNoise();
    uint8_t getLastSmoke();
    uint8_t getLastGas();
    uint8_t getLastLight();
    uint8_t getLastPressure();
    float getLastTemperature();
    float getLastHumidity();
    float getLastAPressure();
    void printData();

protected:
    status_t checkMotion();
    status_t checkNoise();
    status_t checkExtender();
    status_t checkBme();

    ap_node_t ap_node;
    ap_threshold_t ap_th;
    Adafruit_BME280 bme;
};

class AP_Nurse_Room : public AP_Nurse{
public:
    explicit AP_Nurse_Room(float lightThreshold, float noiseThreshold, float smokeThreshold) : AP_Nurse(){
        this -> ap_th.lightTH = lightThreshold;
        this -> ap_th.noiseTH = noiseThreshold;
        this -> ap_th.smokeTH = smokeThreshold;
    }
    uint8_t update();
private:
    
};

class AP_Nurse_Kitchen : public AP_Nurse{
public:
    explicit AP_Nurse_Kitchen(float lightThreshold, float temperatureThreshold, float gasThreshold) : AP_Nurse(){
        this -> ap_th.lightTH = lightThreshold;
        this -> ap_th.tempTH = temperatureThreshold;
        this -> ap_th.gasTH = gasThreshold;
    }
    uint8_t update();
private:
};

class AP_Nurse_Hallway : public AP_Nurse{
public:
    explicit AP_Nurse_Hallway(float temperatureThreshold) : AP_Nurse(){
        this -> ap_th.tempTH = temperatureThreshold;
    }
    uint8_t update();
private:
};

class AP_Nurse_Universal : public AP_Nurse{
public:
    explicit AP_Nurse_Universal(float noiseTH, float smokeTH, float gasTH, float lightTH, float pressTH, float tempTH) : AP_Nurse(){
        this -> ap_th.noiseTH = noiseTH;
        this -> ap_th.smokeTH = smokeTH;
        this -> ap_th.gasTH = gasTH;
        this -> ap_th.lightTH = lightTH;
        this -> ap_th.pressureTH = pressTH;
        this -> ap_th.tempTH = tempTH;
    }
    uint8_t update();
private:
};

#endif