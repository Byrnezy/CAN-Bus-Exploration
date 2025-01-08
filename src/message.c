#include "message.h"
#include <stdio.h>

void print_can_message(const CAN_Message *msg) {
    printf("ID: 0x%X, DLC: %d, Data: ", msg->id, msg->dlc);
    for(int i = 0; i < msg->dlc; i++) {
        printf("%02X ", msg->data[i]);
    }
    printf("\n");
}
