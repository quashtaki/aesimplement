#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

unsigned int polyDiv(unsigned int n, unsigned int d);
unsigned char subBytes(unsigned char byte);
unsigned char msb(unsigned int n);
unsigned char polyMult(unsigned char x, unsigned char y);
unsigned char dumbInverse(unsigned char n);
void addRoundKey(unsigned char state[][4], unsigned char keysched[][4], int round);
void cipher(unsigned char block[][4], unsigned char out[][4],unsigned char keysched[][4]);
void keyExpansion(unsigned char key[], unsigned char words[][4]);
    uint8_t const static sbox[256] = {0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
                        0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
                        0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
                        0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
                        0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
                        0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
                        0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
                        0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
                        0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
                        0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
                        0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
                        0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
                        0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
                        0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
                        0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
                        0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16};
int main(int argc, char* argv[]) {   
    unsigned char key[16] = {0};
    unsigned char block[4][4]; // TODO: Block is 4*4 but should just be 1d-16 array
    read(STDIN_FILENO, key, 16);
   // for(int i = 0; i < 16; i++) {
   //     printf("%02x", key[i]);
   // }
   // printf("\n");
   // printf("%x\n", sbox[0x53]);
    unsigned char keysched[44][4];
    keyExpansion(key, keysched);
    printf("Expanded Key:\n");
    for(int i = 0; i < 44; i++) {
        for(int j = 0; j < 4; j++){
            printf("%02x",keysched[i][j]);
        }
    }
    printf("\n");
    read(STDIN_FILENO, block, 16);
    unsigned char out[4][4];
    cipher(block, out, keysched);
    for(int i = 0; i < 16; i++) {
        printf("%02x", out[i]);
    }
    printf("\n");
    return 0;
}

void cipher(unsigned char block[][4], unsigned char out[][4], unsigned char keysched[][4]){
    unsigned char state[4][4];
    memcpy(state, block, sizeof(block[0])*4);
    addRoundKey(state, keysched, 0);
    memcpy(out,state,16);
}

void addRoundKey(unsigned char state[][4], unsigned char keysched[][4], int round){
    for(int i = 0; i < 4; i++){
        state[i][0] ^= keysched[4*round+i][0];
        state[i][1] ^= keysched[4*round+i][1];
        state[i][2] ^= keysched[4*round+i][2];
        state[i][3] ^= keysched[4*round+i][3];
    }
}

void keyExpansion(unsigned char key[], unsigned char words[][4]){
    unsigned char rc[10] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};
    for(int i = 0; i < 4; i++){
        words[i][0] = key[4*i];
        words[i][1] = key[4*i+1];
        words[i][2] = key[4*i+2];
        words[i][3] = key[4*i+3];
    }
    for(int i = 4; i < 44; i++){
        if (i % 4 == 0) {
            words[i][0] = words[i-4][0] ^ sbox[(words[i-1][1])] ^ rc[(i/4)-1];
            words[i][1] = words[i-4][1] ^ sbox[(words[i-1][2])];      
            words[i][2] = words[i-4][2] ^ sbox[(words[i-1][3])];      
            words[i][3] = words[i-4][3] ^ sbox[(words[i-1][0])];      
        } else {
            words[i][0] = words[i-4][0] ^ words[i-1][0];
            words[i][1] = words[i-4][1] ^ words[i-1][1];
            words[i][2] = words[i-4][2] ^ words[i-1][2];
            words[i][3] = words[i-4][3] ^ words[i-1][3];
        }
    }
}

unsigned char dumbInverse(unsigned char n){ //only 2^8 so its fine :)
    int res = 0;
    unsigned char q = 0;
    for(;res != 1; q++) {
        res = polyMult(n,q);
    }
    return q-1;
}

unsigned char polyMult(unsigned char x, unsigned char y) {
    unsigned char a = x, b = y, p = 0;
    unsigned char carry;
    for (int i = 0; i < 8 && a > 0 && b > 0; i++){
        if (b & 0b1) p ^= a;
        b >>= 1;
        carry = a & 0b10000000;
        a <<= 1;
        if (carry) a ^= 0b00011011;
    }
    return p;
}

unsigned int polyDiv(unsigned int n, unsigned int d) {
    if (d == 0) abort();
    unsigned int a = n, b = d;
    while (b != 0) {
        b = d << (msb(a) - msb(b));
        a = a ^ b;
    }
    return a;
}

unsigned char msb(unsigned int n) {
    if (n == 0) return 0;
    unsigned int msb = 0;
    n = n / 2;
    while (n != 0) {
        n = n/2;
        msb++;
    }
    return msb;
}
