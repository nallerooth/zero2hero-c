#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "common.h"
#include "parse.h"

void list_employees(struct dbheader_t *dbhdr, struct employee_t *employees) {

}

int add_employee(struct dbheader_t *dbhdr, struct employee_t *employees, char *addstring) {

}

int read_employees(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut) {

}

int output_file(int fd, struct dbheader_t *dbhdr, struct employee_t *employees) {
	if (fd < 0) {
		printf("Invalid file descriptor\n");
		return STATUS_ERROR;
	}
	
	// Convert fields to network byte order before writing
	struct dbheader_t header_net = {0};
	header_net.version = htons(dbhdr->version);
	header_net.count = htons(dbhdr->count);
	header_net.filesize = htonl(dbhdr->filesize);
	header_net.magic = htonl(dbhdr->magic);
	
	lseek(fd, 0, SEEK_SET);
	if (write(fd, &header_net, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)) {
		perror("Unable to write db header to file");
		return STATUS_ERROR;
	}
	
	return STATUS_SUCCESS;
}	

int validate_db_header(int fd, struct dbheader_t **headerOut) {
	if (fd < 0) {
		printf("Invalid file descriptor\n");
		return STATUS_ERROR;
	}

	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
	if (header == NULL) {
		printf("Calloc failed allocating memory for 1 dbheader_t\n");
		return STATUS_ERROR;
	}
	
	if (read(fd, header, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)) {
		printf("Wanted to read %lu bytes\n", sizeof(struct dbheader_t));
		perror("Unable to read db header from file");
		free(header);
		return STATUS_ERROR;
	}
	// Convert fields from network byte order to host byte order
	header->version = ntohs(header->version);
	header->count = ntohs(header->count);
	header->filesize = ntohl(header->filesize);
	header->magic = ntohl(header->magic);

	if (header->magic != HEADER_MAGIC) {
		printf("Invalid database file (bad magic value)\n");
		free(header);
		return STATUS_ERROR;
	}

	if (header->version != 0x1) {
		printf("Unsupported database version: %u\n", header->version);
		free(header);
		return STATUS_ERROR;
	}

	struct stat dbstat = {0};
	if (fstat(fd, &dbstat) == -1) {
		perror("fstat failed");
		free(header);
		return STATUS_ERROR;
	}
	if (dbstat.st_size != header->filesize) {
		printf("Database file size mismatch (expected %u, got %ld bytes)\n", header->filesize, dbstat.st_size);
		free(header);
		return STATUS_ERROR;
	}

	*headerOut = header;
}

int create_db_header(struct dbheader_t **headerOut) {
	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
	if (header == NULL) {
		printf("Calloc failed allocating memory for 1 dbheader_t\n");
		return STATUS_ERROR;
	}
	header->version = 0x1;
	header->count = 0;
	header->magic = HEADER_MAGIC;
	header->filesize = sizeof(struct dbheader_t);
	
	*headerOut = header;

	return STATUS_SUCCESS;
}
