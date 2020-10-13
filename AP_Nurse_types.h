#ifndef AP_Nurse_types_h
#define AP_Nurse_types_h

/** ADC Extender I2C address
 */
#define EXTENDER_ADDR   (0x48)

/** AP Alert macros
 */
#define ENABLE 1
#define DISABLE 0

/** AP Nurse Alert type
 */
#ifndef ap_alert_type
#define ap_alert_type
typedef enum ap_alert{
    STATUS_OK = 0,
    MOTION_ALERT = 1,
    NOISE_ALERT = 2,
    SMOKE_ALERT = 4,
    GAS_ALERT = 8,
    LIGHT_ALERT = 16,
    PRESSURE_ALERT = 32,
    GENERAL_ALERT = -1000,
    I2C_NO_DATA = -1001
}status_t;
#endif

/** AP Nurse sensor node data structure
 */
#ifndef ap_sensor_node_struct
#define ap_sensor_node_struct
typedef struct ap_sensor_node{
    uint8_t lastAlert;
    bool lastMotion = false;
    unsigned long lastEcheck;
    union{
        uint8_t lastVal[6];
        struct{
            uint8_t lastNoise, lastSmoke, lastGas, lastLight, lastPressure, lastTemperature;
        };
    };
}ap_node_t;
#endif

/** AP Nurse sensor node alert threshold structure
 */
#ifndef ap_node_threshold_type
#define ap_node_threshold_type
typedef union ap_node_thresholds{
    uint8_t TH[6];
    struct{
        uint8_t noiseTH, smokeTH, gasTH, lightTH, pressureTH, tempTH;
    };
}ap_threshold_t;
#endif

#endif