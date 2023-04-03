#include <Arduino.h>
#include <FlexCAN_T4.h>

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;


void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  Can0.begin();
  Can0.setBaudRate(1000000);
  pinMode(LED_BUILTIN, OUTPUT);
}

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

void loop() {
  blink();
  CAN_message_t msg;
  Can0.read(msg);
  if (msg.len > 0 && msg.id != 0) {
    Serial.println(msg.id, HEX);
    char buf[msg.len + 1];
    memcpy(buf, msg.buf, msg.len);
    buf[msg.len] = '\0';
    Serial.print("Received message: ");
    Serial.println(buf);
  }
}
