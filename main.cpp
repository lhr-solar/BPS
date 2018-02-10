#include "mbed.h"

// Driver controls CAN base address and packet offsets
#define DC_BASE		0x220
#define MC_BASE   0x240
#define DC_DRIVE		0x01
#define DC_POWER		0x02
#define DC_RESET		0x03
#define DC_SWITCH		0x04

CAN can1(PB_8,PB_9,250000);
AnalogIn ain(A0);

#define MAX_VELOCITY 100
#define MAX_CURRENT 1.0

int main() {
  float current = MAX_CURRENT;
  float velocity = MAX_VELOCITY;
  float bus_current = MAX_CURRENT;
  float pedal_position;
  float data[2];
  char const * serial = "0002173";
  // can1.frequency(500000);
  int id;
  CANMessage msg;
  while (1) {
  	pedal_position = ain.read();
  	// current = MAX_CURRENT * pedal_position;
  	// velocity = MAX_VELOCITY * pedal_position;
  	// velocity = BASE_VELOCITY;
  	printf("Velocity: %.2f\n\r", velocity);
    data[1] = current;  // Flipped because of endianness
    data[0] = velocity;
    id = DC_BASE + DC_DRIVE;
    // id = DC_BASE + DC_RESET;
    // if (can1.write(CANMessage(id, (char*)data,8)))
    //   printf("Reset success. \n\r");
    // printf("Heartbeat");
    if (!can1.write(CANMessage(id, (char*)data, 8)))
      printf("Drive failed.\n\r");
    // else
      // printf("Drive failed.\n\r");
    // can1.reset();

    data[1] = bus_current;
    data[0] = 0.0;
    id = DC_BASE + DC_POWER;
    if (!can1.write(CANMessage(id, (char*)data, 8)))
      printf("Power failed.\n\r");
    // else
    //   printf("Power success.\n\r");
    // if (!can1.write(CANMessage(id, serial, 8)))
    //   printf("Serial failed.\n\r");
    // else
    //   printf("Serial success.\n\r");
    if (can1.read(msg)) {
      // if (msg.data[5] == 'T' && msg.data[6] == 'R' && msg.data[7] == 'I')
        // printf("Motor control base %d\n", msg.id);
      // printf("Msg ID %d:", msg.id);
      if (msg.id == MC_BASE) {
        for (int i = 0; i < 4; i++)
          printf("%c ", msg.data[i]);
        printf("%d", (int)msg.data[4]);
        printf("\n\r");
      }
    }
    //printf("Heartbeat\n\r");
    // wait_ms(10);  // Need message every 250ms to maintain operation
  }
}
