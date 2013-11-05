#ifndef _BLOOM_H_
#define _BLOOM_H_

#include "MurmurHash3.h"
#include <stdbool.h>

#define BLOOM_HASH_SEED_1 0x4F91B57E
#define BLOOM_HASH_SEED_2 0x3f0e2b41
#define BLOOM_HASH_SEED_3 0x81c467ef
#define BLOOM_HASH_SEED_4 0xc62e3a92
#define BLOOM_MIN_LEN 16

typedef struct {
  unsigned char *data;
  unsigned int  index_len;
  uint64_t      data_len;
} bloom_data;

bool  bloom_init(bloom_data *bloom, unsigned int len);
void  bloom_free(bloom_data *bloom);
void  bloom_insert(bloom_data *bloom, char *data, int data_len);
bool  bloom_lookup(bloom_data *bloom, char *data, int data_len);
uint64_t bloom_num_bits_set(bloom_data *bloom);
double bloom_false_pos_prob(bloom_data *bloom);

#endif
