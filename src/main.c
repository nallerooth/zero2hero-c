#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include <kv.h>

void print_usage(char *argv[]) {
  printf("Usage: %s -n -f <filename.db>\n", argv[0]);
  printf("\t -n  -  Create new database file\n");
  printf("\t -f  -  (required) Path to database file\n");
  return;
}

int main(int argc, char *argv[]) {
  kv_t *table = kv_init(3);
  printf("%p\n", table);
  printf("%ld\n", table->capacity);

  kv_put(table, "hehe", "haha");

  for (size_t i = 0; i < table->capacity; i++) {
    printf("[%ld] %s : %s\n", i, table->entries[i].key,
           table->entries[i].value);
  }

  return 0;
}
