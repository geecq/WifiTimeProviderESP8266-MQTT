/*
   ESP8266 Webserver for the Arduino Nixie Clock Firmware V1
    - Starts the ESP8266 as an access point and provides a web interface to configure and store WiFi credentials.
    - Allows the time server to be defined and stored

   Program with following settings (status line / IDE):

    Board: Generic ESP8266 Module,
    Crystal Frequency: 26MHz,
    Flash: 80MHz,
    CPU: 160MHz,
    Flash Mode: QIO,
    Upload speed: 115200,
    Flash size: 1M (64k SPIFFS),
    Reset method: ck, Disabled, none
    Erase Flash: All flash contents,
    Builtin LED: 1 (1 for ESP-01)

   Go to http://192.168.4.1 in a web browser connected to this access point to see it
*/

/*
   9Feb19 mjs
   Added ntp and timezone support
   ntp is preferred if available. If a blank ntp server is configured ("none"),
   or the NTP server is unreachable, it will fall back to the configured
   (non-ntp) time server.

   .3 Initial release
   .4 Bug fix and added OTA web browser update ability for 1 MB+ flash devices
      note that a lot of ESP-8266/ESP-01 modules only come with 512K. Be sure to
      set the size correctly in the IDE Tools menu. It looks like Ian sources 1 MB
      ones, so they should work with OTA.
   .5 Added tweaks and basic security for pages which can cause harmful changes
      (web: admin/setup, update: admin/update)
      now syncs clock to NTP within ~25 ms.
      bumped EEPROM size to 1024 for future needs.
   .6 Changed some defaults, more tweaks. Added user/pass hint when in softAP mode.
      Added some bounds checking, support for V1 clocks running i2c protocol v54 (untested).
   .7 Try to sync NTP immediately after changing time server settings. Some refactoring of I2C
      clock reading stuff to try and get V1 compatibility working. Now get a compiler warning
      about "uint8_t available = Wire.requestFrom((int)preferredI2CSlaveAddress, 1);" which is
      counterintuitive, but causes no harm (C++ experts welcome).
   .8 Gave /setvalue a 5 second default timeout. Refactored globals. Better I2C scan detail. Changed
      NTP sync to 5 ms. Added Utility page. 7Feb19
   .9 Show NTP sync status. Revamped status page. Always compile for OTA, enable at run
      time based on sketch size and available space.

*/
