#include <ESP8266WiFi.h>    
#include "AP_Nurse.h"
#include "ClickButton.h"

//uncommenting next line enables buzzer
//#define _BUZZER

//uncommenting the next line enables debug printout
//#define _DEBUG
#ifdef _DEBUG
#define loopDelay   200
#endif

/** Alert control directives
 */
#define muteDuration    2000

/** Encoder transmission enable pin control directives
 */
#define pWindow     5000//ms pair window
#define pWidth      100//ms TE pulse width
#define pInterval   5000//ms periodic RF message advertisement interval

/** Threshold directives
 *
 *  @note each level represents one 255th of the reference voltage (eg. a threshold of 100 @ 3.3V represents 1.29V)
 */
//Day 
#define NOISE_TH        50
#define SMOKE_TH        100
#define GAS_TH          60
#define LIGHT_TH        80
#define PRESSURE_TH     80
#define TEMP_TH         255

//Night
#define NOISE_TH_N      50
#define SMOKE_TH_N      100
#define GAS_TH_N        60
#define LIGHT_TH_N      80
#define PRESSURE_TH_N   80
#define TEMP_TH_N       255

void AP_loop(uint8_t alert);
int Alert(bool enable);

void pulse();
void periodicPulse();

static AP_Nurse_Universal ap_node(NOISE_TH, SMOKE_TH, GAS_TH, LIGHT_TH, PRESSURE_TH, TEMP_TH);//ap nurse control interface
static AP_Nurse_Universal ap_node_night(NOISE_TH_N, SMOKE_TH_N, GAS_TH_N, LIGHT_TH_N, PRESSURE_TH_N, TEMP_TH_N);//ap nurse night control interface
ClickButton button(BUTTON_PIN, HIGH, CLICKBTN_PULLDOWN);//button handler
volatile bool muted = false;
volatile bool wasAlert = false;
volatile long muteStart = 0;
volatile long lastPulse = 0;

void setup(){
    //Button setup
    button.debounceTime = 50;
    button.multiclickTime = 250;
    button.longClickTime = 4000;

    //Serial comm setup
    Serial.begin(115200);
    Serial.println("AP Nurse Universal V 1.0 Booted Succesfully ^^");
    Serial.println("Pairing...");
    Serial.println("End of pairing window...");
}//setup

void loop(){
    button.Update();//updates button state
    int alert = 0;

    if (digitalRead(DAY_NIGHT)) {//sensor data update, basedd on D/N setting
        alert = ap_node_night.update();
    } else {
        alert = ap_node.update();
    }//if (digitalRead(DAY_NIGHT))
    
    AP_loop(alert);//ap node loop body
    periodicPulse();//periodic RF message advertisement

    #ifdef _DEBUG
    delay(loopDelay);
    ap_node.printData();
    #endif
}//loop

//Ap node main loop body
void AP_loop(uint8_t alert){
    //Alert debug
    if(0 < alert){
        char buffer[9];
        sprintf(&buffer[0], BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(alert));
        Serial.print("STATUS: ");
        Serial.println(buffer);  
    }else{
        Serial.println(alert); //debug alert
    }//if(0 < alert)
    
    
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
        //Serial.println("ťuk");
        muted = true;
        Alert(DISABLE);
    }
}//AP_loop

//Alert handler
int Alert(bool enable){
    if(enable){
        digitalWrite(ENCODER_PIN, LOW);
        digitalWrite(ENCODER_PIN2, LOW);

        #ifdef _BUZZER
        digitalWrite(BUZZER_PIN, LOW);
        #endif

        wasAlert = true;
        pulse();
        return 1;
    }//if(enable)

    digitalWrite(ENCODER_PIN, HIGH);
    digitalWrite(ENCODER_PIN2, HIGH);
    wasAlert = false;

    #ifdef _BUZZER
    digitalWrite(BUZZER_PIN, HIGH);
    #endif

    pulse();
    return 0;
}//Alert

//Encoder transmission enable handler
void pulse(){
    digitalWrite(TE, LOW);
    delay(pWidth);
    digitalWrite(TE, HIGH);
}//pulse

//Periodic RF message advertisement
void periodicPulse(){
    if((millis() - lastPulse) >= pInterval){
        pulse();
        lastPulse = millis();
    }
}//periodicPulse
