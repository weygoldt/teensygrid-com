#include <Arduino.h>
#include <FlexCAN_T4.h>

#define MESSAGE "test"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

void setup() {
  Serial.begin(115200);
  // while (!Serial) {}

  Can0.begin();
  Can0.setBaudRate(1000000);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  CAN_message_t msg;
  msg.id = 0x123;
  msg.len = strlen(MESSAGE);
  memcpy(msg.buf, MESSAGE, msg.len);

  Can0.write(msg);

  // print the message to the serial monitor
  Serial.print("Sent message: ");
  Serial.println(MESSAGE);
  Serial.println(msg.id, HEX);
  Serial.println(msg.len);

  // wait 10 seconds before sending the next message
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1 * 1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1 * 1000);
}