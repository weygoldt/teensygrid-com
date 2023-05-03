#include <Arduino.h>
#include <FlexCAN_T4.h>

// define the ID of the master and slave units
#define MASTER_ID 0x000
#define SLAVE1_ID 0x001
#define SLAVE2_ID 0x002
#define SLAVE3_ID 0x003

// define possible CAN bus messages
#define MSG_RESET "0"
#define MSG_RECORDING "1"
#define MSG_PANIC "2"
#define MSG_FINISHED "3"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

void blink()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

void can_send(const char *msg)
{
  CAN_message_t message;
  message.id = MASTER_ID;
  message.len = strlen(msg);
  memcpy(message.buf, msg, message.len);
  Can0.write(message);
}

void can_listen(CAN_message_t &msg)
{
  while (true)
  {
    Can0.read(msg);

    if (msg.len > 0)
    {
      Serial.print("Received message from ID ");
      Serial.print(msg.id, HEX);
      Serial.print(": ");
      for (int i = 0; i < msg.len; i++)
      {
        Serial.print(msg.buf[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
      return;
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Can0.begin();
  Can0.setBaudRate(1000000);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // Send reset message to all slaves
  can_send(MSG_RESET);

  // Listen for finished messages from all slaves
  bool slave1_finished = false;
  bool slave2_finished = false;
  bool slave3_finished = false;

  while (!slave1_finished || !slave2_finished || !slave3_finished)
  {
    CAN_message_t msg;
    can_listen(msg); // Listen for any message on the bus

    if (msg.len > 0 && strcmp((const char *)msg.buf, MSG_FINISHED) == 0)
    {
      if (msg.id == SLAVE1_ID)
      {
        slave1_finished = true;
        Serial.println("Slave 1 finished recording");
      }
      else if (msg.id == SLAVE2_ID)
      {
        slave2_finished = true;
        Serial.println("Slave 2 finished recording");
      }
      else if (msg.id == SLAVE3_ID)
      {
        slave3_finished = true;
        Serial.println("Slave 3 finished recording");
      }
    }

    blink();
  }

  // All slaves finished, send reset message
  can_send(MSG_RESET);
  Serial.println("All slaves finished recording, resetting");
}
