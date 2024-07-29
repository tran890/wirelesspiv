#include <esp_wifi.h>
#include <ESP32_NOW.h>
#include <WiFi.h>
#include <esp_mac.h>

//ESP-Now Channel
#define ESPNOW_WIFI_CHANNEL 6

// Structure to receive data (must match sender structure)
typedef struct struct_message {
  int id;            // Board ID
  int x;             // PIV Status
  int y;             // Heartbeat Signal
  char mac_addr[18]; // Array to hold MAC address string
} struct_message;

//Holds incoming data
struct_message myData;

// Structures to hold readings from each board
struct_message board1;
struct_message board2;
struct_message board3;

//Array of all of the boards
struct_message boardsStruct[3] = {board1, board2, board3};

//Variables to track is a board gets disconnected
bool board1_status = false;
bool board2_status = false;
int board1_cnt = 0;
int board2_cnt = 0;

// Callback function when data is received
void OnDataRecv(uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  // Copy incoming data to myData structure
  memcpy(&myData, incomingData, sizeof(myData));

  //Checks if ID is valid
  if (myData.id >= 1 && myData.id <= 3) {
    // Copy individual fields to boardsStruct
    boardsStruct[myData.id - 1].x = myData.x;
    boardsStruct[myData.id - 1].y = myData.y;
    // Copy MAC address string
    strcpy(boardsStruct[myData.id - 1].mac_addr, myData.mac_addr);

    // Print received data
    Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
    Serial.printf("X value: %d \n", boardsStruct[myData.id - 1].x);
    Serial.printf("Y value: %d \n", boardsStruct[myData.id - 1].y);

    //Prints that the board is still alive
    if(boardsStruct[myData.id - 1].y == 10){
      Serial.printf("Board %d is alive!\n", myData.id);
    }

    //Prints the status of the PIV
    if(boardsStruct[myData.id - 1].x == 30){
      Serial.printf("PIV Status: Closed!\n", myData.id);
    } else if(boardsStruct[myData.id - 1].x == 20){
      Serial.printf("PIV Status: Open!\n", myData.id);
    }

    //Prints the mac address of the board
    Serial.printf("MAC Address: %s \n", boardsStruct[myData.id - 1].mac_addr);
    Serial.println();
  } else {
    Serial.println("Invalid board ID received");
  }

  //Counts up if it doesn't receive message from that specific board and resets if it does
  if(myData.id == 1){
    board1_status = true;
    board1_cnt = 0;
  } else{
    board1_status = false;
  }

  //Counts up if it doesn't receive message from that specific board and reseits if it does
  if(myData.id == 2){
    board2_status = true;
    board2_cnt = 0;
  } else{
    board2_status = false;
  }

}

void setup() {
  //Setting the baud rate
  Serial.begin(115200);

  pinMode(5, OUTPUT); // Heartbeat LED
  pinMode(23, OUTPUT); // PIV Control

  //Sets the ESP32 as a wifi station and the channel
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
  while (!WiFi.STA.started()) {
    delay(100);
  }

  //Prints initial messages
  Serial.println("Broadcasting Fire Alarm Panel.");
  Serial.println("Wi-Fi parameters:");
  Serial.println("  Mode: STA");
  Serial.println("  MAC Address: " + WiFi.macAddress());
  Serial.printf("  Channel: %d\n", ESPNOW_WIFI_CHANNEL);

  //In case ESP-NOW protocol fails
  if (!ESP_NOW.begin()) {
    Serial.println("Failed to initialize ESP-NOW");
    Serial.println("Restarting in 5 seconds...");
    delay(5000);
    ESP.restart();
  }

  //Register receiving and callback function
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  Serial.println("Setup complete. Waiting for master devices to broadcast...");
}

void loop() {
  //Variable to indicate if a message was received
  int msgReceived = 0;

  //Assign variables of board 1
  int board1X = boardsStruct[0].x;
  int board1Y = boardsStruct[0].y;
  char macAddress1[18];  
  String mac_addr1 = boardsStruct[0].mac_addr;
  mac_addr1.toCharArray(macAddress1, 18);

  //Assign variables of board 2
  int board2X = boardsStruct[1].x;
  int board2Y = boardsStruct[1].y;
  char macAddress2[18];
  String mac_addr2 = boardsStruct[1].mac_addr;
  mac_addr2.toCharArray(macAddress2, 18);

  //Checks if board1 hasn't sent a message in 10 loops
  if(!board1_status){
    board1_cnt++;
    if(board1_cnt >= 10){
      Serial.println("Board 1 is not responding.");
      Serial.printf("MAC Address: %s \n\n", macAddress1);
    }
  } 

  //Checks if board2 hasn't sent a message in 10 loops
  if(!board2_status) {
    board2_cnt++;
    if(board2_cnt >= 10){
      Serial.println("Board 2 is not responding.");
      Serial.printf("MAC Address: %s \n\n", macAddress2);
    }
  } 

  //Checks if it gets a message from either board 1 or 2
  if(board1_status || board2_status) {
    //Blinks the yellow LED for heartbeat
    if (board1Y == 10 || board2Y == 10) {
      digitalWrite(5, HIGH);
      delay(1000);
      digitalWrite(5, LOW);
      delay(1000);
      msgReceived = 1;
    }

    //Turns on red LED for closed PIV indication
    if (board1X == 30 || board2X == 30) {
    digitalWrite(23, HIGH);
    msgReceived = 1;
    } else {
    digitalWrite(23, LOW);
    msgReceived = 1;
    }
  }

  //10 Second Delay
  delay(10000);
}
