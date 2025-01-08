#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <linux/can.h>

// Function prototypes
void node_init(int can_sock, int node_num);
void node_run();

#endif 
