# WifiTimeProviderESP8266-MQTT
A fork of the open source WifiTimeProviderESP8266 from nixieclock.biz to provide MQTT capability

![Fig. 1](https://user-images.githubusercontent.com/10426316/71139162-3d0f3400-21c2-11ea-9c2f-b54d4ca77836.png)

__IMPORTANT NOTE:__ WifiTimeProviderESP8266-MQTT assumes a familiarity with WifiTimeProviderESP8266 and what it does. Knowledge of MQTT is also assumed, in particular, the Mosquitto broker. I am running a Mosquitto broker on a Raspberry Pi Zerro W. PubSubClient library for Arduino is used for this implementation, but Line 26 of PubSubClient.h:

    #define MQTT_MAX_PACKET_SIZE 128 

must be changed to at least 384. PubSubClient uses this number as a constant buffer size of chars to publish strings on MQTT regardless of actual string size. If you try to publish a string longer than this max size, it will simply not publish anything. Mine is set at 512 with no obvious ill effects. YMMV

# The Nixie Clock MQTT

WifiTimeProviderESP8266 is code used by the ESP8266 add-on to the nixie clock board kits sold by nixieclock.biz. This fork adds MQTT capability to the board, enabling it to publish/subscribe to an MQTT system using the PubSubClient library for Arduino. This is a non-destructive mod: all changes are made by simply adding new lines of code - other than that, no alterations or deletions are made to the original. I have a six digit All-In-One Rev3 model that came pre-installed with WifiTimeProviderESP8266 v1 protocol v356. However, after following this [NTP support](https://www.tubeclockdb.com/forum/12-arduino-nixie-clock-kit-support-forum/7690-ntp-based-wifi-time-for-v1-clocks) discussion in the nixie clock support forum, I decided to switch to WifiTimeProviderESP8266 v2 protocol v357mjs.9. This business of Rev/Vers/Protocol numbering is quite labyrinthine and very, very confusing, to say the least. Bottom line is that the WifiTimeProviderESP8266 code framework this MQTT mod is based on is non-standard. My suggestion is to simply take all MQTT additions, explained more fully below, and just graft them onto your working version of WifiTimeProviderESP8266 in the appropriate locations. There's a great chance that it will actually work. If not, simply take them out again.

Also, I replaced my board's original ESP8266 with a Wemos D1 mini. This required modification of the I2C initialization line:

    // Wire.begin(0, 2);    // SDA = 0, SCL = 2  <-- uncomment start of this line if using ESP8266
       Wire.begin(D2, D1);  // <-- comment start of this line if not using Wemos D1 mini
   
This code was compiled and tested using Arduino IDE 1.8.10 with all relevant libraries updated as of 17 December 2019. It compiles and runs in an ESP8266 but has not been tested. 

__Be sure to replace the MQTT credentials section with your actual setup.__ 

Since the original WifiTimeProviderESP8266 code is essentially untouched, the webserver user interface is still operational and, in some cases, preferrable over the MQTT pub/sub system. So, why add MQTT capability?

1.  Under MQTT, it is not necessary to know the nixie clock's IP address to access its user interface. The Wemos is subscribed to a specific topic and listens for its call sign - in this case "Nixie Clock".
    
2.  A web browser is not necessary which greatly simplifies programmatic access to the user interface. For example, PIR/RCWL motion sensors can be set up easily using different microntrollers away from the nixie clock to trigger some action without having to make complex changes to the original WifiTimeProviderESP8266 code. The webserver's access security protocol is bypassed completely. The MQTT broker will, of course, have its own security protocol. This easy extensibility can be used by other sensors/interfaces to control the nixie clock (buttons, remotes, etc).
    
3.  Lastly, the nixie clock can become an upstanding member of your existing MQTT system. 
    
The advantage of using the webserver interface over MQTT is a nicely formatted big picture info at a glance about the nixie clock system, not to mention the other clock features not covered by this mod.   

WifiTimeProviderESP8266-MQTT implemented here only covers Clock Modes 0 (Never blank) to 4 (Blank during selected hours everyday). Where applicable, specifying start and end hours for blanking can be specified. Extending MQTT's application to all the remaining clock modes should be trivial. RGB led settings are also covered in this implementation. All other interface tweaks are considered specialty items, set infrequently so that using the webserver interface makes more sense. 

The nixie clock's current IP address (among other things) can be easilty obtained by publishing in the "Command" topic:

    "Nixie Clock Status"
    
The Wemos would then reply by publishing in the "Information" topic a set of telemetry data (see pic above). For another example, the start hour of blanking can be changed by publishing 

    "Nixie Clock Mode 4 Off=0"

in the "Command" topic. This will blank the tubes (and/or LEDs, as the case may be) everyday at midnight. End hour of blanking, if not specifically set, will default to the previous setting. Simply publishing 

    "Nixie Clock Mode 4"
    
will blank the display at midnight and turn the display back on at 08:00 everyday. In all cases, the Wemos will respond after receiving a command by publishing the relevant changes made on the "Information" topic. The MQTT in this code implements only two topics: "Command" and "Information". 

All "Command" phrases must begin with "Nixie Clock ". Note that there is a single space after "Clock " which is then followed by the appropriate command. There is a single space after "Mode " followed by the mode number. There is a single space after mode number if either "On=" or "Off=" follows. Do not add spaces before or after "=" sign. The other commands are self-explanatory.

Valid "Command" vocabulary: Mode {0-4} [On=|Off=|{0-23}]|[Red=|Blue=|Green=|{0-15}]|Status

Changes to WifiTimeProviderESP8266 are marked by 

/**** START MQTT ****/ 

Additions here

/**** END MQTT ****/
