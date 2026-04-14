#ifndef KV_H
#define KV_H

#include <stdlib.h>

typedef struct {
  char *key;
  char *value;
} kv_entry_t;

typedef struct {
  kv_entry_t *entries;
  size_t count;
  size_t capacity;
} kv_t;

kv_t *kv_init(size_t capacity);

#endif
