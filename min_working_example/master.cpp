#include <Arduino.h>
#include <FlexCAN_T4.h>

// message must be less than 8 bytes
#define MESSAGE "test"

// define the ID of the master and slave units
#define MASTER_ID 0x000
#define SLAVE_ID 0x123

// create a CAN bus object
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

void blink() {
  // blink the LED to indicate that program is running
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

void setup() {
  // initialize serial monitor
  Serial.begin(115200);

  // initialize CAN bus
  Can0.begin();
  Can0.setBaudRate(1000000);

  // initialize LED
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // create a message object
  CAN_message_t msg;

  // set message attributes
  msg.id = SLAVE_ID;
  msg.len = strlen(MESSAGE);
  memcpy(msg.buf, MESSAGE, msg.len);

  // send the message
  Can0.write(msg);

  // print the message to the serial monitor
  Serial.print("Sent message: ");
  Serial.println(MESSAGE);

  // wait xx seconds before sending the next message
  blink();
}

