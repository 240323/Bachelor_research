#pragma once
#include <M5Core2.h>
#include <BluetoothSerial.h>
#include <esp_now.h>
#include <WiFi.h>

/*-------定義-----------*/
extern BluetoothSerial SerialBT;

extern esp_now_peer_info_t esp_now_slave;  // espnowの通信相手=slave

#define CHANNEL 1
#define PRINTSCANRESULTS 0
#define DELETEBEFOREPAIR 0
#define MINV 1800
#define MAXV 3300

extern const uint8_t slaveMAC[];

void vibrate(String, String);
void InitESPNow();
void OnDataSent(const uint8_t *, esp_now_send_status_t);
bool manageSlave();
void ScanForSlave();
void sendData(String);
void deletePeer();
void divStr(String*, String);