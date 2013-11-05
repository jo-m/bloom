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
  bloom_init(&b, BLOOM_MIN_LEN);
  bloom_insert(&b, str1, strlen(str1));
  bloom_insert(&b, "123323123123", 10);
  bloom_insert(&b, "123dsf23123123", 10);
  bloom_insert(&b, "12334123123123", 10);

  if(bloom_lookup(&b, str1, strlen(str1)))
    printf("str1 found!\n");

  if(!bloom_lookup(&b, str2, strlen(str1)))
    printf("str2 not found!\n");

  printf("%f\n", bloom_false_pos_prob(&b));
  printf("%llu\n", bloom_num_bits_set(&b));

  return EXIT_SUCCESS;
}
