#ifndef _BLOOM_H_
#define _BLOOM_H_

#include "MurmurHash3.h"
#include <stdbool.h>

#define HASH_SEED 0x4F91B57E

typedef struct {
  unsigned char *data;
  unsigned int  index_len;
  uint64_t      data_len;
} bloom_data;

bool bloom_init(bloom_data *bloom, unsigned int len);
void bloom_free(bloom_data *bloom);
void bloom_insert(bloom_data *bloom, char *data, int data_len);
bool bloom_lookup(bloom_data *bloom, char *data, int data_len);

#endif
