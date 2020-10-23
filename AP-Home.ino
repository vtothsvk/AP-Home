#include <ESP8266WiFi.h>    
#include "AP_Nurse.h"
#include "ClickButton.h"

void trigger(bool &state);

//uncommenting the following line enables debug printout
#define _TRIGLOOP
#define loopDelay   2000
static bool state = true;

//uncommenting the following line enables encoder TE control
#define _TECONTROL
#ifdef _TECONTROL
#define pWidth  200
void pulse();
#endif

void setup(){
    //encoder pin init
    pinMode(ENCODER_PIN, OUTPUT);
    pinMode(ENCODER_PIN2, OUTPUT);
    pinMode(TE, OUTPUT);

    //encoder pin log level setup
    digitalWrite(ENCODER_PIN, HIGH);
    digitalWrite(ENCODER_PIN2, HIGH);
    digitalWrite(TE, LOW);

    //serial com setup
    Serial.begin(115200);
    Serial.println("AP Nurse Universal V 1.0 Booted Succesfully ^^");
    delay(10000);
    Serial.println("Starting TrigLoop");
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
        #ifdef _TECONTROL
        pulse();
        #endif
    }else{
        digitalWrite(ENCODER_PIN, HIGH);
        digitalWrite(ENCODER_PIN2, HIGH);
        #ifdef _TECONTROL
        pulse();
        #endif 
    }//if(state)

    state = !state;
}//trigger

#ifdef _TECONTROL
void pulse(){
    digitalWrite(TE, LOW);
    delay(pWidth);
    digitalWrite(TE, HIGH);
}//pulse
#endif
