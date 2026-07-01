#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// Move the terminal cursor up one row using ANSI escape codes.
void printup(){
    printf("\033[A");
    return;
}

// Move the terminal cursor down one row using ANSI escape codes.
void printdown(){
    printf("\033[B");
    fflush(stdout);
    return;
}

// Clear the current line using an ANSI escape code.
void printclr(){
    printf("\033[2K");
    return;
}

// Move down n rows, overwrite the character beneath the cursor, then move back up.
void insertBeneath(char ch, int n){
    if (n < 1){
        printf("Error, printing less than one line beneath cursor.\n");
        exit(1);
    }
    for (int i = 0; i < n; i++){
        printdown();
    }
    printf("\b%c", ch);
    for (int i = 0; i < n; i++){
        printup();
    }
    return;
}

// Move down n rows, overwrite a string beneath the cursor, then move back up.
void insertBeneathStr(char * ch, char * model, int n){
    if (n < 1){
        printf("Error, printing less than one line beneath cursor.\n");
        exit(1);
    }
    for (int i = 0; i < n; i++){
        printdown();
    }
    int modelLen = strlen(model);
    for (int i = 0; i < modelLen; i++){
        printf("\b");
    }
    printf("%s", ch);
    for (int i = 0; i < n; i++){
        printup();
    }
    return;
}

// Convert a binary string like "1010" into its integer value.
void strToBinary(char *str, int *binary, int len) {
    *binary = 0;
    for (int i = 0; i < len; i++) {
        *binary = *binary + ((str[i] - '0') << (len - i - 1));
    }
    return;
}

// Convert an integer into a binary string of a fixed length.
void intToBinStr(int num, char *str, int len) {
    for (int i = 0; i < len; i++) {
        str[len - i - 1] = (num & 1) + '0';
        num >>= 1;
    }
    str[len] = '\0';
    return;
}

// Fill the destination with a random 32-bit pattern of 0s and 1s.
void randomInt(unsigned int* num){
    for (int i = 0; i < sizeof(unsigned int)*8;i++){
        unsigned int temp = rand() % 2;
        *num = *num + (temp << i);
    }
    return;
}



int main(){
    // Print an introduction to the bit-stuffing demonstration.
    printf("---- Bit stuffing demo program ----\n");
    printf("---- ---- ---- ---- ---- ---- ---- ----\n");
    printf("This program visually examines bit stuffing.\n\
In many data link control protocols, a variable size frame is used.\n\
Because of this varying size, we need a way to convey the begining and end of a frame.\n\
Special, 'Start/Stop flags' can be used.\n\
These flags can consist of a string of bits which denote the beginning and end of frame.\n\
However, if the string of bits or is used within the payload, we can inadvertently end the frame.\n\
Bit stuffing solves this by adding a bit to a given string every time it is encountered on the receiver side,\
and removed every time it is received on the receiver side.\n\
For example, the bit string 01111110 is often used as a start/stop flag.\n\
One solution would be to add a zero after 0 followed by 5 ones (011111) is encountered on the sender side.\n\
On the receiver side we would remove a zero following the same string of bits, after the start/stop flags have been properly identified.\n");
    printf("An example of this is displayed below: ");

    // Buffer and state for the animated display.
    char stream[sizeof(int)*8];
    char chkbuf[6] = {'0','0','0','0','0','0'};
    unsigned int bitstream[100];
    int termLength = 0;
    char *flag = "01111110";

    // Move the cursor up a few rows so the animation sits in a consistent place.
    for (int i = 0; i < 4; i++){
        printup();
    }

    // Keep generating random bit patterns and animating the stuffing process.
    while (1){
        for (int i = 0; i< 100; i++){
            randomInt(&(bitstream[i]));
            intToBinStr(bitstream[i], stream, sizeof(int)*8);

            // Process each bit of the generated word.
            for (int j = 0; j < sizeof(int)*8; j++){
                // Start a new visual block after every 64 displayed characters.
                if (termLength % 64 == 0){
                    insertBeneathStr(flag, "",1);
                    insertBeneathStr("stop flag ^^^^^^^^", "stop flag ^^^^^^^^", 2);
                    printf("\n\n\n\n\n\n\n\n\n\n\033[A\033[A\033[A");
                    printup();
                    printf("Original Data Block:  ");
                    insertBeneathStr("Stream Sent: ", "Original Data Block:   ", 1);
                    insertBeneathStr(flag, "",1);
                    printf("\r");
                    insertBeneathStr("  start flag ^^^^^^^^", "", 2);
                    printf("\r");
                    insertBeneathStr("Data received:       ", "", 3);
                    //insertBeneath(*(flag), 1);
                }
                termLength++;
                printf("%c", stream[j]);
                insertBeneath(stream[j], 1);
                insertBeneath(stream[j], 3);
                chkbuf[5] = stream[j];

                // Detect the "011111" pattern and visually insert a stuffed zero.
                if ((chkbuf[0] == '0') && (chkbuf[1] == '1') && (chkbuf[2] == '1') && (chkbuf[3] == '1') && (chkbuf[4] == '1') && (chkbuf[5] == '1')){
                    printf(" ");
                    insertBeneath('0', 1);
                    insertBeneathStr("stuff^", "stuff^", 2);
                    insertBeneathStr("unpack^", "unpack^", 4);
                }

                // Shift the detection window one bit to the right.
                chkbuf[0] = chkbuf[1];
                chkbuf[1] = chkbuf[2];
                chkbuf[2] = chkbuf[3];
                chkbuf[3] = chkbuf[4];
                chkbuf[4] = chkbuf[5];
                usleep(125000);
            }
        }
    }

    return 0;
}