#include <ESP8266WiFi.h>    
#include "AP_Nurse.h"
#include "ClickButton.h"

void trigger(bool &state);

//uncommenting the next line enables debug printout
//#define _TRIGLOOP
#define loopDelay   200
static bool state = true;

void setup(){
    //encoder pin init
    pinMode(ENCODER_PIN, OUTPUT);
    pinMode(ENCODER_PIN2, OUTPUT);

    //encoder pin log level setup
    digitalWrite(ENCODER_PIN, HIGH);
    digitalWrite(ENCODER_PIN2, HIGH);

    //serial com setup
    Serial.begin(115200);
    Serial.println("AP Nurse Universal V 1.0 Booted Succesfully ^^");
}//setup

void loop(){
    delay(loopDelay);
    #ifdef _TRIGLOOP
    trigger(state);
    #endif
}//loop

void trigger(bool &state){
    if(state){
        digitalWrite(ENCODER_PIN, LOW);
        digitalWrite(ENCODER_PIN2, LOW);
    }else{
        digitalWrite(ENCODER_PIN, HIGH);
        digitalWrite(ENCODER_PIN2, HIGH);
    }//if(state)

    state = !state;
}//trigger
