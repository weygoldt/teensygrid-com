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

void CAN_send(const char *msg)
{
  CAN_message_t message;
  message.id = SLAVE3_ID;
  message.len = strlen(msg);
  memcpy(message.buf, msg, message.len);
  Can0.write(message);
}

const char *CAN_listen(uint32_t expected_id)
{
  static CAN_message_t msg;
  static char buf[256] = "";

  while (true)
  {
    if (Can0.read(msg))
    {
      if (msg.len > 0 && msg.id == expected_id)
      {
        memcpy(buf, msg.buf, msg.len);
        buf[msg.len] = '\0';
        return buf;
      }
    }
  }
}

void record()
{
  const unsigned long one_minute = 1UL * 30UL * 1000UL;
  unsigned long start_time = millis();
  unsigned long elapsed_time = 0;

  while (elapsed_time < one_minute)
  {
    CAN_send(MSG_RECORDING);
    blink();
    elapsed_time = millis() - start_time;

    // Check if one minute has elapsed and exit the function
    if (elapsed_time >= one_minute)
    {
      CAN_send(MSG_FINISHED);
      break;
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
  const char *msg = CAN_listen(MASTER_ID);
  Serial.print("Received message: ");
  Serial.println(msg);

  if (strcmp(msg, MSG_RESET) == 0)
  {
    record();
  }
}
