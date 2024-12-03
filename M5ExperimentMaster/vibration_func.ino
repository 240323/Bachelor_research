#include "define.h"

void vibrate(String intensity = "HIGH", String duration = "1000") {
    //振動強度設定
    if (intensity == "LOW") {
      M5.Axp.SetLDOVoltage(3, MINV);
    }
    if (intensity == "MID") {
      M5.Axp.SetLDOVoltage(3, (MINV + MAXV) / 2);
    }
    if (intensity == "HIGH") {
      M5.Axp.SetLDOVoltage(3, MAXV);
    }
    M5.Axp.SetLDOEnable(3, true);   //振動開始
    delay(duration.toInt());        //振動長さ
    M5.Axp.SetLDOEnable(3, false);  //振動停止
}

