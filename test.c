#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <stdint.h>
#include <ctype.h>

#include "base64.h"

int main(int argc, char **argv) {
  char *str;
  
  str = "The problem resolver.";

  printf("orig=%s\n", str);

  char * enc = base64_encode(str);

  printf("enc =%s\n", enc);

  char * dec = base64_decode(enc);
  
  printf("dec =%s\n", dec);
  
  return 0;
}
