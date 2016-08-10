/*

Base64 encoder/decoder


char * base64_encode(char *str);
--------------------------------
The `str` must be char* which points a sequence of positive char values < 128.
base64_encode returns char* of a Base64 encoded string which may be compliant with
https://en.wikipedia.org/wiki/Base64#RFC_4648 as possible,
i.e., It perhaps satisfy the following:

The char for index 62 = '+'
The char for index 63 = '/'
The pad char = '=' (mandatory)
no CR/LF

Memory of the value is obtained with malloc() and have to be freed with free() after you use it.


char * base64_decode(char *str);
--------------------------------
The `str` must be char* of a Base64 encoded string, and its length must be multiple of 4.
Otherwise, it will die.
base64_decode returns char* of a Base64 decoded string.
Memory of the value is obtained with malloc() and have to be freed with free() after you use it.

 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <stdint.h>
#include <ctype.h>

#include "base64.h"

char *
base64_encode(char *str)
{
  char *base64_enc_map = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  char *pstr = str;
  char *enc = malloc(((strlen(str)*4/3 + 3) & ~0x03) + 1);
  char *penc = enc;
  
  int bits = 0x00000000;
  
  
  for (; *pstr; pstr++) {
    assert(0 < *pstr);
    assert(*pstr < 128);

    bits = bits << 8 | *pstr;
    if ((uintptr_t)(pstr - str) % 3 == 0) {
      *penc++ = base64_enc_map[(bits >> 2) & 0x3f];
    }
    else if ((uintptr_t)(pstr - str) % 3 == 1) {
      *penc++ = base64_enc_map[(bits >> 4) & 0x3f];
    }
    else if ((uintptr_t)(pstr - str) % 3 == 2) {
      *penc++ = base64_enc_map[(bits >> 6) & 0x3f];
      *penc++ = base64_enc_map[bits & 0x3f];
    }
  }

  // edge process
  if ((uintptr_t)(pstr - str) % 3 == 1) {
    *penc++ = base64_enc_map[(bits << 4) & 0x3f];
    *penc++ = '=';
    *penc++ = '=';
  }
  else if ((uintptr_t)(pstr -str) % 3 == 2) {
    *penc++ = base64_enc_map[(bits << 2) & 0x3f];
    *penc++ = '=';
  }

  *penc = 0;

  assert((uintptr_t)(penc - enc) % 4 == 0);
  
  return enc;
}

char *
base64_decode(char *str)
{
  char *pstr = str;
  char *dec = malloc(strlen(str)*3/4 + 1);
  char *pdec = dec;
  
  int bits;
  
  while(1) {
    if (*pstr == '\0') break;
    
    bits = (base64_dec_map(*pstr++) << 18)
      | (base64_dec_map(*pstr++) << 12)
      | (base64_dec_map(*pstr++) << 6)
      | base64_dec_map(*pstr++);
    
    *pdec++ = (bits & 0xff0000) >> 16;
    *pdec++ = (bits & 0x00ff00) >> 8;
    *pdec++ = (bits & 0x0000ff);
  }
  *pdec = '\0';

  return dec;
}

char base64_dec_map(char c) {
  if (isupper(c)) return c - 'A';
  if (islower(c)) return c - 'a' + 26;
  if (isdigit(c)) return c - '0' + 26 + 26;
  if (c == '+') return 62;
  if (c == '/') return 63;
  if (c == '=') return 0;
  assert(0);
}

