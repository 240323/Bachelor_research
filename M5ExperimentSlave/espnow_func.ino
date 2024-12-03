#include "define.h"

const char divchar[] = ",";  //文字列分割時の区切り文字

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
    M5.Lcd.println("ESPNow Init Success");
  } else {
    Serial.println("ESPNow Init Failed");
    M5.Lcd.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

// 受信コールバック
void OnDataRecv(const uint8_t* mac_addr, const uint8_t* data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.printf("Last Packet Recv from: %s\n", macStr);
  Serial.printf("Last Packet Recv Data(%d): ", data_len);
  char received_data[data_len + 1];  //終端文字分+1
  //received_data[data_len] = '\n';
  for (int i = 0; i < data_len; i++) {
    received_data[i] = data[i];
    Serial.print(received_data[i]);
  }
  Serial.println("");

  //つけたしここから--------------------
  float batterylevel = M5.Axp.GetBatteryLevel();  //バッテリー残量取得
  Serial.println("Battery:" + String((int)batterylevel) + "%. " + "data from Master:" + received_data);
  M5.Lcd.print("Battery:" + String((int)batterylevel) + "%. " + "data from Master:" + received_data);  //Dataの最後に改行コードがくっついてる
  //文字列分解
  String vibData[3];
  divStr(vibData, received_data);  //vibDataに文字列をカンマ区切りでいれる
  if (sizeof(vibData) / sizeof(vibData[0]) == 3) {
    //振動処理
    vibrate(vibData[1], vibData[2]);
  }
}

void divStr(String* vibData, String Data) {
  //受け取った文字列をカンマ区切りにして配列に入れる関数
  char* token;  //分離後のもじれつをさすぽいんた
  char c[100];
  Data.toCharArray(c, sizeof(c));
  token = strtok(c, divchar);
  if (token == NULL) {
    return;
  }
  vibData[0] = String(token);
  vibData[0].trim();
  for (int i = 1; i < 3; i++) {
    token = strtok(NULL, divchar);
    if (token == NULL) {
      return;
    }
    vibData[i] = String(token);
    vibData[i].trim();
  }
}
