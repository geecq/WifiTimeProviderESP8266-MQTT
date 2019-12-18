# WifiTimeProviderESP8266-MQTT
A fork of the open source WifiTimeProviderESP8266 from nixieclock.biz to provide MQTT capability

WifiTimeProviderESP8266 is code used by nixie clock board kits sold by nixieclock.biz. This fork adds MQTT capability to the board, enabling it to publish/subscribe to an MQTT system using the PubSubClient library for Arduino. All changes were made using code *additions* only -- no modifications were made to actual original code lines. Previous changes made to WifiTimeProviderESP8266 not involving MQTT are included here and are discussed more fully in the nixie clock support forum: https://www.tubeclockdb.com/forum/12-arduino-nixie-clock-kit-support-forum/7690-ntp-based-wifi-time-for-v1-clocks

Also, I replaced my nixie clock board ESP8266 with a Wemos D1 mini. This code was compiled and tested with Arduino 1.8.10 and all relevant libraries updated as of 17 December 2019. It also compiles and runs in an ESP8266 but has not been tested.

Since the original WifiTimeProviderESP8266 code is essentially untouched, the webserver user interface is still operational and, in some cases, preferrable over the MQTT pub/sub system. So why add MQTT capability?

1.  Under MQTT, it is not necessary to know the nixie clock's IP address to access its user interface. The Wemos is subscribed to a specific topic and listens for its call sign - in this case "Nixie Clock".
    
2.  A web browser is not necessary and, therefore, simplifies programmatic access to the user interface. For example, PIR/RCWL motion sensors can be easily set up with different microntrollers away from the nixie clock to trigger action without having to go through the webserver's security. The MQTT broker will, of course, have its own security protocol. This easy extensibility can be used by other sensors/interfaces to control the nixie clock (buttons, remotes, etc).
    
3.  Lastly, the nixie clock can be part of your existing MQTT system. 
    
The advantage of using the webserver interface over MQTT is a nicely formatted big picture info at a glance about the nixie clock system. 

WifiTimeProviderESP8266-MQTT implemented here only covers Clock Modes 0 (Never Blank) to 4 (Blank during selected hours everyday). Where applicable, specifying start and end hours for blanking can be specified. Extending MQTT's application to all the remaining clock modes should be trivial. RGB led settings are also covered in this implementation. All other interface tweaks are specialty items, set infrequently so that using the webserver interface makes more sense. 

The nixie clock's current IP address (among other things) can be easilty obtained by publishing in the "Command" topic the phrase "Nixie Clock Status" through the MQTT system. The Wemos would then reply by publishing in the "Information" topic a set of telemetry items. For another example, the start hour of blanking can be changed by publishing "Nixie Clock Mode 4 Off=0" in the "Command" topic. This will blank the tubes (and LEDs, as the case may be) everyday at midnight. End hour of blanking, if not specifically set, will default to the previous setting. Simply publishing "Nixie Clock Mode 4" will blank the display at midnight and turn the display back on at 08:00 everyday. In both cases, the Wemos will respond by publishing the relevant changes on the "Information" topic. The MQTT in this code implements only those two topics: "Command" and "Information". 

All "Command" phrases must begin with "Nixie Clock ". Note that there is a space after "Clock " which is then followed by the appropriate command. There is a single space after "Mode " and then a single space after mode number if either "On=" or "Off=" follows. Do not add spaces before or after "=" sign. 

Valid "Command" vocabulary: Mode {0-4} [On=|Off=|{0-23}]|[Red=|Blue=|Green=|{0-15}]|Status

Changes to WifiTimeProviderESP8266 are marked with START MQTT and ends with END MQTT. 
