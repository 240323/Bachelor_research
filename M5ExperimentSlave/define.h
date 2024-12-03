#pragma once
#include <M5Core2.h>
#include <BluetoothSerial.h>
#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1
#define PRINTSCANRESULTS 0
#define DELETEBEFOREPAIR 0
#define MINV 1800
#define MAXV 3300

void vibrate(String, String);
