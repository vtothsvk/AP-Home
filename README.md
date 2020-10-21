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

## Handling a git repo

### Cloning a repo

Git GUI is handled via cmd in Windows and via a terminal in Linux
(

**1.** Create a directory for the project and navigate to your new directory

In Windows cmd:

```
C:Users\[yourUsername]> md MySuperDuperProject

C:Users\[yourUsername]> cd MySuperDuperProject

C:Users\[yourUsername]\MySuperDuperProject>
```

**2.** Clone the git repo

By **cloning** a git repository u locally cache its content to your machine creating a local repository that **tracks** the remote repository u cloned it from.
This allows you to make changes locally and then **push** them online to the remote repo or **pull** any changes from the remote repo to your local one using a single command instead of downloading unziping, copy-pasting and eventually getting lost :D .

Cloning is done by the git clone command:

```
*inside the folder u wanna clone the repo to
git clone [yourRemoteRepoLink]
```

eg.

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

### Updating your local repo

A new version of the source code is online and dont want to go to all the hassle of manually updating all the relevant files?
Behold the **pull** command

```
*inside your cloned local repo
git pull
```
eg.

```
C:\Users\epic_\mySuperDuperProject\AP-Home> git pull

remote: Enumerating objects: 9, done.
remote: Counting objects: 100% (9/9), done.
remote: Compressing objects: 100% (5/5), done.
remote: Total 5 (delta 3), reused 0 (delta 0), pack-reused 0
Unpacking objects: 100% (5/5), 1.27 KiB | 72.00 KiB/s, done.
From https://github.com/vtothsvk/AP-Home
   4b606a7..e1124ed  main       -> origin/main
   8fef733..9a07a15  pairTest   -> origin/pairTest
Updating 4b606a7..e1124ed
Fast-forward
 README.md | 64 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 1 file changed, 64 insertions(+)
 
C:\Users\epic_\mySuperDuperProject\AP-Home>
```

**Done! your local repo is up to date ^^** 

### Branching

Wanna change the code but fear screwing up the programme? Git lets u create a **branch** of your code that u can edit independently of the
original source code.

### Creating a branch

```
*inside a git repo
git branch [yourBranchName]
```

### "Switching" branches

Now that u have created a branch u have to "check out" to it (by checking out u basically select the branch u are working on)

```
*inside a git repo
git checkout [yourBranchName]
```

### Listing all branches

Not sure about the branch u are working on or wanna check out but forgot the branch name?

```
*inside a git repo
git branch -a
```

The command prints out all available branches and higligths the one u are curenntly check out to.

### Checking out to a local branch of a remote repo

Git clone has a mysterious way of displaying locally cached branches lets see an example.

I cloned the AP-Home repo that has 2 branches "main" and "pairTest". Let's check which branches are chached locally.

```
C:\Users\epic_\mySuperDuperProject\AP-Home>git branch -a

* main
  remotes/origin/HEAD -> origin/main
  remotes/origin/main
  remotes/origin/pairTest
  
C:\Users\epic_\mySuperDuperProject\AP-Home>
```

Hmm, curious, it seems only the "main" branch is in my local repo. Truth is Git caches all the remote branches locally, but
does not display them util I check out to them. So I see the remote repo has a branch "pairTest", that means I should have a
equally named branch in my local repo as well. Let's try it out...

C:\Users\epic_\mySuperDuperProject\AP-Home>git checkout pairTest
Switched to a new branch 'pairTest'
Branch 'pairTest' set up to track remote branch 'pairTest' from 'origin'.

```
C:\Users\epic_\mySuperDuperProject\AP-Home>git branch -a
  main
* pairTest
  remotes/origin/HEAD -> origin/main
  remotes/origin/main
  remotes/origin/pairTest

C:\Users\epic_\mySuperDuperProject\AP-Home>
```

Noice!

### "Saving" progress on your current branch

U can **"save"** your progress to the current branch by the **git commit** command. First tho, u have to **stage**
all of your changes by the **git add** command.

* git add

```
*inside a git repo
git add .
```

This command stages all changes to all files inside a repo

```
*inside a git repo
git commit --all -m "Your commit message"
```

This command commits all staged changes with a **commit message**
A commit message is a way of communicating what changed in the current commit eg. "Fixed incorrect pin initialisation".

### "Uploading" changes

To **"upload"** any local repo changes the **git push** command is used. (All your changes has to be locally commited first!)

```
*inside a locally cahced remote repo
git push
```

This command "pushes" all your locall changes to their tracked remote repo.

## Git repo workflow

**We do not make direct changes to the "main" branch!!!**

In case of developing new feature, bug fux or any other shinanigans we follow the workflow shown below.

![Image of Git Workflow](https://i.ibb.co/KDhzhCn/Git-Workflow.png)

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
