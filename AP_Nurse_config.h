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

/** AP-Nurse Home DOOR version config
 */
#define STUCK_TIMER_DELAY   5//s

/** AP-Nurse Care version override
 * 
 *  @note uncommenting the following line overrides Home system version logic with Care version data advertisement
 */
//#define CARE_OVERRIDE

/** AP-Nurse Care advertisement interval
 * 
 *  @note using times less than 5s not recommended
 */
#define ADVERTISEMENT_INTERVAL  5//s

#endif
