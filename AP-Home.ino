#include <ESP8266WiFi.h>    
#include "AP_Nurse.h"
#include "ClickButton.h"
#include "networkManager.h"

//uncommenting next line enables buzzer
#define _BUZZER

//uncommenting the next line enables debug printout
//#define _DEBUG
#ifdef _DEBUG
#define loopDelay   200
#endif

//uncommenting the next line enables alert level debug printout
//#define _LEVEL

/** Alert control directives
 */
#define muteDuration    2000

/** Encoder transmission enable pin control directives
 */
#define pWindow     5000//ms pair window
#define pWidth      100//ms TE pulse width
#define pInterval   5000//ms periodic RF message advertisement interval

void AP_loop(int alert);
int Alert(level_t enable);
level_t alertLevel(int alert);

void pulse();
void periodicPulse();

/** AP-Nurse Home config initialisation
 */
ap_config_t config_day = { NOISE_TH, SMOKE_TH, GAS_TH, LIGHT_TH, PRESSURE_TH, TEMP_TH, HUMIDITY_TH, PIR_E, NOISE_E, BME_E, EXTENDER_E };
ap_config_t config_night = { NOISE_TH_N, SMOKE_TH_N, GAS_TH_N, LIGHT_TH_N, PRESSURE_TH_N, TEMP_TH_N, HUMIDITY_TH_N, PIR_E_N, NOISE_E_N, BME_E_N, EXTENDER_E_N };

/** AP-Nurse Home object and global variable initialisation
 */
static AP_Nurse_Universal ap_node(config_day);//ap nurse control interface
static AP_Nurse_Universal ap_node_night(config_night);//ap nurse night control interface
ClickButton button(BUTTON_PIN, HIGH, CLICKBTN_PULLDOWN);//button handler
volatile bool muted = false;
volatile bool wasAlert = false;
volatile long muteStart = 0;
volatile long lastPulse = 0;

static long advTime = 0;
static long diffAdvTime = 0;
static ap_node_t oldData;

void setup() {
    //Button setup
    button.debounceTime = 50;
    button.multiclickTime = 250;
    button.longClickTime = 4000;

    //Serial comm setup
    Serial.begin(115200);
    Serial.println("AP Nurse Universal V 1.0 Booted Succesfully ^^");
    Serial.println("Pairing...");
    Serial.println("End of pairing window...");

    #ifdef CARE_OVERRIDE
    //Start WiFi manager
    runWifiManager();
    advTime = diffAdvTime = millis();
    #endif
}//setup

void loop() {
    button.Update();//updates button state
    int alert = 0;
    bool d_n = digitalRead(DAY_NIGHT);

    #ifndef CARE_OVERRIDE
    AP_loop(alert);//ap node loop body
    periodicPulse();//periodic RF message advertisement
    #endif

    if (d_n) {//sensor data update, basedd on D/N setting
        alert = ap_node_night.update();
    } else {
        alert = ap_node.update();
    }//if (d_n)

    #ifdef CARE_OVERRIDE
    ap_node_t data;
    if (d_n) {//sensor data advertisement, based on D/N setting
        //advertiseData(ap_node_night.getLastData());
        data = ap_node_night.getLastData();
    } else {
        //advertiseData(ap_node.getLastData());
        data = ap_node.getLastData();
    }//if (d_n)

    if ((millis() - advTime) >= (ADVERTISEMENT_INTERVAL * 12 * 1000)) {
        advertiseData(data);
        advTime = millis();
    }

    diffAdv(data, oldData);

    oldData = data;
    delay(ADVERTISEMENT_INTERVAL * 1000);
    #endif

    #ifdef _DEBUG
    delay(loopDelay);
    
    if (d_n){
        ap_node_night.printData();
    } else {
        ap_node.printData();
    }
    #endif
}//loop

//Ap node main loop body
void AP_loop(int alert) {
    //Alert debug
    if (0 < alert) {
        char buffer[9];
        sprintf(&buffer[0], BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(alert));
        Serial.print("STATUS: ");
        Serial.println(buffer);  
    }else{
        Serial.println(alert); //debug alert
    }//if (0 < alert)
    
    //Alert handling
    if (alert&&(!muted)) {
        level_t condition = alertLevel(alert);
        Alert(condition);
        muted = true;
        muteStart = millis();
    }//if (alert&&(!muted))
    if (muted&&((millis() - muteStart) >= muteDuration)) {
        muted = false;
        Alert(NO_ALERT);
    }//if (muted&&((millis() - muteStart) >= muteDuration))
    
    //Button handling
    if((button.clicks == 1)){
        //Serial.println("ťuk");
        muted = true;
        Alert(NO_ALERT);
    }
}//AP_loop

//Alert handler
int Alert(level_t enable) {
    #ifdef _LEVEL
    Serial.printf("a level: %d\r\n", enable);
    #endif

    if (enable) {
        digitalWrite(ENCODER_PIN, LOW);

        if (enable & CRITICAL) { //in case of a critical condition
            digitalWrite(ENCODER_PIN2, LOW);
        }

        #ifdef _BUZZER
        digitalWrite(BUZZER_PIN, LOW);
        #endif

        wasAlert = true;
        pulse();
        return 1;
    }//if (enable)

    digitalWrite(ENCODER_PIN, HIGH);
    digitalWrite(ENCODER_PIN2, HIGH);
    wasAlert = false;

    #ifdef _BUZZER
    digitalWrite(BUZZER_PIN, HIGH);
    #endif

    pulse();
    return 0;
}//Alert

level_t alertLevel(int alert) {

    #ifdef BED
    if (alert & (MOTION_ALERT | SMOKE_ALERT | GAS_ALERT | NOISE_ALERT)) {
        return CRITICAL;
    } else if ((alert & LIGHT_ALERT) && (alert & MOTION_ALERT)) {
        return ABNORMAL;
    } else {
        return NO_ALERT;
    }//(alert & (MOTION_ALERT | SMOKE_ALERT | GAS_ALERT | NOISE_ALERT))
    #endif

    #ifdef DOOR
    if (alert & STUCK_ALERT) {
        return CRITICAL;
    } else if (alert & MOTION_ALERT) {
        return ABNORMAL;
    } else {
        return NO_ALERT;
    }//if (alert & STUCK_ALERT)
    #endif

    #ifdef HALLWAY
    if (alert & MOTION_ALERT) {
        return CRITICAL;
    } else if (alert & NOISE_ALERT) {
        return ABNORMAL;
    } else {
        return NO_ALERT;
    }//if (alert & MOTION_ALERT) 
    #endif

    #ifdef KITCHEN
    if (alert & (0xffff - LIGHT_ALERT)) {
        return CRITICAL;
    } else if (alert & LIGHT_ALERT) {
        return ABNORMAL;
    } else {
        return NO_ALERT;
    }//if (alert & (0xffff - LIGHT_ALERT)) 
    #endif
}

//Encoder transmission enable handler
void pulse() {
    digitalWrite(TE, LOW);
    delay(pWidth);
    digitalWrite(TE, HIGH);
}//pulse

//Periodic RF message advertisement
void periodicPulse() {
    if ((millis() - lastPulse) >= pInterval) {
        pulse();
        lastPulse = millis();
    }
}//periodicPulse
