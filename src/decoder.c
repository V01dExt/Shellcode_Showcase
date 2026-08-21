#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <Encoded_Shellcode.bin>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "rb");
    if (!file) { perror("Error opening file"); return 1; }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    long shellcode_size = file_size / 2;

    if (shellcode_size <= 0) {
        fprintf(stderr, "Error: File is empty or invalid\n");
        fclose(file);
        return 1;
    }

    printf("unsigned char shellcode[] = {\n");

    for (long i = 0; i < shellcode_size; i++) {
        unsigned char encoded, key;
        if (fread(&encoded, 1, 1, file) != 1) break;
        if (fread(&key, 1, 1, file) != 1) break;


        unsigned char step1 = encoded ^ 0x11;
        unsigned char step2 = (step1 - key) & 0xFF;
        unsigned char decoded = step2 ^ key;

        printf("0x%02x", decoded);
        if (i < shellcode_size - 1) {
            printf(", ");
            if ((i + 1) % 16 == 0) printf("\n");
        }
    }

    printf("\n};\n");
    printf("[+] Total length: %ld bytes\n", shellcode_size);
    fclose(file);
    return 0;
}