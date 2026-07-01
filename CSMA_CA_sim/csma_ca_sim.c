#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>


#define IFS 100000 // Interframe Space in microseconds (100ms)

void send_data(const char *frame) {
    printf("Sending data: %s\n", frame);
}

void send_RTS() {
    return;
}

bool timeout_reached(int elapsed_us, int timeout_us) {
    return elapsed_us >= timeout_us;
}

bool wait_for_CTS(int timeout_us, double success_probability) {
    int elapsed_us = 0;

    while (!timeout_reached(elapsed_us, timeout_us)) {
        usleep(100000); // Poll every 100ms
        elapsed_us += 100000;

        if (((double)rand() / RAND_MAX) < success_probability) {
            printf("CTS received.\n");
            return true;
        }
    }

    printf("Timed out waiting for CTS.\n");
    return false;
}

bool wait_for_ACK(int timeout_us, double success_probability) {
    int elapsed_us = 0;

    while (!timeout_reached(elapsed_us, timeout_us)) {
        usleep(100000); // Poll every 100ms
        elapsed_us += 100000;

        if (((double)rand() / RAND_MAX) < success_probability) {
            printf("ACK received.\n");
            return true;
        }
    }

    printf("Timed out waiting for ACK.\n");
    return false;
}

void CSMA_CA_sim() {
    int num_attempts = 0;
    bool channel_busy = (rand() % 8 == 0); // Randomly set the channel to busy or idle with probability 0.125 of being busy
    while (channel_busy) {
        printf("Channel is busy. Waiting...\n");
        usleep(250000); // Wait for 250ms
        channel_busy = (rand() % 8 == 0); // Check the channel status again
    }    
    printf("Channel is idle. \n");
    printf("Waiting for Interframe Space: %d ms\n", IFS / 1000);
    usleep(IFS); // Wait for Interframe Space
    int R = rand() % (int)pow(2, num_attempts); // Number of slots to wait before sending RTS, based on the number of attempts
    printf("Random backoff: %d slots\n", R);
    for (int i = 0; i < R; i++) {
        printf("Waiting for %d slots before sending RTS...\n", R);
        usleep(100000); // Wait for 100ms per slot
        if (channel_busy) { // Check the channel status again
            i = 0;
            while (channel_busy){ // wait until the channel is idle again
                printf("Channel is busy. Waiting...\n");
                usleep(250000); // Wait for 250ms
                channel_busy = (rand() % 8 == 0); // Check the channel status again
            }
        }
    }
    send_RTS(); // Send RTS after waiting for R slots
    bool cts_received = wait_for_CTS(1000000, 0.7); // Wait up to 1s for CTS with 70% success probability
    if (!cts_received) {
        printf("Retrying transmission after CTS timeout.\n");
        return;
    }

    send_data("Hello from CSMA/CA");
    bool ack_received = wait_for_ACK(1000000, 0.7); // Wait up to 1s for ACK with 70% success probability
    if (!ack_received) {
        printf("Transmission failed: no ACK received.\n");
    }
}

int main(){
    bool frame_to_send = true;
    if (frame_to_send){
        CSMA_CA_sim();
    }
    return 0;
}