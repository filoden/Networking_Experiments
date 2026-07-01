#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printup(){ // move cursor up wrapper
    printf("\033[A");
    return;
}

void printdown(){ //move cursor down wrapper
    printf("\033[B");
    return;
}

void printclr(){ // clear line wrapper
    printf("\033[2K");
    return;
}

void strToBinary(char *str, int *binary, int len) { // Pre: str is a string of only 0s and 1s, len is the length of the string
    // Post: binary contains the integer representation of the binary string
    *binary = 0;
    for (int i = 0; i < len; i++) {
        *binary = *binary + ((str[i] - '0') << (len - i - 1));
    }
    return;
}

void intToBinStr(int num, char *str, int len) { // Pre: num is an integer, len is the length of the string
    // Post: str contains the binary string representation of the integer
    for (int i = 0; i < len; i++) {
        str[len - i - 1] = (num & 1) + '0';
        num >>= 1;
    }
    str[len] = '\0';
    return;
}


int main() {

    // Give explanation
    printf("---- Checksum Calculation Program ----\n\n");
    printf("A cyclic redundancy check (CRC) is a method of detecting errors in data transmission or storage.\n\
It works by taking the dataword, shifting it to the right, dividing it by a predefined divisor, and concatenating the remainder.\n");
    printf("For exmaple, say we have a 4 bit dataword, and a 4 bit divisor.\n\
We will shift the dataword right by three bits, and then divide it by the divisor.\n\
The remainder will be concatenated to the dataword to form the codeword.\n");

    // Collect first user input
    while (1) {
    printf("\nChoose a dataword of any length (e.g., 01101) to see for yourself:\n");
    char dataword[500];
    scanf("%s", dataword);
    int redo = 0;
    int datawordlen = strlen(dataword);

    // Validate first user input
    for (int i = 0; i < datawordlen; i++) {
        if (dataword[i] != '0' && dataword[i] != '1') {
            printf("Invalid input. Please enter a word of only 0s and 1s: ");
            redo = 1;
            break;
        }
    }
    if (redo) {
        continue;
    }

    // Collect second user input
    printf("Now choose a divisor (generator) of any length smaller than the dataword (e.g., 1101). Note that any leading zeros are truncated:\n");
    char divisor[500];
    scanf("%s", divisor);
    int divisorlen = strlen(divisor);

    // Validate second user input
    for (int i = 0; i < divisorlen; i++) {
        if (divisor[i] != '0' && divisor[i] != '1') {
            printf("Invalid input. Please enter a word of only 0s and 1s: ");
            redo = 1;
            break;
        }
    }
    if (redo) {
        continue;
    }

    // Remove any leading zeros
    int circuitbreaker;
    for (int i = 0; i < divisorlen; i++) {
        circuitbreaker = 0;
        int j = 0;
        //printf("Loop: %d, Divisor: %s", i, divisor);
        if (divisor[i] == '0') {
            j = i;
            i--;
            while ( (divisor[j] != '\0') && (circuitbreaker < 100)){
                divisor[j] = divisor[j+1];
                circuitbreaker++;
                j++;
            }
        }
        else{
            break;
        }
    }
    divisorlen = strlen(divisor);

    // Convert binary input into integer format
    int dataword_int= 0;
    int divisor_int = 0; 
    strToBinary(dataword, &dataword_int, datawordlen);
    strToBinary(divisor, &divisor_int, divisorlen);

    dataword_int <<= (divisorlen - 1); // Shift left by (divisor size minus 1) bits (equivalent to multiplying by 2^(divisorlen-1))
    intToBinStr(dataword_int, dataword, datawordlen + divisorlen - 1); // Convert back to binary string with (datawordlen + divisorlen - 1) bits
    printf("We first zero pad the dataword by %d bits (divisor size minus 1) to the right, giving us: %s\n", divisorlen, dataword);
    printf("Next, we divide the dataword by the divisor using binary long division.\nOnly the remainder is useful to us.\nThis is efficiently accomplished using the XOR operator:\n");

    int dataword_temp = dataword_int; // Store the original dataword for later use
    for (int i = 0; i < divisorlen + 1; i++) {
        printf(" ");
    }
    for (int i = 0; i < datawordlen + divisorlen; i++) {
        printf("_");
    }
    printf("\n");
    printf("%s |%s\n", divisor, dataword);
    int i = 0;
    while (dataword_int >= divisor_int) {
        if (dataword[i] == '1') {
            printf("XOR");
            for (int j = 0; j < i + divisorlen - 3; j++) {
                printf(" ");
            }
            printf("  %s\n", divisor);
            dataword_int ^= (divisor_int << (datawordlen - 1 - i)); // Perform XOR operation
            for (int j = 0; j < divisorlen + 1; j++) {
                printf(" ");
            }
            for (int j = 0; j < datawordlen + divisorlen; j++) {
                printf("_");
            }
            printf("\n");
            for (int j = 0; j < divisorlen + 1; j++) {
                printf(" ");
            }
            intToBinStr(dataword_int, dataword, datawordlen + divisorlen - 1);
            printf(" %s\n", dataword);
        }
        else {
        }
        i++;
        
    }
    printf("The remainder is: %s\n", dataword);
    printf("Now we concatenate the remainder to the original dataword to form the codeword:\n");
    char dataword_temp_str[500];
    char remainder_str[500];
    char codeword_str[500];
    intToBinStr(dataword_temp, dataword_temp_str, datawordlen + divisorlen - 1); // Convert remainder to binary string with (divisorlen - 1) bits
    intToBinStr(dataword_int, remainder_str, datawordlen + divisorlen - 1); // Convert remainder to binary string with (divisorlen - 1) bits
    int codeword = (dataword_temp) | (dataword_int); // Concatenate remainder to original dataword

    intToBinStr(codeword, codeword_str, datawordlen + divisorlen - 1); // Convert codeword to binary string with (datawordlen + divisorlen - 1) bits
    printf("%s OR %s = %s\n", dataword_temp_str, remainder_str, codeword_str);


}
    return 0;
}