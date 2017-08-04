# Home-Brewery Automation: PC Front-End
PC-application for my home-brewery

![frontpanel](img/ebrew_screen.png)<br>
*Screen-dump of Program*

# Features
- Connects to the Brew-Arduino over USB or Ethernet
- State Transition Diagram (STD) that fully automates the entire Brewing-Day. Only adding Malt is still a manual operation
- Cleaning-In-Place (CIP) also done automatically
- Many brewing parameters are adjustable and stored in Windows Registry: PID-control, Mashing, Sparging, Boiling and Measurements
- Brewing variables (volume, temperature) are set automatically, but can be overruled manually with so-called Switches and Fixes
- Reading of entire mash-scheme: one file stores all relevant parameters
- Convenient Graphical User Interface (GUI) that shows exactly what is going on in the brewing setup

More software design details: see my website: http://www.vandelogt.nl/uk_software.php

# Software Development Environment
Use with Borland C++ Builder 6. Yes, it is ancient, but I never took the time to update to a newer version (which is also very expensive...)

# Interface with Brew-Arduino
The Arduino-Nano uses a virtual COM port as its main-connection to the PC. This virtual COM port used the USB port of the Arduino-Nano. At the PC side, the Arduino Nano is recognised by Windows
(if the standard drivers for the Arduino-Nano have been installed). Virtual COM port settings are (38400,N,8,1).

Typically the PC-program sends commands to the Arduino-Nano, like **P0** (Pump Off) or **P1** (Pump On). These commands are then executed by the Arduino-Nano.
Although you can type in the commands manually, it is more efficient to use a dedicated PC-program for it, with a nice Graphical User Interface.

More information about the PC-Interface can be found at my website: http://www.vandelogt.nl/uk_hardware.php#PC_INTF



