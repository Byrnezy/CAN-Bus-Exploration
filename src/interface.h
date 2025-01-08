#ifndef INTERFACE_H
#define INTERFACE_H

#include <linux/can.h>

#define NUM_NODES 5
#define INTERFACE "can0"

// Function prototypes
int can_init(const char *interface);
int can_send(int sockfd, canid_t id, uint8_t *data, uint8_t dlc);
int can_receive(int sockfd, canid_t *id, uint8_t *data, uint8_t *dlc);
void can_close(int sockfd);

#endif 
