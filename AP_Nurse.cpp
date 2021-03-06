#include "AP_Nurse.h"

PCF8591 extender(0, false);

/** AP_nurse methods definitions
 */
AP_Nurse::AP_Nurse(){
    //I/O init
    #ifdef PIR_ENABLE
    pinMode(PIR_PIN, INPUT);
    #endif

    #ifdef NOISE_ENABLE
    pinMode(NOISE_PIN, INPUT);
    #endif

    pinMode(DAY_NIGHT, INPUT);
    pinMode(TE, OUTPUT);
    pinMode(ENCODER_PIN, OUTPUT);
    pinMode(ENCODER_PIN2, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    //pin state init
    digitalWrite(TE, HIGH);
    digitalWrite(ENCODER_PIN, HIGH);
    digitalWrite(ENCODER_PIN2, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);

    //I2C init
    Wire.begin();

    //BME280 init
    #ifdef BME_ENABLE
    this -> bme.begin();

    #ifdef CARE_OVERRIDE
    this -> bme.setTemperatureOversampling(BME680_OS_8X);
    this -> bme.setHumidityOversampling(BME680_OS_2X);
    this -> bme.setPressureOversampling(BME680_OS_4X);
    this -> bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    this -> bme.setGasHeater(320, 150);
    #endif
    #endif

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

float AP_Nurse::getLastTemperature(){
    return (this -> ap_node.lastTemperature);
}

float AP_Nurse::getLastHumidity(){
    return (this -> ap_node.lastHumidity);
}

float AP_Nurse::getLastAPressure(){
    return (this -> ap_node.lastAPressure);
}

ap_node_t AP_Nurse::getLastData(){
    return this -> ap_node;
}

void AP_Nurse::printData(){
    Serial.println();
  
    #ifdef PIR_ENABLE
    Serial.printf("Motion:      %d\r\n", this -> getLastMotion());
    #endif

    #ifdef NOISE_ENABLE
    Serial.printf("Noise:       %d\r\n", this -> getLastNoise());
    #endif

    #ifdef EXTENDER_ENABLE
    Serial.printf("Smoke:       %d\r\n", this -> getLastSmoke());
    Serial.printf("Gas:         %d\r\n", this -> getLastGas());
    Serial.printf("Light:       %d\r\n", this -> getLastLight());
    Serial.printf("Pressure:    %d\r\n", this -> getLastPressure());
    #endif

    #ifdef BME_ENABLE
    Serial.printf("Temperature: %.1f\r\n", this -> getLastTemperature());
    Serial.printf("Humidity:    %.1f\r\n", this -> getLastHumidity());
    Serial.printf("APressure:   %.1f\r\n", this -> getLastAPressure());
    #endif

    Serial.printf("Button:      %d\r\n", digitalRead(BUTTON_PIN));
}

status_t AP_Nurse::checkMotion(){
    this -> ap_node.lastMotion = digitalRead(PIR_PIN);
    if(this -> ap_node.lastMotion){
        this -> ap_node.lastAlert |= MOTION_ALERT;
        #ifdef DOOR
        if ((this -> ap_node.isTimer_a) & this -> ap_node.isMotionCheck_n) {
            this -> disableTimer();
        } else if (this -> ap_node.isMotionCheck_n) {
            this -> startTimer();
        }
        this -> ap_node.isMotionCheck_n = false;
        #endif
        return MOTION_ALERT;
    }
    
    this -> ap_node.lastAlert &= (0xff - MOTION_ALERT);
    this -> ap_node.isMotionCheck_n = true;
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
        this -> ap_node.lastVal[i] = (uint8_t)(extender.adc_read(i, 1) * 255);
        if(this -> ap_node.lastVal[i] > this -> ap_th.TH[i]){
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

status_t AP_Nurse::checkBme(){
    int ret = STATUS_OK;

    #ifndef CARE_OVERRIDE
    if((this -> ap_node.lastTemperature = this -> bme.readTemperature()) <= this -> ap_th.tempTH){
        ret |= TEMPERATURE_ALERT;
        this -> ap_node.lastAlert |= TEMPERATURE_ALERT;
    }else{
        this -> ap_node.lastAlert &= (0xff - TEMPERATURE_ALERT);
    }
    this -> ap_node.lastHumidity = this -> bme.readHumidity();
    this -> ap_node.lastAPressure = this -> bme.readPressure();
    this -> ap_node.lastSmoke = this -> bme.readGas();
    #else
    if (! this -> bme.performReading()) {
        Serial.println("Failed to perform bme reading");
        return I2C_NO_DATA;
    }

    if((this -> ap_node.lastTemperature = this -> bme.temperature) <= this -> ap_th.tempTH){
        ret |= TEMPERATURE_ALERT;
        this -> ap_node.lastAlert |= TEMPERATURE_ALERT;
    }else{
        this -> ap_node.lastAlert &= (0xff - TEMPERATURE_ALERT);
    }
    this -> ap_node.lastHumidity = this -> bme.humidity;
    //this -> ap_node.lastAPressure = this -> bme.readPressure();
    this -> ap_node.bmeSmoke = this -> bme.gas_resistance;
    #endif

    Serial.printf("t: %.2f\r\nh: %.2f\r\nsmoke: %.2f\r\n",
        this -> ap_node.lastTemperature,
        this -> ap_node.lastHumidity,
        this -> ap_node.bmeSmoke
    );

    return (status_t)ret; 
}

void AP_Nurse::startTimer() {
    this -> ap_node.isTimer_a = true;
    this -> ap_node.timerStart = millis();
}

void AP_Nurse::checkTimer() {
    if (this -> ap_node.isTimer_a && ((millis() - this -> ap_node.timerStart) >= (STUCK_TIMER_DELAY * 1000))) {
        this -> ap_node.lastAlert |= STUCK_ALERT;
    }
}

void AP_Nurse::disableTimer() {
    this -> ap_node.isTimer_a = false;
}


void AP_Nurse::timerISR(void *pArg){
    this -> ap_node.lastAlert |= STUCK_ALERT;
}

void AP_Nurse::clearAlert(){
    this -> ap_node.lastAlert = STATUS_OK;
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
    this -> checkExtender();

    return this -> ap_node.lastAlert;
}

/** AP_Nurse_Kitchen methods definitions
 */
uint8_t AP_Nurse_Hallway::update(){
    this -> checkMotion();

    return this -> ap_node.lastAlert;
}

/** AP_nurse_Universal methods definitions
 */
uint8_t AP_Nurse_Universal::update(){
    #ifdef PIR_ENABLE
    if (this -> ap_config.enable_pir) {
        this -> checkMotion();
    }
    #endif

    #ifdef DOOR
    this -> checkTimer();
    #endif

    #ifdef NOISE_ENABLE
    if ( this -> ap_config.enable_noise) {
        this -> checkNoise();
    }
    #endif

    #ifdef BME_ENABLE
    if (this -> ap_config.enable_bme) {
        this -> checkBme();
    }
    #endif
    
    #ifdef EXTENDER_ENABLE
    if( this ->ap_config.enable_extender && ((millis() - this -> ap_node.lastEcheck) >= 5000)){
        this -> checkExtender();
        this -> ap_node.lastEcheck = millis();
    }
    #endif

    return this -> ap_node.lastAlert;
}
