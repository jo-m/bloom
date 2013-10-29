#include "bloom.h"
#include "stdlib.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

uint64_t bloom_mask_integer(uint64_t i, int len) {
  if(len == 0) {
    return 0;
  }
  return (i << (64 - len)) >> (64 - len);
}

bool bloom_init(bloom_data *bloom, unsigned int len) {
  assert(len > 4);
  bloom->index_len = len;
  bloom->data_len = ((uint64_t)1) << (len - 3);
  bloom->data = malloc(bloom->data_len);
  memset(bloom->data, 0, bloom->data_len);
  if(bloom->data == NULL) {
    return false;
  }
  return true;
}

void bloom_free(bloom_data *bloom) {
  bloom->data_len = 0;
  bloom->index_len = 0;
  free(bloom->data);
}

void bloom_calc_index(bloom_data *bloom, char *data, int data_len, int *mem_index, int *bit_index) {
  char hash_out[16] = {0};
  uint64_t index;
  MurmurHash3_x64_128(data, data_len, HASH_SEED, hash_out);
  index = bloom_mask_integer(*((uint64_t*)hash_out), bloom->index_len);
  *mem_index = index >> 3;
  *bit_index = (int)(index & 0x07);
}

void bloom_insert(bloom_data *bloom, char *data, int data_len) {
  int bit_index, mem_index;
  bloom_calc_index(bloom, data, data_len, &mem_index, &bit_index);

  bloom->data[mem_index] |= (unsigned char)(1 << bit_index);
}

bool bloom_lookup(bloom_data *bloom, char *data, int data_len) {
  int bit_index, mem_index;
  bloom_calc_index(bloom, data, data_len, &mem_index, &bit_index);

  return (bloom->data[mem_index] & (unsigned char)(1 << bit_index)) >> bit_index;
}
