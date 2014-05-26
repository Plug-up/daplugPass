#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <daplug/common.h>


typedef unsigned char Byte;

char* DAPLUGAPI DAPLUGCALL str_sub (const char *, int, int);

void DAPLUGAPI DAPLUGCALL strToBytes(const char *str, Byte *bytes_array);
void DAPLUGAPI DAPLUGCALL bytesToStr(Byte *bytes,int bytes_s,char *str);
int DAPLUGAPI DAPLUGCALL isHexInput(const char *input);
void DAPLUGAPI DAPLUGCALL asciiToHex(const char *s,char* s_hex);
int DAPLUGAPI DAPLUGCALL hexToAscii(const char *s_hex,char* s);

#ifdef __cplusplus
}
#endif

#endif // UTILS_H_INCLUDED
