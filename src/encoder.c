#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this is a basic shellcode encoder project that i decided to do
// i'll document what each part does as i go along


// defining the array of bad characters, these can ruin your injection or exploit
const unsigned char bad_chars[] = {0x00, 0x0A, 0x0D, 0xFF};
const int BAD_CHARS_LENS = 4;


// defining the key for XOR
#define DEFAULT_KEY 0x55


// function prototypes
int bad_byte(unsigned char byte);
unsigned char byte_encode(unsigned char original, unsigned char *key);


// helper for checking if byte is bad
int bad_byte(unsigned char byte)
{
    for (int i = 0; i < BAD_CHARS_LENS; i++) {
        if (byte == bad_chars[i])
        {
            return 1;
        }
    }
    return 0;
}


// helper for encoding
unsigned char encode_byte(unsigned char original, unsigned char *key) {
    unsigned char result;
    int attempts = 0;

    while (1) {
        result = original ^ (*key);
        result = (result + (*key)) & 0xFF;
        result = result ^ 0x11;

        if (!bad_byte(result)) {
            return result;
        }

        (*key)++;
  
        attempts++;
        if (attempts > 256) {
            fprintf(stderr, "Error: Failed to encode byte 0x%02x\n", original);
            exit(1);
        }
    }
}

int main (int argc, char *argv[]) {

    
    // outputing the usage
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s <Shellcode.bin File> [output_hex]\n", argv[0]);
        return 1;
    }


    // setting file argument and reading and opening the file
    const char *filename = argv[1];

    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    if (file_size <= 0) {
        fprintf(stderr, "Error: File is empty\n");
        fclose(file);
        return 1;
    }


    // memory handling and stuff
    unsigned char *shellcode = (unsigned char *)malloc(file_size);
    unsigned char *encoded = (unsigned char *)malloc(file_size);

    if (!shellcode || !encoded) {
        fprintf(stderr, "Error: Failed to allocate memory\n");
        fclose(file);
        return 1;
    }

    size_t bytes_read = fread(shellcode, 1, file_size, file);
    fclose(file);

    if (bytes_read != file_size) {
        fprintf(stderr, "Error: Could not read file\n");
        free(shellcode);
        free(encoded);
        return 1;
    }

    unsigned char current_key = DEFAULT_KEY;
    printf("[+] Encoded Shellcode: Key starts at 0x%02X\n", current_key);
    printf("[+] Payload[] = {\n");


    // looping through and encoding the bytes
    for (long i = 0; i < file_size; i++) {
        unsigned char encoded = encode_byte(shellcode[i], &current_key);
        unsigned char key_used = current_key;

        
        printf("0x%02x,0x%02x", encoded, key_used);

        if (i < file_size - 1) {
            printf(", ");
            if ((i + 1) % 8 == 0) printf("\n");
        }

        current_key++;
    }

    printf("\n}\n");
    printf("[+] Total length: %ld bytes\n", file_size);


    // cleanup after its done
    free(shellcode);
    free(encoded); 

    return 0;
}
