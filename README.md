# AP-Home

Custom FW of the AP-Nurse Home node based on the ESP8266 Arduino core.

## Files

* **AP-Home.ino** - main app - Arduino core project
* **AP_Nurse_Home_pinout.h** - board pinout
* **AP_Nurse.h** - AP-Home controll interface header
* **AP_Nurse_types.h** - important type, structure, macro and directive definitions
* **AP_Nurse.cpp** - AP-Home controll interface implementation
* **ClickButton.h** - Button handler library header
* **ClickButton.cpp** - Button handler library implementation
* **PCF8591.h** - I2C analog extender driver header
* **PCF8591.cpp** - I2C analog extender driver implementation
* **LICENCE** - GNU General Public License

## Programming

## IDE setup

To programme, build and flash the app u'll need **Arduino IDE v 1.8.9** with the **ESP8266 package**

* https://www.arduino.cc/download_handler.php?f=/arduino-1.8.9-windows.exe (Windows)
* https://www.arduino.cc/download_handler.php?f=/arduino-1.8.9-linux64.tar.xz (Linux)

ESP8266 package installation tutorial:

https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/

Ideally u can set up Arduino core projet support within **Visual Studio Code** or **Eclipse IDE** (unnecessary, but highly recommended)

* https://maker.pro/arduino/tutorial/how-to-use-visual-studio-code-for-arduino (VSCode tutorial)
* https://www.eclipse.org/community/eclipse_newsletter/2017/april/article4.php (Eclipse tutorial)

## Git BASH

For convenient and fast code update and sharing using the Git supply chain manager GUI **Git BASH** is recommended.

* https://git-scm.com/download/win (Windows)
* https://git-scm.com/download/linux (Linux)

### Handling a git repo

#### Cloning a repo

Git GUI is handled via cmd in Windows and via a terminal in Linux
(

**1.** Create a directory for the project and navigate to your new directory

```
C:Users\[yourUsername]> md MySuperDuperProject

C:Users\[yourUsername]> cd MySuperDuperProject

C:Users\[yourUsername]\MySuperDuperProject>
```

**2.** Clone the git repo

By **cloning** a git repository u locally cache its content to your machine creating a local repository that **tracks** the remote repository u cloned it from.
This allows you to make changes locally and then **push** them online to the remote repo or **pull** any changes from the remote repo to your local one using a single command instead of downloading unziping, copy-pasting and eventually getting lost :D .

```
C:Users\[yourUsername]\MySuperDuperProject> git clone https://github.com/vtothsvk/AP-Home.git
Cloning into 'AP-Home'...

remote: Enumerating objects: 217, done.
remote: Counting objects: 100% (217/217), done.
remote: Compressing objects: 100% (191/191), done.
remote: Total 217 (delta 129), reused 54 (delta 25), pack-reused 0R
Receiving objects: 100% (217/217), 58.08 KiB | 914.00 KiB/s, done.
Resolving deltas: 100% (129/129), done.

C:Users\[yourUsername]\MySuperDuperProject>
```

**3.**  Open your cloned repo and make yourself at home

```
C:Users\[yourUsername]\MySuperDuperProject> cd AP-Home

C:Users\[yourUsername]\MySuperDuperProject\AP-Home> 
```

**Voilà all set and done ^^**

Now u can start managing the git repo

#### Updating your local repo

A new version of the source code is online and dont want to go to all the hassle of manually updating all the relevant files?
Behold the **pull** command

```
C:Users\[yourUsername]\MySuperDuperProject\AP-Home> git pull
```

## Programming

To programme the app open the Arduino core project **AP-Home.ino**

Sensor threshold configuration is handled by the **_TH** directives in **AP-Home.ino**:

```C++
/** Threshold directives
 *
 *  @note each level represents one 255th of the reference voltage (eg. a threshold of 100 @ 3.3V represents 1.29V)
 */ 
#define NOISE_TH        255
#define SMOKE_TH        255
#define GAS_TH          60
#define LIGHT_TH        80
#define PRESSURE_TH     80
#define TEMP_TH         255
```

## Building

**1.** The ESP8266 mcu has to be set as the active build **target**:

![Image of Building](https://i.ibb.co/dKZLKC0/Arduino-target.png)

**2.** The NodeMCU devboard **serial port** has to be selected:

![Image of Serial Port](https://i.ibb.co/NKj9jyp/serial-Port-duino.png)

*your board COM port may vary

### Building (compiling and linking into a .elf image):

* In Arduino IDE: **CTRL+R**
* In VSCode: **CTRL+ALT+R**
* In Eclipse: **CTRL+B**

### Flashing:

* In Arduino IDE: **CTRL+U**;
* In VSCode: **CTRL+ALT+U**
* In Eclipse: **CTRL+F11**

## Debugging

AP-Nurse Home unit debugging is handled via a UART bus connected to the ESP8266 NodeMCU devboard USB port.

**1.** When plugged in to your computer, connect to the board **Serial port** using a **serial monitor terminal** of your choice (eg. HTerm, TeraTerm or the Arduino IDE Serial Monitor).

![Image of Serial Monitor Serial port](https://i.ibb.co/wJyc1jW/serialport-hterm.png)

**2.** Select a **115200** bit/s **baud rate** and **new line** at **new line character**(\n or LF) 

![Image of baud rate](https://i.ibb.co/dtS7ngd/hterm-baud.png)

![Image of newline](https://i.ibb.co/VgR9Bms/newline.png)

**3.** **Connect** to your board

![Image of Connect](https://i.ibb.co/BBL4D6z/connect.png)

![Image of Connected](https://i.ibb.co/6mWPY6c/connected.png)


The AP-Nurse Home unit periodically updates sensor data and prints out a **status code**. Individual status codes with their respective meaning are listed in the **ap_alert** enumeration in **AP_Nurse_types.h**

```C++
/** AP Nurse Alert type
 * 
 *  @note each alert uses a dedicated bit of the status code allowing multiple simultanious alert triggers
 */
#ifndef ap_alert_type
#define ap_alert_type
typedef enum ap_alert{
    STATUS_OK = 0,      
    MOTION_ALERT = 1,       //0b00000001
    NOISE_ALERT = 2,        //0b00000010
    SMOKE_ALERT = 4,        //0b00000100
    GAS_ALERT = 8,          //0b00001000
    LIGHT_ALERT = 16,       //0b00010000
    PRESSURE_ALERT = 32,    //0b00100000
    GENERAL_ALERT = -1000,
    I2C_NO_DATA = -1001
}status_t;
#endif
```
