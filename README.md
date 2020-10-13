# AP-Home

Custom FW of the AP-Nurse Home node based on the ESP8266 Arduino core.

## Files

* **AP-Home.ino** - main app - Arduino core project
* **AP_Nurse_Home_pinout.h** - board pinout
* **AP_Nurse.h** - AP-Home controll interface header
* **AP_Nurse_types.h** - important type and structure, macro and directive definitions
* **AP_Nurse.cpp** - AP-Home controll interface implementation
* **ClickButton.h** - Button handler library header
* **ClickButton.cpp** - Button handler library implementation
* **PCF8591.h** - I2C analog extender driver header
* **PCF8591.cpp** - I2C analog extender driver implementation
* **LICENCE** - GNU General Public License

## Programming

## IDE setup

To programme, build and flash the app u need **Arduino IDE v 1.8.9** with the **ESP8266 package**

* https://www.arduino.cc/download_handler.php?f=/arduino-1.8.9-windows.exe (Windows)
* https://www.arduino.cc/download_handler.php?f=/arduino-1.8.8-linux64.tar.xz (Linux)

ESP8266 package installation tutorial:

https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/

Ideally u can set up Arduino core projet support within **Visual Studio Code** or **Eclipse IDE**

* https://maker.pro/arduino/tutorial/how-to-use-visual-studio-code-for-arduino (VSCode tutorial)
* https://www.eclipse.org/community/eclipse_newsletter/2017/april/article4.php (Eclipse tutorial)

## Programming

To programme the app open the Arduino core project **AP-Home.ino**

Sensor threshold configuration is handled by the **_TH** directives in **AP-Home.ino**:

```C++
#define NOISE_TH        255
#define SMOKE_TH        255
#define GAS_TH          60
#define LIGHT_TH        80
#define PRESSURE_TH     80
#define TEMP_TH         255
```

## Building

1. The ESP8266 mcu has to be set as the active build **target**:

![Image of Building](https://i.ibb.co/dKZLKC0/Arduino-target.png)

2. The NodeMCU devboard serial **port** has to be selected:

![Image of Serial Port](https://i.ibb.co/NKj9jyp/serial-Port-duino.png)

*your board COM port may vary

### Building (Compiling and linking into a .elf image):

* In Arduino IDE: **CTRL+R**
* In VSCode: **CTRL+ALT+R**
* In Eclipse: **CTRL+B**

### Flashing:

* In Arduino IDE: **CTRL+U**;
* In VSCode: **CTRL+ALT+U**
* In Eclipse: **CTRL+B**

## Debugging

AP-Nurse Home unit debugging is handled via UART bus connected to the ESP8266 NodeMCU devboard USB port.

1. When plugged in to your computer connect to the board Serial port using any serial monitor software (such as HTerm or TeraTerm or the Arduino IDE Serial Monitor).

![Image of Serial Monitor Serial port](https://i.ibb.co/wJyc1jW/serialport-hterm.png)


2. Select Baud rate 115200 and new line on new line (\n or LF) 

![Image of baud rate](https://i.ibb.co/dtS7ngd/hterm-baud.png)

![Image of newline](https://i.ibb.co/VgR9Bms/newline.png)


3. Connect to your board

![Image of Connect](https://i.ibb.co/BBL4D6z/connect.png)

![Image of Connected](https://i.ibb.co/6mWPY6c/connected.png)


The AP-Nurse Home unit periodically updates sensor data and prints a **status code**. Individual status codes with their respective meaning are listed in the **ap_alert** enumeration in **AP_Nurse_types.h**

```C++
/** AP Nurse Alert type
 * 
 *  @note each alert uses a nth bit of the status code allowing multiple simultaneous alert triggers
 */
#ifndef ap_alert_type
#define ap_alert_type
typedef enum ap_alert{
    STATUS_OK = 0,      
    MOTION_ALERT = 1,   //0b0000 0001
    NOISE_ALERT = 2,    //0b0000 0010
    SMOKE_ALERT = 4,    //0b0000 0100
    GAS_ALERT = 8,      //0b0000 1000
    LIGHT_ALERT = 16,   //0b0001 0000
    PRESSURE_ALERT = 32,//0b0010 0000
    GENERAL_ALERT = -1000,
    I2C_NO_DATA = -1001
}status_t;
#endif
```
