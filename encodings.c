#include "encodings.h"

/*
 Naive encoding of a 64-bit long to 8 bytes. 
 */
int EncodeLong(long long l, byte *bytes) {
    LongBytes lb;
    lb.ll = l;
    memcpy(bytes, lb.bytes, 8);
    return 8;
}

/*
  Translates 8 bytes pointed to by 'bytes' to a 64-bit long.
 */
long long DecodeLong(byte *bytes) {
    LongBytes lb;
    memcpy(lb.bytes, bytes, 8);
    return lb.ll;
}