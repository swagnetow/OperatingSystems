#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void decimal_to_octal(unsigned long long);

int main(int argc, char** argv) {
    unsigned long long virtual_address;
    unsigned long long page_number;
    unsigned long long offset;

    virtual_address = atoll(argv[1]);

    /* Bit shifting. */
    page_number = virtual_address >> 15;

    /* Bit masking. */
    offset = virtual_address & 0x7FFF;

    printf("The address %lld is equal to ", virtual_address);
    decimal_to_octal(virtual_address);

    printf("Page number: %lld decimal and ", page_number);
    decimal_to_octal(page_number);

    printf("Offset = %lld decimal and ", offset, offset);
    decimal_to_octal(offset);

    return 0;
}

void decimal_to_octal(unsigned long long decimal) {
    char octal[64];
    char temp[64];
    int i = 0;
    int k = 0;
    int remainder;

    memset(octal, 0, sizeof(char));
    memset(temp, 0, sizeof(char));

    while(decimal > 0) {
        remainder = decimal % 8;
        decimal /= 8;
        temp[i++] = remainder + '0';
    }

    while(i >= 0) {
        octal[k++] = temp[--i];
    }

    octal[k] = 0;

    printf("%s octal\n", octal);
}
