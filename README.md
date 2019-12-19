# WifiTimeProviderESP8266-MQTT
A fork of the open source WifiTimeProviderESP8266 from nixieclock.biz to provide MQTT capability

IMPORTANT NOTE: WifiTimeProviderESP8266-MQTT assumes a familiarity with MQTT and, in particular, Mosquitto. I am running a Mosquitto broker on a Raspberry Pi Zerro W. Line 26 of PubSubClient.h:

#define MQTT_MAX_PACKET_SIZE 128 

must be changed to at least 384. PubSubClient uses this number as a constant buffer size of chars to publish strings on MQTT regardless of actual string size. If you try to publish a string longer than this max size, it will simply not publish anything. Mine is set at 512 with no evident ill effects. YMMV

WifiTimeProviderESP8266 is code used by nixie clock board kits sold by nixieclock.biz. This fork adds MQTT capability to the board, enabling it to publish/subscribe to an MQTT system using the PubSubClient library for Arduino. All changes were made using code additions only - no modifications or deletions were made to any original code lines. Previous changes made to WifiTimeProviderESP8266 not involving MQTT are included here and are discussed more fully in the nixie clock support forum: https://www.tubeclockdb.com/forum/12-arduino-nixie-clock-kit-support-forum/7690-ntp-based-wifi-time-for-v1-clocks

I replaced my board's original ESP8266 with a Wemos D1 mini. This required modification of the I2C initialization line:

//  Wire.begin(0, 2);   // SDA = 0, SCL = 2   // <---- uncomment this line if using ESP8266 - not MQTT mod
    Wire.begin(D2, D1);                       // <---- comment this line if not using Wemos D1 mini 

This code was compiled and tested with Arduino 1.8.10 with all relevant libraries updated as of 17 December 2019. It compiles and runs in an ESP8266 but has not been tested.

Since the original WifiTimeProviderESP8266 code is essentially untouched, the webserver user interface is still operational and, in some cases, preferrable over the MQTT pub/sub system. So, why add MQTT capability?

1.  Under MQTT, it is not necessary to know the nixie clock's IP address to access its user interface. The Wemos is subscribed to a specific topic and listens for its call sign - in this case "Nixie Clock".
    
2.  A web browser is not necessary which greatly simplifies programmatic access to the user interface. For example, PIR/RCWL motion sensors can be easily set up with different microntrollers away from the nixie clock to trigger action without having to make complex changes to the original WifiTimeProviderESP8266 code. The webserver's access security protocol is bypassed. The MQTT broker will, of course, have its own security protocol. This easy extensibility can be used by other sensors/interfaces to control the nixie clock (buttons, remotes, etc).
    
3.  Lastly, the nixie clock can become an upstanding member of your existing MQTT system. 
    
The advantage of using the webserver interface over MQTT is a nicely formatted big picture info at a glance about the nixie clock system. 

WifiTimeProviderESP8266-MQTT implemented here only covers Clock Modes 0 (Never blank) to 4 (Blank during selected hours everyday). Where applicable, specifying start and end hours for blanking can be specified. Extending MQTT's application to all the remaining clock modes should be trivial. RGB led settings are also covered in this implementation. All other interface tweaks are specialty items, set infrequently so that using the webserver interface makes more sense. 

The nixie clock's current IP address (among other things) can be easilty obtained by publishing in the "Command" topic the phrase "Nixie Clock Status". The Wemos would then reply by publishing in the "Information" topic a set of telemetry items. For another example, the start hour of blanking can be changed by publishing "Nixie Clock Mode 4 Off=0" in the "Command" topic. This will blank the tubes (and/or LEDs, as the case may be) everyday at midnight. End hour of blanking, if not specifically set, will default to the previous setting. Simply publishing "Nixie Clock Mode 4" will blank the display at midnight and turn the display back on at 08:00 everyday. In all cases, the Wemos will respond after receiving a command by publishing the relevant changes made on the "Information" topic. The MQTT in this code implements only two topics: "Command" and "Information". 

All "Command" phrases must begin with "Nixie Clock ". Note that there is a single space after "Clock " which is then followed by the appropriate command. There is a single space after "Mode " followed by the mode number. There is a single space after mode number if either "On=" or "Off=" follows. Do not add spaces before or after "=" sign. The other commands are self-explanatory.

Valid "Command" vocabulary: Mode {0-4} [On=|Off=|{0-23}]|[Red=|Blue=|Green=|{0-15}]|Status

Changes to WifiTimeProviderESP8266 are marked by 

/**** START MQTT ****/ 

Additions here

/**** END MQTT ****/
