#include <Arduino.h>
#include <FlexCAN_T4.h>

// define the ID of the master and slave units
#define MASTER_ID 0x000
#define SLAVE_ID 0x123

// define possible CAN bus messages 
#define MSG_RESET "0"
#define MSG_RECORDING "1"
#define MSG_PANIC "2"
#define MSG_FINISHED "3"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

void blink() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

void can_send(const char* msg) {
  CAN_message_t message;
  message.id = MASTER_ID;
  message.len = strlen(msg);
  memcpy(message.buf, msg, message.len);
  Can0.write(message);
}

const char* can_listen(uint32_t expected_id) {
  static CAN_message_t msg;
  static char buf[256];

  while (true) {
    Can0.read(msg);

    if (msg.len > 0 && msg.id == expected_id) {
      Serial.println(msg.id, HEX);
      memcpy(buf, msg.buf, msg.len);
      buf[msg.len] = '\0';
      return buf;
    }
  }
}

void setup() {
  Serial.begin(115200);
  Can0.begin();
  Can0.setBaudRate(1000000);
  pinMode(LED_BUILTIN, OUTPUT);

  // can_send(MSG_RESET);
  // blink();
}

void loop() {
  can_send(MSG_RESET);
  const char* SLAVE_STATUS = can_listen(SLAVE_ID);
  
  while (strcmp(SLAVE_STATUS, MSG_FINISHED) != 0) {
    SLAVE_STATUS = can_listen(SLAVE_ID);
    Serial.println(SLAVE_STATUS);
    blink();
  }

  if (strcmp(SLAVE_STATUS, MSG_FINISHED) == 0) {
    Serial.println("Slave finished recording");
    can_send(MSG_RESET);
  }
}
