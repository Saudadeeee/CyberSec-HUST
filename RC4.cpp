#include <iostream>
#include <cstring>

unsigned char S[256];

void rc4_initialize(const unsigned char* key, int keyLength) {
    for (int i = 0; i < 256; i++) {
        S[i] = i;
    }
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % keyLength]) % 256;
        unsigned char temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
}
void rc4_setup(const char* key) {
    rc4_initialize((const unsigned char*)key, strlen(key));
}

void generateKeyStream(unsigned char* keyStream, int length) {
    unsigned char tempS[256];
    memcpy(tempS, S, 256);
    
    int i = 0, j = 0;
    for (int k = 0; k < length; k++) {
        i = (i + 1) % 256;
        j = (j + tempS[i]) % 256;
        unsigned char temp = tempS[i];
        tempS[i] = tempS[j];
        tempS[j] = temp;
        
        keyStream[k] = tempS[(tempS[i] + tempS[j]) % 256];
    }
}

void encrypt(const char* plaintext, unsigned char* ciphertext, int length) {
    unsigned char keyStream[length];
    generateKeyStream(keyStream, length);
    
    for (int i = 0; i < length; i++) {
        ciphertext[i] = plaintext[i] ^ keyStream[i];
    }
}

void decrypt(const unsigned char* ciphertext, char* plaintext, int length) {
    unsigned char keyStream[length];
    generateKeyStream(keyStream, length);
    
    for (int i = 0; i < length; i++) {
        plaintext[i] = ciphertext[i] ^ keyStream[i];
    }
}

void byteToHex(unsigned char byte, char* output) {
    const char hexChars[] = "0123456789abcdef";
    output[0] = hexChars[(byte >> 4) & 0xF];
    output[1] = hexChars[byte & 0xF];
}

void printHex(const unsigned char* bytes, int length, const char* label) {
    std::cout << label << " (hex): ";
    char hex[3];
    hex[2] = '\0';
    
    for (int i = 0; i < length; i++) {
        byteToHex(bytes[i], hex);
        std::cout << hex << " ";
    }
    std::cout << std::endl;
}
void printChars(const unsigned char* bytes, int length, const char* label) {
    std::cout << label << " (text): ";
    for (int i = 0; i < length; i++) {
        if (bytes[i] >= 32 && bytes[i] <= 126) { 
            std::cout << bytes[i];
        } else {
            std::cout << '.'; 
        }
    }
    std::cout << std::endl;
}

int main() {
    const char* key = "encryptionkey";
    std::cout << "Key: " << key << std::endl;
    const char* plaintext = "Hanoi University of Science and Technology ";
    int plaintextLength = strlen(plaintext);
    std::cout << "Plaintext: " << plaintext << std::endl;
    rc4_setup(key);
    
    unsigned char keyStream[plaintextLength];
    generateKeyStream(keyStream, plaintextLength);
    
    unsigned char ciphertext[plaintextLength];
    encrypt(plaintext, ciphertext, plaintextLength);
    std::cout << "--------\n";
    printHex(keyStream, plaintextLength, "Keystream");
    printChars(keyStream, plaintextLength, "Keystream");
    printHex(ciphertext, plaintextLength, "Ciphertext");
    printChars(ciphertext, plaintextLength, "Ciphertext");
    char decryptedText[plaintextLength + 1]; 
    decrypt(ciphertext, decryptedText, plaintextLength);
    decryptedText[plaintextLength] = '\0'; 
    std::cout << "--------\n";
    std::cout << "Decrypted text: " << decryptedText << std::endl;
    
    return 0;
}
