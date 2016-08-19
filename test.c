#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <stdint.h>
#include <ctype.h>

#include "base64.h"

void test1() {
  printf("-------------------- test1 --------------------\n");
  
  char *str = "The problem resolver";

  printf("orig=%s\n", str);

  char * enc = base64_encode(str);

  printf("enc =%s\n", enc);

  char * dec = base64_decode(enc);
  
  printf("dec =%s\n", dec);

  assert(strcmp(str,dec)==0);
}

void test2() {
  printf("-------------------- test2 --------------------\n");

  char * enc = "VGhlIHB\ty\nb 2!@#$%^&*()_JsZW0gcmVzb2x2ZXI=\n";

  printf("enc =%s\n", enc);

  char * dec = base64_decode(enc);
  
  printf("dec =%s\n", dec);

  assert(strcmp(dec,"The problem resolver")==0);
}

int main(int argc, char **argv) {

  test1();
  test2();
  
  return 0;
}

