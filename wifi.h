

#include "WiFi.h"

void theCoolWifiInitiation() { 
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}



void theCodeToCheckAmperage() { 
  // WiFi.scanNetworks will return the number of networks found
  //noInterrupts();
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(1000);
  interrupts();
  }


//wifi mode is entered by longpress
//it may draw more current because
//uploading is a powerful microwave operation.
//that is if you want to download your samples

//however in low power it can be used to switch 
//a bank of saved presets, 
//or save the current loop to the fifo preset bank,
//size calculated from available flash / 200k
//the loop sram is 128k * 3/2 for 12bit depth
//these are downloaded as raw files that are padded out to 16bit

//website with basic html table of five to ten links to 
//randomnly named files (applelizard, feltrock, etc)
//renaming posisble with simple post button. 
//uploading possible with post button tzoo.

//wifi station becomes client 
//initially named cafe_right, cafe_left, or cafe_mono
//that is a .local site
//can be renamed 
