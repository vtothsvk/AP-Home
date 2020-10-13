#ifndef AP_Nurse_Home_Pinout_h
#define AP_Nurse_Home_Pinout_h

/** AP Nurse Home edition sensor node pinout
 * 
 * @note version 1.0
 */

/** Communication Interfaces pinout
 */
#define I2C_SCL     (5)
#define I2C_SDA     (4)

/** Node sensor pinout  
 */
#define NOISE_PIN   (A0) //KY038
#define PIR_PIN     (14) //SR505

/** Encoder dontroll 
 */
#define TE              (12)//Transmission enable
#define ENCODER_PIN     (0)//Encoder input
#define ENCODER_PIN2    (13)//Encoder input 2

/** Other peripherals pinout
 */
#define BUTTON_PIN      (3)//Utility button
#define BUZZER_PIN      (2)//Buzzer
#define DAY_NIGHT       (16)//Day/Night switch

/** ADC extender channels 
 */
#define GAS     (0)
#define SMOKE   (1)
#define LIGHT   (2)
#define FSR     (3)



#endif