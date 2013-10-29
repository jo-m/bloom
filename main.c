#include <stdio.h>
#include <stdlib.h>
#include "MurmurHash3.h"
#include "bloom.h"
#include <string.h>

int main ()
{
  bloom_data b;
  char *str1 = "This is a test string.";
  char *str2 = "This is another test string.";
  bloom_init(&b, 32);
  bloom_insert(&b, str1, strlen(str1));

  if(bloom_lookup(&b, str1, strlen(str1)))
    printf("str1 found!\n");

  if(!bloom_lookup(&b, str2, strlen(str1)))
    printf("str2 not found!\n");

  return EXIT_SUCCESS;
}
