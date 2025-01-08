#ifndef MESSAGE_H
#define MESSAGE_H

#include <linux/can.h>
#include <stdint.h>

typedef struct {
    canid_t id;
    uint8_t data[8];
    uint8_t dlc;
} CAN_Message;

// Function prototypes (if any)
void print_can_message(const CAN_Message *msg);

#endif // CAN_MESSAGE_H
