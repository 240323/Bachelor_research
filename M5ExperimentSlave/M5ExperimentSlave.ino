#include "define.h"

void setup() {
  /*--------共通設定---------------------*/
  // M5の初期設定，LCDのログの表示設定
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  Serial.begin(115200);

  // ESP-NOWの初期設定
  //  Set device in STA mode to begin with
  WiFi.mode(WIFI_AP);
  // This is the mac address of the Master in Station Mode
  // Serial.print("STA MAC: ");
  // Serial.println(WiFi.macAddress());
  // M5.Lcd.print("STA MAC: ");
  // M5.Lcd.println(WiFi.macAddress());
  Serial.print("AP MAC: ");
  Serial.println(WiFi.softAPmacAddress());
  M5.Lcd.print("AP MAC: ");
  M5.Lcd.println(WiFi.softAPmacAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();

  // スレーブ側初期化処理
  M5.Lcd.println("Hi, I'm Slave.");
  float batterylevel = M5.Axp.GetBatteryLevel();  //バッテリー残量取得
  Serial.println("Battery:" + String((int)batterylevel) + "%. ");
  M5.Lcd.println("Battery:" + String((int)batterylevel) + "%. ");  //Dataの最後に改行コードがくっついてる
  // データ受信時のコールバック関数登録
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();
  if (M5.BtnA.isPressed()) {
    //ログ表示リセット
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(0, 0);
  }
}
