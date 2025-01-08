#include "interface.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int can_init(const char *interface) {
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_can addr;

    // Create socket
    if ((sockfd = socket(AF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Socket");
        return -1;
    }

    // Specify CAN interface
    strcpy(ifr.ifr_name, interface);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        perror("IOCTL");
        close(sockfd);
        return -1;
    }

    // Bind the socket to the CAN interface
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind");
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int can_send(int sockfd, canid_t id, uint8_t *data, uint8_t dlc) {
    struct can_frame frame;
    frame.can_id = id;
    frame.can_dlc = dlc;
    memcpy(frame.data, data, dlc);

    int nbytes = write(sockfd, &frame, sizeof(frame));
    if (nbytes != sizeof(frame)) {
        perror("Write");
        return -1;
    }

    return 0;
}

int can_receive(int sockfd, canid_t *id, uint8_t *data, uint8_t *dlc) {
    struct can_frame frame;
    int nbytes = read(sockfd, &frame, sizeof(frame));

    if (nbytes < 0) {
        perror("Read");
        return -1;
    } else if (nbytes < sizeof(struct can_frame)) {
        fprintf(stderr, "Incomplete CAN frame\n");
        return -1;
    }

    *id = frame.can_id;
    *dlc = frame.can_dlc;
    memcpy(data, frame.data, frame.can_dlc);

    return 0;
}

void can_close(int sockfd) {
    close(sockfd);
}
