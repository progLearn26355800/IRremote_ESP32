#include <Arduino.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>
const uint16_t kRecvPin = 15; //受信ピン
const uint32_t kBaudRate = 115200;  //シリアルの通信レート
const uint16_t kCaptureBufferSize = 1024; //受信データのバッファサイズ
#if DECODE_AC
const uint8_t kTimeout = 50;
#else
const uint8_t kTimeout = 15;
#endif
const uint16_t kMinUnknownSize = 12;
#define LEGACY_TIMING_INFO false

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true); //受信用オブジェクト
decode_results results; //結果格納用

void setup() {
Serial.begin(kBaudRate, SERIAL_8N1);
#if DECODE_HASH
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) { //データを受け取った場合
    uint32_t now = millis();  //現在の時間を取得
    Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
    if (results.overflow) //データがオーバーフローしたとき
      Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
    Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_ "\n");
    Serial.print(resultToHumanReadableBasic(&results)); //結果を人間が読める形にしてシリアルに表示
    String description = IRAcUtils::resultAcToString(&results);
    if (description.length()) Serial.println(D_STR_MESGDESC ": " + description);
    yield(); //WatchDogTimerを正常に動作させるため（？）
#if LEGACY_TIMING_INFO
    Serial.println(resultToTimingInfo(&results));
    yield();
#endif
    Serial.println(resultToSourceCode(&results));
    Serial.println();
    yield();
  }
}
