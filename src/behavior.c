// node_behavior.c
#include "behavior.h"
#include "interface.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static int can_sock_fd;
static canid_t my_id;
static int node_number;

// Example function to assign CAN ID based on node number
static canid_t get_can_id(int node_num) {
    switch(node_num) {
        case 1: return 0x100;
        case 2: return 0x200;
        case 3: return 0x300;
        case 4: return 0x400;
        case 5: return 0x500;
        default: return 0x100; // Default ID
    }
}

// Initialize node parameters
void node_init(int can_sock, int node_num) {
    can_sock_fd = can_sock;
    node_number = node_num;
    my_id = get_can_id(node_num);
    printf("Node %d initialized with CAN ID 0x%X.\n", node_num, my_id);
}

// Thread function for sending messages
static void* send_thread(void *arg) {
    while (1) {
        CAN_Message msg;
        msg.id = my_id;
        msg.dlc = 2; // Example data length
        msg.data[0] = node_number;      // Example data byte 1
        msg.data[1] = rand() % 256;     // Example data byte 2

        if (can_send(can_sock_fd, msg.id, msg.data, msg.dlc) == 0) {
            printf("Node %d sent message with ID 0x%X.\n", node_number, msg.id);
        }

        sleep(1); // Send every second
    }
    return NULL;
}

// Thread function for receiving messages
static void* receive_thread(void *arg) {
    while (1) {
        canid_t recv_id;
        uint8_t data[8];
        uint8_t dlc;

        if (can_receive(can_sock_fd, &recv_id, data, &dlc) == 0) {
            // Ignore messages sent by self
            if (recv_id != my_id) {
                printf("Node %d received message with ID 0x%X.\n", node_number, recv_id);
                // Process the message as needed
                // For example, print the data
                for(int i = 0; i < dlc; i++) {
                    printf("Data[%d]: %02X ", i, data[i]);
                }
                printf("\n");
            }
        }

        // Optionally add a small delay to prevent tight loop
        usleep(100000); // 100 ms
    }
    return NULL;
}

// Start node operation
void node_run() {
    pthread_t sender, receiver;

    // Create sender thread
    if(pthread_create(&sender, NULL, send_thread, NULL) != 0) {
        perror("Failed to create send thread");
        exit(EXIT_FAILURE);
    }

    // Create receiver thread
    if(pthread_create(&receiver, NULL, receive_thread, NULL) != 0) {
        perror("Failed to create receive thread");
        exit(EXIT_FAILURE);
    }

    // Join threads (infinite loop, so this will block indefinitely)
    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);
}
