#include "AP_Nurse.h"

PCF8591 extender(0, false);

/** AP_nurse methods definitions
 */
AP_Nurse::AP_Nurse(){
    //I/O init
    #ifndef _DISABLE_PIR
    pinMode(PIR_PIN, INPUT);
    #endif
    pinMode(NOISE_PIN, INPUT);
    pinMode(DAY_NIGHT, INPUT);
    pinMode(TE, OUTPUT);
    pinMode(ENCODER_PIN, OUTPUT);
    pinMode(ENCODER_PIN2, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    //pin state init
    digitalWrite(TE, HIGH);
    digitalWrite(ENCODER_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);

    //I2C init
    Wire.begin();

    //get program start time
    this -> ap_node.lastEcheck = millis();
    //Wire.pins(I2C_SDA, I2C_SCL);
}

uint8_t AP_Nurse::getLastAlert(){
    return (this -> ap_node.lastAlert);
}

bool AP_Nurse::getLastMotion(){
    return (this -> ap_node.lastMotion);
}

uint8_t AP_Nurse::getLastNoise(){
    return (this -> ap_node.lastNoise);
}

uint8_t AP_Nurse::getLastSmoke(){
    return (this -> ap_node.lastVal[1]);
}

uint8_t AP_Nurse::getLastGas(){
    return (this -> ap_node.lastVal[2]);
}

uint8_t AP_Nurse::getLastLight(){
    return (this -> ap_node.lastVal[3]);
}

uint8_t AP_Nurse::getLastPressure(){
    return (this -> ap_node.lastVal[4]);
}

void AP_Nurse::printData(){
    Serial.println();
    Serial.print("Motion:    ");
    Serial.println(this -> getLastMotion());
    Serial.print("Noise:     ");
    Serial.println(this -> getLastNoise());
    Serial.print("Smoke:     ");
    Serial.println(this -> getLastSmoke());
    Serial.print("Gas:       ");
    Serial.println(this -> getLastGas());
    Serial.print("Light:     ");
    Serial.println(this -> getLastLight());
    Serial.print("Pressure:  ");
    Serial.println(this -> getLastPressure());
    Serial.print("Button:    ");
    Serial.println(digitalRead(BUTTON_PIN));
}

status_t AP_Nurse::checkMotion(){
    this -> ap_node.lastMotion = digitalRead(PIR_PIN);
    if(this -> ap_node.lastMotion){
        this -> ap_node.lastAlert |= MOTION_ALERT;
        return MOTION_ALERT;
    }
    
    this -> ap_node.lastAlert &= (0xff - MOTION_ALERT);
    return STATUS_OK;
}

status_t AP_Nurse::checkNoise(){
    this -> ap_node.lastNoise = analogRead(NOISE_PIN);
    if(this -> ap_node.lastNoise >= this -> ap_th.noiseTH){
        this -> ap_node.lastAlert |= NOISE_ALERT;
        return NOISE_ALERT;
    }

    this -> ap_node.lastAlert &= (0xff - NOISE_ALERT);
    return STATUS_OK;
}

status_t AP_Nurse::checkExtender(){

    
    bool wasAlert = false;
    
    //byte buffer = Wire.read();
    for(uint8_t i = 0; i < 4; i++){
        //this -> ap_node.lastVal[i+1] = Wire.read();
        this -> ap_node.lastVal[i+1] = (uint8_t)(extender.adc_read(i, 1) * 255);
        if(this -> ap_node.lastVal[i+1] >= this -> ap_th.TH[i+1]){
            this -> ap_node.lastAlert |= (1 << (i + 2));
            wasAlert = true;
        }else{
            this -> ap_node.lastAlert &= (0xff - (1<< (i + 2)));
        }
        delay(50);
    }
    //Wire.endTransmission();

    //Extender debug
    
    for(uint8_t i = 0; i < 4; i++){
        Serial.print(i);
        Serial.print(": ");
        Serial.println(this -> ap_node.lastVal[i+1]);
    }

    if(wasAlert){
        return GENERAL_ALERT;
    }

    return STATUS_OK;
}

status_t AP_Nurse::checkTemperature(){
    return STATUS_OK;
}

/** AP_Nurse_Room methods definitions
 */
uint8_t AP_Nurse_Room::update(){
    this -> checkMotion();
    this -> checkNoise();
    this -> checkExtender();

    return this -> ap_node.lastAlert;
}

/** AP_Nurse_Kitchen methods definitions
 */
uint8_t AP_Nurse_Kitchen::update(){
    this -> checkMotion();
    this -> checkTemperature();
    this -> checkExtender();

    return this -> ap_node.lastAlert;
}

/** AP_Nurse_Kitchen methods definitions
 */
uint8_t AP_Nurse_Hallway::update(){
    this -> checkMotion();
    this -> checkTemperature();

    return this -> ap_node.lastAlert;
}

/** AP_nurse_Universal methods definitions
 */
uint8_t AP_Nurse_Universal::update(){
    #ifndef _DISABLE_PIR
    this -> checkMotion();
    #endif
    this -> checkNoise();
    //this -> checkTemperature();
    //this -> checkExtender();
    if((millis() - this -> ap_node.lastEcheck) >= 5000){
        this -> checkExtender();
        this -> ap_node.lastEcheck = millis();
    }

    return this -> ap_node.lastAlert;
}
