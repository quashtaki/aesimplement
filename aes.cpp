#include <fstream>
#include <iostream>
#include <iomanip>
unsigned int polyDiv(unsigned int n, unsigned int d);
unsigned char subBytes(unsigned char byte);
unsigned char msb(unsigned int n);
unsigned char polyMult(unsigned char x, unsigned char y);
unsigned char dumbInverse(unsigned char n);
int main(int argc, char* argv[]) {   
    unsigned char key[16] = {};
    unsigned char block[4][4];
    std::ifstream is("aes_sample.in", std::ios::binary | std::ios::in);
    is.read(reinterpret_cast<char*>(key), 16);
    is.read(reinterpret_cast<char*>(block[0]), 4);
    is.read(reinterpret_cast<char*>(block[1]), 4);
    is.read(reinterpret_cast<char*>(block[2]), 4);
    is.read(reinterpret_cast<char*>(block[3]), 4);
    for(int i = 0; i < 16; i++) {
        std::cout << std::setw(2) << std::setfill('0') << std::hex <<(int)key[i]; 
    }
    std::cout << std::endl;
    for(int i = 0; i < 4 ; i++) {
        for(int j = 0; j < 4 ; j++) {
            std::cout << std::setw(2) << std::setfill('0') << std::hex <<(int) block[i][j]; 
        }
    }
    std::cout << std::endl;


    std::cout << std::setw(2) << std::setfill('0') << std::hex <<(int)dumbInverse(0x53) << std::endl;
    std::cout << std::setw(2) << std::setfill('0') << std::hex <<(int)subBytes(0x53) << std::endl;
    return 0;
}

unsigned char dumbInverse(unsigned char n){ //only 2^8 so its fine :)
    int res = 0;
    unsigned char q = 0;
    for(;res != 1; q++) {
        res = polyMult(n,q);
    }
    return q-1;
}

unsigned char subBytes(unsigned char byte){
    unsigned char inv = dumbInverse(byte);
   // unsigned char mask = 1;
   // unsigned char c = 0x63;
   // unsigned char ci, bi, bi4, bi5, bi6, bi7, newbyte, newbi;
   // for(int i = 0; i < 8; i++){
   //     mask <<= i;
   //     bi = (byte & mask) >> i;
   //     bi4 = (byte & (1 << (i + 4) % 8)) >> ((i+4)%8);
   //     bi5 = (byte & (1 << (i + 5) % 8)) >> ((i+5)%8);
   //     bi6 = (byte & (1 << (i + 6) % 8)) >> ((i+6)%8);
   //     bi7 = (byte & (1 << (i + 7) % 8)) >> ((i+7)%8);
   //     ci = (c & mask) >> i;
   //     newbi = (bi ^ bi4 ^ bi5 ^ bi6 ^ bi7 ^ ci);
   //     newbyte ^= (-newbi ^ newbyte) & (1UL << i);        
   // }
   // return newbyte;
    return ((polyMult(inv,31) % 257) ^ 99);
}

unsigned char polyMult(unsigned char x, unsigned char y) {
    unsigned char a = x, b = y, p = 0;
    bool carry;
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
    if (d == 0) exit(1);
    unsigned int a = n, b = d;
    while (b != 0) {
        b = d << (msb(a) - msb(b));
        a = a ^ b;
        std::cout<< "a: " << a << "b: " << b << std::endl;
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
        std::cout<<"123" << std::endl;
    return msb;
}
