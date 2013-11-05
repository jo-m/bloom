#include "bloom.h"
#include "stdlib.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

uint64_t bloom_mask_integer(uint64_t i, int len) {
  if(len == 0) {
    return 0;
  }
  return (i << (64 - len)) >> (64 - len);
}

bool bloom_init(bloom_data *bloom, unsigned int len) {
  assert(len >= BLOOM_MIN_LEN);
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

void bloom_calc_index(bloom_data *bloom, char *data, int data_len, int *mem_index, int *bit_index, uint32_t seed) {
  char hash_out[16] = {0};
  uint64_t index;
  MurmurHash3_x64_128(data, data_len, seed, hash_out);
  index = bloom_mask_integer(*((uint64_t*)hash_out), bloom->index_len);
  *mem_index = index >> 3;
  *bit_index = (int)(index & 0x07);
}

void bloom_insert_single(bloom_data *bloom, char *data, int data_len, uint32_t seed) {
  int bit_index, mem_index;
  bloom_calc_index(bloom, data, data_len, &mem_index, &bit_index, seed);

  bloom->data[mem_index] |= (unsigned char)(1 << bit_index);
}

void bloom_insert(bloom_data *bloom, char *data, int data_len) {
  bloom_insert_single(bloom, data, data_len, BLOOM_HASH_SEED_1);
  bloom_insert_single(bloom, data, data_len, BLOOM_HASH_SEED_2);
  bloom_insert_single(bloom, data, data_len, BLOOM_HASH_SEED_3);
  bloom_insert_single(bloom, data, data_len, BLOOM_HASH_SEED_4);
}

bool bloom_lookup_single(bloom_data *bloom, char *data, int data_len, uint32_t seed) {
  int bit_index, mem_index;
  bloom_calc_index(bloom, data, data_len, &mem_index, &bit_index, seed);

  return (bloom->data[mem_index] & (unsigned char)(1 << bit_index)) >> bit_index;
}

bool bloom_lookup(bloom_data *bloom, char *data, int data_len) {
  return
    bloom_lookup_single(bloom, data, data_len, BLOOM_HASH_SEED_1) &&
    bloom_lookup_single(bloom, data, data_len, BLOOM_HASH_SEED_2) &&
    bloom_lookup_single(bloom, data, data_len, BLOOM_HASH_SEED_3) &&
    bloom_lookup_single(bloom, data, data_len, BLOOM_HASH_SEED_4);
}

int bloom_num_bits_set_int(uint32_t i) {
  i = i - ((i >> 1) & 0x55555555);
  i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
  return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

uint64_t bloom_num_bits_set(bloom_data *bloom) {
  uint64_t total = 0;
  uint64_t i;
  for(i = 0; i < bloom->data_len; i += 4) {
    unsigned char val2 = bloom->data[i];
    uint32_t *val = (uint32_t*)&bloom->data[i];
    total += bloom_num_bits_set_int(*val);
  }
  return total;
}

double bloom_false_pos_prob(bloom_data *bloom) {
  double bits_set = (double)bloom_num_bits_set(bloom);
  double bits_total = (double)bloom->data_len * 8;
  return pow(bits_set / bits_total, 4);
}
