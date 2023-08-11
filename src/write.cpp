#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>

const uint16_t kIrLed = 13; //赤外線LEDのピンを取得

IRsend irsend(kIrLed);                  // 送信オブジェクト

bool success = true;
decode_type_t protocol = NEC;
uint16_t size = 32;
const uint32_t rightup = 0x41B6659A;    //部屋の明かりをつけるコード
const uint32_t turnoff = 0x41B67D82;    //部屋の明かりをつけるコード

void setup() {
  irsend.begin(); // 赤外線LEDの設定
  Serial.begin(115200);
}

void loop() {
  success = irsend.send(protocol, rightup, size); //送信
  delay(1000);
  success = irsend.send(protocol, turnoff, size); //送信
  delay(1000);
}
