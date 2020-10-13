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

Sensor threshold configuration is handled by the **_TH** directives:

```C++
#define NOISE_TH        255
#define SMOKE_TH        255
#define GAS_TH          60
#define LIGHT_TH        80
#define PRESSURE_TH     80
#define TEMP_TH         255
```

## Building

To build the app, first the ESP8266 mcu has to be set as the active build target:

![Image of Building](https://i.ibb.co/dKZLKC0/Arduino-target.png)

Building (Compiling and linking into a .elf image):

* In Arduino IDE: **CTRL+R**
* In VSCode: **CTRL+ALT+R**
* In Eclipse: **CTRL+B**

Flashing:

* In Arduino IDE: **CTRL+U**
* In VSCode: **CTRL+ALT+U**
* In Eclipse: **CTRL+B**

## Debugging
