#include <esp_wifi.h>
#include <ESP32_NOW.h>
#include <WiFi.h>
#include <esp_mac.h> 

//ESP-Now Channel
#define ESPNOW_WIFI_CHANNEL 6

// Structure to send data (must match sender structure)
typedef struct struct_message {
  int id;            // Board ID
  int x;             // PIV Status
  int y;             // Heartbeat Signal
  char mac_addr[18]; // Array to hold MAC address string
} struct_message;

//Holds transmitting data
struct_message myData;

// Replace with the slave device MAC address
uint8_t broadcastAddress[] = {0xD0, 0xEF, 0x76, 0x34, 0x82, 0xA0};

//Receiving info
esp_now_peer_info_t peerInfo;

//Displays whether or not the message was successful or not
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  //Setting the baud rate
  Serial.begin(115200);
  
  pinMode(5, OUTPUT); // Indicator LED
  pinMode(23, INPUT); // Switch pin
  
  //Sets the ESP32 as a wifi station and the channel
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  //Prints initial messages
  Serial.println("ESP-NOW Example - Broadcast Master");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  //In case ESP-NOW protocol fails
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //Register send function
  esp_now_register_send_cb(OnDataSent);

  //Copies mac address into peerInfo
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  //Adds peer as a receiver
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  Serial.println("Setup complete. Broadcasting messages every 5 seconds.");
}

void loop() {
  //Need to have a unique ID for each PIV
  myData.id = 1; // Unique ID for this master
  
  // Determine x based on swtich status
  myData.x = digitalRead(23) ? 30 : 20; 

  // Heartbeat signal
  myData.y = 10; 
  
  //Turns on the red LED if the switch is on or off
  if(digitalRead(23)){
    digitalWrite(5, HIGH);
  } else{
    digitalWrite(5, LOW);
  }

  //Get the macAddress of the PIV ESP32 and copy it into a char array
  String macAddress = WiFi.macAddress();
  macAddress.toCharArray(myData.mac_addy, 18);

  //Send the result to the fire alarm panel ESP32
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  //Checks if it was delivered.
  if (result == ESP_OK) {
    Serial.println("Sent with success.");
    Serial.printf("X is %d.\n", myData.x);
    Serial.printf("Y is %d.\n", myData.y);
    Serial.printf("MAC Address is %s.\n", myData.mac_addy);
  } else {
    Serial.println("Error sending the data");
  }
  
  delay(5000); // Broadcast every 5 seconds
}
