# Esp8266 Wifi Range Extender
![61GwRCoPxlL _AC_SL1035_-1](https://user-images.githubusercontent.com/104082922/191763205-425980bf-9bb9-4c5f-8783-fd364690a3d2.png)
This project was created for easy to make a wifi range extender for any project, and the example of printing some Station and AP information on the Serial Monitor

It easy setting, just setup your WIFI_SSID & WIFI_PASS for Station mode
and AP_SSID & AP_PASS for Access Point mode 

Find the example in the code (Line 6 -- 16)

// Set WiFi credentials
#ifndef WIFI_SSID
  #define WIFI_SSID  "your wifi name"
  #define WIFI_PASS  "your wifi pass"
#endif

// Set Ap credentials
#ifndef AP_SSID
  #define AP_SSID "AP wifi name"
  #define AP_PASS "AP wifi pass"
#endif

Hope you enjoy the Project
