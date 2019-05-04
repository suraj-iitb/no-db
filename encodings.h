#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef char byte;

typedef union {
    long long ll;
    byte bytes[8];
} LongBytes;


int EncodeLong(long long l, byte *bytes);
long long DecodeLong(byte *bytes);