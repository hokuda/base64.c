#define NOT_A_BASE64_CHAR 0x100

int base64_dec_map(char c);
char * base64_encode(char *str);
char * base64_decode(char *str);
