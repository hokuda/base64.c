/*

Base64 encoder/decoder

*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <stdint.h>
#include <ctype.h>

#include "base64.h"

/*
  returns base64-encoded string which may be compliant with 
  https://en.wikipedia.org/wiki/Base64#RFC_4648 as possible, e.g., char for
  index 62 is '+', char for index 63 = '/', pad char is '=', and it has no
  CR/LF. Memory of the value is obtained with malloc() and have to be freed
  with free().
*/
char *
base64_encode(char *str)
{
  char *base64_enc_map = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  char *pstr = str;
  char *enc = malloc(((strlen(str)*4/3 + 3) & ~0x03) + 1);
  char *penc = enc;
  
  int bits = 0;
  
  
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

  *penc = '\0';

  assert((uintptr_t)(penc - enc) % 4 == 0);
  
  return enc;
}


/* maps base64-char to bits */
int base64_dec_map(char c) {
  if (isupper(c)) return c - 'A';
  if (islower(c)) return c - 'a' + 26;
  if (isdigit(c)) return c - '0' + 26 + 26;
  if (c == '+') return 62;
  if (c == '/') return 63;
  if (c == '=') return 0;
  return NOT_A_BASE64_CHAR;
}


/*
  returns base64-decoded string ignoring non-base64-chars. An effective
  length of str must be multiple of 4, otherwise, it will die. Memory of
  the value is obtained with malloc() and have to be freed with free().
*/
char *
base64_decode(char *str)
{
  char *pstr = str;
  char *dec = malloc(strlen(str)*3/4 + 1);
  char *pdec = dec;
  int skip_count = 0;
  
  int bits;
  
  while(1) {
    
    int i=0;
    while(i<4) {
      if (*pstr == '\0') goto OUT;

      int c = base64_dec_map(*pstr++);
      if (c == NOT_A_BASE64_CHAR) {
        skip_count++;
        continue;
      }
      bits = (bits << 6) | c;
      i++;
    }
    *pdec++ = (bits & 0xff0000) >> 16;
    *pdec++ = (bits & 0x00ff00) >> 8;
    *pdec++ = (bits & 0x0000ff);
  }
 OUT:
  *pdec = '\0';

  assert(((uintptr_t)(pstr - str) - skip_count) % 4 == 0);

  return dec;
}

