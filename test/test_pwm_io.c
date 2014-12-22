
#include <bbb_pwm_internal.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_rw_uint32();
void test_rw_int8();

int 
main() 
{

	test_rw_int8();
	test_rw_uint32();
}

void 
test_rw_uint32()
{
	uint32_t data = 0;
	char iofilepath[1024];
	FILE *iofile = NULL;

	if(getcwd(iofilepath, sizeof(iofilepath)) == NULL) {
		fprintf(stderr, "Error getting cwd!\n");
		exit(errno);
	}	
	
	assert(strlen(iofilepath) + strlen("uint32") < sizeof(iofilepath));
	strcat(iofilepath, "/uint32.txt");

	iofile = fopen(iofilepath, "w+");
	assert(iofile != NULL);

	write_uint32_to_file(iofile, UINT32_MAX);
	read_uint32_from_file(iofile, &data);
	fprintf(stderr, "Written: %d Read: %d\n", UINT32_MAX, data);
	assert(data == UINT32_MAX);

	write_uint32_to_file(iofile, 0);
	read_uint32_from_file(iofile, &data);
	fprintf(stderr, "Written: %d Read: %d\n", 0, data);
	assert(data == 0);

	fclose(iofile);
	if(remove(iofilepath) != 0) {
		fprintf(stderr, "Error removing test file: %s\n", iofilepath);
		exit(errno);
	}
}

void test_rw_int8()
{
	int8_t data = 0;
	char iofilepath[1024];
	FILE *iofile = NULL;

	if(getcwd(iofilepath, sizeof(iofilepath)) == NULL) {
		fprintf(stderr, "Error getting cwd!\n");
		exit(errno);
	}	
	
	assert(strlen(iofilepath) + strlen("int8") < sizeof(iofilepath));
	strcat(iofilepath, "/int8.txt");

	iofile = fopen(iofilepath, "w+");
	assert(iofile != NULL);

	write_int8_to_file(iofile, INT8_MIN);
	read_int8_from_file(iofile, &data);
	fprintf(stderr, "Written: %d Read: %d\n", INT8_MIN, data);
	assert(data == INT8_MIN);

	write_int8_to_file(iofile, INT8_MAX);
	read_int8_from_file(iofile, &data);
	fprintf(stderr, "Written: %d Read: %d\n", INT8_MAX, data);
	assert(data == INT8_MAX);

	write_int8_to_file(iofile, 0);
	read_int8_from_file(iofile, &data);
	fprintf(stderr, "Written: %d Read: %d\n", 0, data);
	assert(data == 0);

	fclose(iofile);
	if(remove(iofilepath) != 0) {
		fprintf(stderr, "Error removing test file: %s\n", iofilepath);
		exit(errno);
	}
}
