#include <malloc.h>
#include <stdlib.h> 
#include <time.h> 

#include "../include/util/tool.h"

void init_tool(){
  // Mets la graine au temps courrant
  srand(time(NULL));
}

char *random_str(unsigned long len){
  char *s = calloc(sizeof(char), len + 1);

  static const char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
  unsigned long characters_len = sizeof_a(characters);

  for(size_t i = 0; i < len; i++)
    s[i] = characters[rand() % characters_len];

  return s;
}