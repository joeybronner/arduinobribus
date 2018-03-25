
 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>

#define SSD1306_128_64

#define OLED_MOSI   D7 //Connect to D1 on OLED
#define OLED_CLK    D5 //Connect to D0 on OLED 
#define OLED_DC     D1 //Connect to DC on OLED
#define OLED_CS     D8 //Connect to CS on OLED
#define OLED_RESET  D3 //Connect to RES on OLED

// WiFi
const char* KNOWN_SSID[] = {"SAP-Guest", "SFR-97f0"};
const char* KNOWN_PASSWORD[] = {"", "Q54CH9NNKU4A"};
const int   KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]);
HTTPClient http;

// Timer
BlynkTimer timer;

// Bus data
String sMissionId = "100100099";
String sStationId = "PC3_1047_1074";
String sWay = "a";

String sDirection = "Champerret";
String sBusSHortName = "PC3";

String busAfirst = "00";
String busAsecon = "00";
String busBfirst ="00";
String busBsecon = "00";

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup()   {                
  Serial.begin(115200);
  delay(200);
  Serial.println("");
  Serial.println("Wemos Started");
  
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(1000);
  display.clearDisplay();
  //display.setFont(&TomThumb);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setFont(NULL);

  // WiFi before launch
  initWiFiConnection();

  // Setup a function to be called every second
  timer.setInterval(30000L, checkConnection);
  timer.setInterval(20000L, ratpQuery);
}

void loop()
{
  timer.run();

  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println(sBusSHortName);
  display.setTextSize(1);
  display.println("Direction " + sDirection);
  display.display();
  delay(3000);
  
  display.clearDisplay();
  
  // Next 
  display.setCursor(0,0);
  display.setTextSize(2);
  display.print(busAfirst);
  display.setCursor(30,5);
  display.setTextSize(1);
  display.print("minutes");

  // Next +1
  display.setCursor(0,18);
  display.setTextSize(2);
  display.print(busAsecon);
  display.setCursor(30,22);
  display.setTextSize(1);
  display.print("minutes");
  
  display.display();
  delay(3000);
}

void ratpQuery() {
  http.begin("http://restratpws.azurewebsites.net/api/missions/" + sMissionId + "/from/" + sStationId + "/way/" + sWay);
  int httpCode = http.GET();

  if (httpCode > 0) { //Check the returning code
    String payload = http.getString();
    Serial.println(payload);

    const int numberOfBus = 2;
    String bus[numberOfBus];
    int counter = 0;
    int lastIndex = 0;

    for (int i = 0; i < payload.length(); i++) {
        // Loop through each character and check if it's a comma
        if (payload.substring(i, i+1) == ",") {
          // Grab the piece from the last index up to the current position and store it
          bus[counter] = payload.substring(lastIndex, i);
          // Update the last position and add 1, so it starts from the next character
          lastIndex = i + 1;
          // Increase the position in the array that we store into
          counter++;
        }

        // If we're at the end of the string (no more commas to stop us)
        if (i == payload.length() - 1) {
          // Grab the last part of the string from the lastIndex to the end
          bus[counter] = payload.substring(lastIndex, i);
        }
    }

    // Extr
    String f = bus[0];
    busAfirst = "";
    for (int i = 0; i < f.length(); i++) {
        if (f.substring(i, i+1) == "0" || f.substring(i, i+1) == "1" || f.substring(i, i+1) == "2" || f.substring(i, i+1) == "3" || f.substring(i, i+1) == "4" || f.substring(i, i+1) == "5" || f.substring(i, i+1) == "6" || f.substring(i, i+1) == "7" || f.substring(i, i+1) == "8" || f.substring(i, i+1) == "9") {
          busAfirst = busAfirst + f.substring(i, i+1);
        }
    }
    if (busAfirst == "") {
      busAfirst = "00";
    }

    String s = bus[1];
    busAsecon = "";
    for (int i = 0; i < s.length(); i++) {
        if (s.substring(i, i+1) == "0" || s.substring(i, i+1) == "1" || s.substring(i, i+1) == "2" || s.substring(i, i+1) == "3" || s.substring(i, i+1) == "4" || s.substring(i, i+1) == "5" || s.substring(i, i+1) == "6" || s.substring(i, i+1) == "7" || s.substring(i, i+1) == "8" || s.substring(i, i+1) == "9") {
          busAsecon = busAsecon + s.substring(i, i+1);
        }
    }
    if (busAsecon == "") {
      busAsecon = "00";
    } 

    Serial.println(busAfirst);
    Serial.println(busAsecon);

  }

  http.end();
}

void checkConnection() {
  int timeout = millis();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println('Connected');
  } else {
    digitalWrite(BUILTIN_LED, 0);
    initWiFiConnection();
  }
}

void initWiFiConnection () {
  digitalWrite(BUILTIN_LED, 0);
  boolean wifiFound = false;
  int i, n;

  // ----------------------------------------------------------------
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  // ----------------------------------------------------------------
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");

  // ----------------------------------------------------------------
  // WiFi.scanNetworks will return the number of networks found
  // ----------------------------------------------------------------
  Serial.println(F("scan start"));
  int nbVisibleNetworks = WiFi.scanNetworks();
  Serial.println(F("scan done"));
  if (nbVisibleNetworks == 0) {
    Serial.println(F("no networks found. Reset to try again"));
    while (true); // no need to go further, hang in there, will auto launch the Soft WDT reset
  }

  // ----------------------------------------------------------------
  // if you arrive here at least some networks are visible
  // ----------------------------------------------------------------
  Serial.print(nbVisibleNetworks);
  Serial.println(" network(s) found");

  // ----------------------------------------------------------------
  // check if we recognize one by comparing the visible networks
  // one by one with our list of known networks
  // ----------------------------------------------------------------
  for (i = 0; i < nbVisibleNetworks; ++i) {
    Serial.println(WiFi.SSID(i)); // Print current SSID
    for (n = 0; n < KNOWN_SSID_COUNT; n++) { // walk through the list of known SSID and check for a match
      if (strcmp(KNOWN_SSID[n], WiFi.SSID(i).c_str())) {
        Serial.print(F("\tNot matching "));
        Serial.println(KNOWN_SSID[n]);
      } else { // we got a match
        wifiFound = true;
        break; // n is the network index we found
      }
    } // end for each known wifi SSID
    if (wifiFound) break; // break from the "for each visible network" loop
  } // end for each visible network

  if (!wifiFound) {
    Serial.println(F("no Known network identified. Reset to try again"));
    while (true); // no need to go further, hang in there, will auto launch the Soft WDT reset
  }

  // ----------------------------------------------------------------
  // if you arrive here you found 1 known SSID
  // ----------------------------------------------------------------
  Serial.print(F("\nConnecting to "));
  Serial.println(KNOWN_SSID[n]);

  // ----------------------------------------------------------------
  // We try to connect to the WiFi network we found
  // ----------------------------------------------------------------
  WiFi.begin(KNOWN_SSID[n], KNOWN_PASSWORD[n]);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  // ----------------------------------------------------------------
  // SUCCESS, you are connected to the known WiFi network
  // ----------------------------------------------------------------
  digitalWrite(BUILTIN_LED, 1);
  delay(150);
  Serial.println(F("WiFi connected, your IP address is "));
  Serial.println(WiFi.localIP());
}
