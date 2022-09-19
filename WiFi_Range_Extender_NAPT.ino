/* 
  Team  : CyberX_kh
  Data  : 14-Sep-2022
*/

#if LWIP_FEATURES && !LWIP_IPV6

#define HAVE_NETDUMP 0

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

// Set BSSID variable
uint8_t macAddr[6];

#define AP_CHANNEL 6
#define AP_HIDDEN true
// defult is 4 maximum number of clients(MAX 8)
#define AP_MAX_CON 4

#include <ESP8266WiFi.h>
#include <lwip/napt.h>
#include <lwip/dns.h>
#include <LwipDhcpServer.h>

#define NAPT 1000
#define NAPT_PORT 10

#if HAVE_NETDUMP

#include <NetDump.h>

void dump(int netif_idx, const char* data, size_t len, int out, int success) {
  (void)success;
  Serial.print(out ? F("out ") : F(" in "));
  Serial.printf("%d ", netif_idx);

  // optional filter example: if (netDump_is_ARP(data))
  {
    netDump(Serial, data, len);
    //netDumpHex(Serial, data, len);
  }
}
#endif
  void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.printf("\n\nNAPT Range extender\n");
    Serial.printf("Heap on start: %d\n", ESP.getFreeHeap());

    #if HAVE_NETDUMP
      phy_capture = dump;
    #endif

    // first, connect to STA so we can get a proper local DNS server
    WiFi.mode(WIFI_STA);
    WiConnect();
    // give DNS servers to AP side
    dhcpSoftAP.dhcps_set_dns(0, WiFi.dnsIP(0));
    dhcpSoftAP.dhcps_set_dns(1, WiFi.dnsIP(1));
    WiFi.softAPConfig(  // enable AP, with android-compatible google domain
                      IPAddress(172, 217, 28, 254),
                      IPAddress(172, 217, 28, 254),
                      IPAddress(255, 255, 255, 0));
    // Begin Access Point
    //WiFi.softAP(AP_SSID, AP_PASS);
    WiFi.softAP(AP_SSID, AP_PASS, AP_CHANNEL, AP_HIDDEN, AP_MAX_CON);
    WiFi.softAPmacAddress(macAddr);
    Serial.printf("\nAP: %s : %s\n",
                  AP_SSID,
                  WiFi.softAPIP().toString().c_str());
    Serial.printf("MAC address = %02x:%02x:%02x:%02x:%02x:%02x\n",
                  macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
                  
    Serial.printf("Heap before: %d\n", ESP.getFreeHeap());
    err_t ret = ip_napt_init(NAPT, NAPT_PORT);
    Serial.printf("ip_napt_init(%d,%d): ret=%d (OK=%d)\n", NAPT, NAPT_PORT, (int)ret, (int)ERR_OK);
    if (ret == ERR_OK) {
      ret = ip_napt_enable_no(SOFTAP_IF, 1);
      Serial.printf("ip_napt_enable_no(SOFTAP_IF): ret=%d (OK=%d)\n", (int)ret, (int)ERR_OK);
      if (ret == ERR_OK) {
        Serial.printf("WiFi Network %s with password : %s is now NATed behind %s\n", WIFI_SSID WIFI_PASS, WIFI_SSID);
      }
    }
    Serial.printf("Heap after napt init: %d\n", ESP.getFreeHeap());
    if (ret != ERR_OK) {
      Serial.printf("NAPT initialization failed\n");
    }
  }
#else
  void setup() {
    Serial.begin(115200);
    Serial.printf("\n\nNAPT not supported in this configuration\n");
  }
#endif

void loop() {
  while(WiFi.status() == WL_DISCONNECTED){
    Serial.println("WiFi Disconnected...");
    WiConnect();
  }
}

void WiConnect(){
  WiFi.begin(WIFI_SSID, WIFI_PASS);
    // Checking WiFi status
    if(WiFi.status() == WL_NO_SSID_AVAIL){
      Serial.println("Check ESSID ...");
    }else{
      if(WiFi.status() == WL_WRONG_PASSWORD){
        Serial.println("Check WiFi Pass ...");
      }else{
        while (WiFi.status() != WL_CONNECTED){
          delay(100);
          Serial.print(".");
        }
        // Connected to WiFi
        Serial.println();
        Serial.println("CONNECTED!");
        Serial.printf("\nSTA: %s : %s (dns: %s / %s)\n",
                      WIFI_SSID,
                      WiFi.localIP().toString().c_str(),
                      WiFi.dnsIP(0).toString().c_str(),
                      WiFi.dnsIP(1).toString().c_str());
      }
    }
}
