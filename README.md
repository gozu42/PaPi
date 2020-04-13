# ProjectOwlUPRM_Papapi

This project is part of Project Owl. To learn more about this effort, go to [this link](https://github.com/Project-Owl/ClusterDuck-Protocol).

Papapi is an effort to collect local data from the ClusterDuck-Protocol. The intention is to use a Raspberry Pi in order to collect the data incoming from the PapaDuck. This is mainly to see live metrics of the network. The code in this repository contains a modification to the PapaDuck firmware in order for it to work with the Raspberry Pi and the code for a Node.js server that collects and displays the data via the MQTT protocol.

## How it works
Currently, the way it works is by placing a Raspberry Pi as a Wifi access point and making the PapaDuck talk to the Node.js server in the Raspberry Pi via MQTT. This Node.js server then manages the data sent by the Papa Duck which the end goal is to display metrics of said data.

## Things you're going to need

* Raspberry Pi (Model B or up)
* A Micro SD card (4 gigs should do)
* Project Owl Duck (see [this link](https://github.com/Project-Owl/ClusterDuck-Protocol) for more information)
* Ethernet Cable
* USB Keyboard
* USB Mouse
* HDMI Cable
* Monitor with HDMI input

## Setup
NOTE: Currently, this has been tested using Raspbian and specific software. The final production implementation might not use Raspbian or the specific software detailed in this README.

First, setup the Raspberry Pi with Raspbian. Use [this link](https://projects.raspberrypi.org/en/projects/raspberry-pi-setting-up) to follow the tutorial on how to set it up. Very important to note that the Raspberry Pi will be used as an access point and if you do not have an aditional wireless interface, you need to connect the Pi to a wired internet connection because you will not be able to use the Wifi interface of the Raspberry PI to connect to the internet. 

After setting up the Raspberry Pi, go and install raspap-webgui. This is used to turn the Raspberry Pi into a Wifi access point so that the PapaDuck can connect and communicate with the Pi. The [following link](https://github.com/billz/raspap-webgui) shows how to install this service onto your Raspberry Pi. 

Next, we are going to need to install Node.js on the Raspberry Pi. You can follow this [Instructables post](https://www.instructables.com/id/Install-Nodejs-and-Npm-on-Raspberry-Pi/) to install it. 

Afterwards, clone this repository to your Raspberry Pi in whichever directory you want. Then, run the following command on the cloned directory:

```
npm install
```

Finally, run `node index.js` on the aformentioned directory in order to start the MQTT server. This is the server that the PapaDuck will connect to in order to send data to the Pi.

The next thing is to flash the Papapi firmware found in this repository onto the Duck. First, you are going to need to set up your environment to flash this software. This can be found on Project Owl's [Documentation page](https://www.project-owl.com/documentation). We recommend having a working Duck before flashing the Papapi firmware. Once you setup your environment, the next thing is to make sure that the firmware configuration works for your setup.

If you changed the raspap-webgui configuration settings for SSID and/or password, change the SSID and password variables accordingly in the PapaPiDuck.ino file. Next, you might need to assign a different papapiServer address if you changed the default IP address of the Raspberry Pi (e.i 10.3.141.1).

Once you made sure that all of the settings are setup, upload the PapaPiDuck.ino code using the Arduino IDE or the method you used to flash Ducks. If you keep it connected to your computer or you power it on using another method, it will try to connect to the Raspberry Pi via Wifi. Once it connects, it will start sending payloads and you should see the Raspberry Pi terminal that is running the MQTT process show the data being sent from the Duck.





