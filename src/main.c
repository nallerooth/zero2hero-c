#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
  printf("Usage: %s -n -f <filename.db>\n", argv[0]);
  printf("\t -n  -  Create new database file\n");
  printf("\t -f  -  (required) Path to database file\n");
  return;
}

int main(int argc, char *argv[]) {
  int c;
  bool newfile = false;
  char *filepath = NULL;

  int dbfd = -1;

  struct dbheader_t *dbheader = NULL;

  while ((c = getopt(argc, argv, "nf:")) != -1) {
    switch (c) {
    case 'n':
      newfile = true;
      break;
    case 'f':
      filepath = optarg;
      break;
    case '?':
      printf("Unknown option -%c\n", c);
      break;

    default:
      return -1;
    }
  }

  if (filepath == NULL) {
    fprintf(stderr, "Filepath is required\n");
    print_usage(argv);

    return -1;
  }

  if (newfile) {
    dbfd = create_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Unable to create new file\n");
      return -1;
    }
	
	if (create_db_header(dbfd, &dbheader) == STATUS_ERROR) {
		printf("Unable to create db header\n");
		return -1;
	}

	if (output_file(dbfd, dbheader, NULL) == STATUS_ERROR) {
		printf("Unable to write db header to file\n");
		return -1;
	}
  } else {
    dbfd = open_db_file(filepath);
    if (dbfd == STATUS_ERROR) {
      printf("Unable to open file\n");
      return -1;
    }

	if (validate_db_header(dbfd, &dbheader) == STATUS_ERROR) {
		printf("Invalid db header\n");
		return -1;
	}
  }

  printf("Newfile: %d\n", newfile);
  printf("Filepath: %s\n", filepath);
}
