#include <ESP8266WiFi.h>    
#include "AP_Nurse.h"
#include "ClickButton.h"

//uncommenting next line enables buzzer
//#define _BUZZER 

#define muteDuration    2000

#define pWidth  100

/** Threshold directives  
 */ 
#define NOISE_TH        255
#define SMOKE_TH        255
#define GAS_TH          60
#define LIGHT_TH        80
#define PRESSURE_TH     80
#define TEMP_TH         255

void AP_loop(uint8_t alert);
int Alert(bool enable);

void pulse();

AP_Nurse_Universal ap_node(NOISE_TH, SMOKE_TH, GAS_TH, LIGHT_TH, PRESSURE_TH, TEMP_TH);
ClickButton button(BUTTON_PIN, LOW, CLICKBTN_PULLUP);
volatile bool muted = false;
volatile bool wasAlert = false;;
volatile long muteStart = 0;

void setup(){
    //Button setup
    button.debounceTime = 50;
    button.multiclickTime = 250;
    button.longClickTime = 4000;

    //Serial com setup
    Serial.begin(115200);
    Serial.println("AP Nurse Universal V 1.0 Booted Succesfully ^^");
}

void loop(){
    button.Update();//updates button state
    uint8_t alert = ap_node.update();//updates sensor data
    AP_loop(alert);//ap node loop body
}

//Ap node main loop body
void AP_loop(uint8_t alert){
    //Alert debug
    if(0 < alert){
        char buffer[9];
        sprintf(&buffer[0], BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(alert));
        Serial.print(buffer);    
    }else{
        Serial.println(alert); //debug alert
    }//if(0 < alert){
    
    
    //Alert handling
    if(alert&&(!muted)){
        Alert(ENABLE);
        muted = true;
        muteStart = millis();
    }
    if(muted&&((millis() - muteStart) >= muteDuration)){
        muted = false;
        Alert(DISABLE);
    }//if(alert&&(!muted))
    
    //Button handling
    if((button.clicks == 1)){
        Serial.println("ťuk");
    }
}

//Alert handler
int Alert(bool enable){
    if(enable){
        digitalWrite(ENCODER_PIN, LOW);
        digitalWrite(ENCODER_PIN2, LOW);

        #ifdef _BUZZER
        tone(BUZZER_PIN, 1000);
        #endif

        wasAlert = true;
        pulse();
        return 1;
    }

    digitalWrite(ENCODER_PIN, HIGH);
    digitalWrite(ENCODER_PIN2, HIGH);
    wasAlert = false;

    #ifdef _BUZZER
    noTone(BUZZER_PIN);
    #endif

    pulse();
    return 0;
}

//Encoder transmission enable handler
void pulse(){
    digitalWrite(TE, LOW);
    delay(pWidth);
    digitalWrite(TE, HIGH);
}
