#include <Arduino.h>
#include <FlexCAN_T4.h>

// define the ID of the master and slave units
#define MASTER_ID 0x000
#define SLAVE_ID 0x123

// create a CAN bus object
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

void blink() {
  // blink the LED to indicate that program is running
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
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

  // read the message from the can bus
  Can0.read(msg);

  // print the message to the serial monitor 
  if (msg.len > 0 && msg.id == MASTER_ID) {

    // print the sender ID
    Serial.println(msg.id, HEX);

    // do some magic to print the message
    char buf[msg.len + 1];
    memcpy(buf, msg.buf, msg.len);
    buf[msg.len] = '\0';

    // print the message
    Serial.print("Received message: ");
    Serial.println(buf);
  }

  blink();
}

