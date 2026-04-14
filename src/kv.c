#include <string.h>
#include <stdlib.h>

#include <kv.h>

size_t hash(const char *key, size_t capacity) {
	size_t hash = 0x13371337deadbeef;

	while (*key) {
		hash ^= *key;
		hash = hash << 8;
		hash += *key;

		key++;
	}
	
	return hash % capacity;
}


kv_t *kv_init(size_t capacity) {
  if (capacity == 0) {
    return NULL;
  }

  kv_t *table = malloc(sizeof(kv_t));
  if (table == NULL) {
    return NULL;
  }

  table->capacity = capacity;
  table->count = 0;

  table->entries = calloc(capacity, sizeof(kv_entry_t));
  if (table->entries == NULL) {
    free(table);
    return NULL;
  }

  return table;
}

int kv_put(kv_t *table, const char *key, const char *value) {
	if (!table || !key || !value) {
		return -1;
	}

	size_t idx = hash(key, table->capacity);

	for (size_t i = 0; i < table->capacity-1; i++) {
		size_t real_idx = (idx + i) % table->capacity;

		kv_entry_t *entry = &table->entries[real_idx];

		// Key already exists, update value
		if (entry->key && entry->key != TOMBSTONE && !strcmp(entry->key, key)) {

			char *new_value = strdup(value);
			if (!new_value) { return -1; }
			entry->value = new_value;

			return real_idx;
		}

		// Empty slot, insert new entry
		if (!entry->key || entry->key == TOMBSTONE) {
			char *new_value = strdup(value);
			char *new_key = strdup(key);

			if (!new_value || !new_key) { 
				free(new_key);
				free(new_value);
				return -1; 
			}
			entry->key = new_key;
			entry->value = new_value;
			table->count++;

			return real_idx;
		}
	}
	
	// Table is full
	return -1;
}

