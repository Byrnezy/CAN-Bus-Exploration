#include "interface.h"
#include "behavior.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    // Initialize node based on arguments
    if (argc != 2) {
        printf("Usage: %s <node_number (1-5)>\n", argv[0]);
        return -1;
    }

    int node_num = atoi(argv[1]);
    if (node_num < 1 || node_num > NUM_NODES) {
        printf("Invalid node number. Must be between 1 and %d.\n", NUM_NODES);
        return -1;
    }

    // Initialize CAN interface
    int can_sock = can_init(INTERFACE);
    if (can_sock < 0) {
        fprintf(stderr, "Failed to initialize CAN interface.\n");
        return -1;
    }

    // Initialize node behavior
    node_init(can_sock, node_num);

    // Main loop can be managed within node_behavior.c
    node_run();

    // Cleanup (if reachable)
    can_close(can_sock);
    return 0;
}
