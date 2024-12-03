#include "define.h"

/*-------定義-----------*/
BluetoothSerial SerialBT;
bool BTconnected = false;

const uint8_t slaveMAC[] = { 0x3c, 0x61, 0x05, 0x0d, 0xc3, 0x09 };

const char divchar[] = ",";  //文字列分割時の区切り文字

void setup() {
  /*--------共通設定---------------------*/
  // M5の初期設定，LCDのログの表示設定
  M5.begin();
  Wire.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  Serial.begin(115200);

  // ESP-NOWの初期設定
  //  Set device in STA mode to begin with
  WiFi.mode(WIFI_STA);
  // This is the mac address of the Master in Station Mode
  Serial.print("STA MAC: ");
  Serial.println(WiFi.macAddress());
  M5.Lcd.print("STA MAC: ");
  M5.Lcd.println(WiFi.macAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  // esp_now_register_send_cb(OnDataSent);
  ScanForSlave();
  manageSlave();
  // マスタ側初期化処理
  M5.Lcd.println("Hi, I'm Master.");
  // SerialBTの初期設定
  uint8_t macBT[6];
  esp_read_mac(macBT, ESP_MAC_BT);
  M5.Lcd.print("BT MAC: ");
  M5.Lcd.printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n", macBT[0], macBT[1], macBT[2], macBT[3], macBT[4], macBT[5]);
  SerialBT.begin("M5BTSerial_slave");
  Serial.println("The device started, now you can pair it with bluetoth!");

  // マルチキャスト用slave登録(macアドレスすべて1でブロードキャスト, FF:FF:FF:FF:FF:FF)
  memset(&esp_now_slave, 0, sizeof(esp_now_slave));
  for (int i = 0; i < 6; ++i) {
    esp_now_slave.peer_addr[i] = slaveMAC[i];
    //esp_now_slave.peer_addr[i] = (uint8_t)0xff;
  }

  esp_err_t addStatus = esp_now_add_peer(&esp_now_slave);

  if (addStatus == ESP_OK) {
    Serial.println("ESPNow Add Peer success");
    M5.Lcd.println("ESPNow Add Peer success");
  }
  float batterylevel = M5.Axp.GetBatteryLevel();  //バッテリー残量取得
  Serial.println("Battery:" + String((int)batterylevel) + "%. ");
  M5.Lcd.println("Battery:" + String((int)batterylevel) + "%. ");  //Dataの最後に改行コードがくっついてる
  // データ送信時のコールバック関数登録
  esp_now_register_send_cb(OnDataSent);
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();
  if (M5.BtnA.isPressed()) {
    //ログ表示リセット
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(0, 0);
  }
  //Serial.println(batterylevel);
  //float batvol = M5.Axp.GetBatVoltage();
  //Serial.println(batvol);
  if (!BTconnected && !SerialBT.isClosed() && SerialBT.connected()) {
    Serial.println("SerialBT connected!");
    M5.Lcd.println("SerialBT connected!");
    BTconnected = true;
  }
  if (SerialBT.available()) {
    // マスタの処理
    //String Data = String(SerialBT.read());
    String Data = SerialBT.readString();
    //改行コードがくるまで配列に入れる．
    float batterylevel = M5.Axp.GetBatteryLevel();  //バッテリー残量取得
    Serial.println("Battery:" + String((int)batterylevel) + "%. " + "data from pc:" + Data);
    M5.Lcd.print("Battery:" + String((int)batterylevel) + "%. " + "data from pc:" + Data);  //Dataの最後に改行コードがくっついてる
    // データを分解，解析
    String vibData[3];
    divStr(vibData, Data);  //vibDataに文字列をカンマ区切りでいれる
    if (sizeof(vibData) / sizeof(vibData[0]) == 3) {
      //(データがスレーブ用の場合)ESP-NOWを使って振動データ送信
      if (vibData[0] == "W") {
        sendData(Data);
        //スレーブに振動命令
        vibrate(vibData[1], vibData[2]);
        // 自分宛ての振動実行
      }
      else if (vibData[0] == "L") {
        //左あての振動命令(L)
        sendData(Data);
      } else {
        // 自分宛ての振動実行(R)
        vibrate(vibData[1], vibData[2]);
      }
    }
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
