#ifndef AP_CONFIG_H
#define AP_CONFIG_H

#include "AP_Nurse_types.h"

/** AP-Nurse Home version selection directives
 * 
 *  @note select only one
 */
//#define BED
#define DOOR
//#define HALLWAY
//#define KITCHEN

/** Threshold and sensor enable directives 
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

/** AP-Nurse Home DOOR version config
 */
#define STUCK_TIMER_DELAY   5//s

ap_config_t config_day = { NOISE_TH, SMOKE_TH, GAS_TH, LIGHT_TH, PRESSURE_TH}

#endif