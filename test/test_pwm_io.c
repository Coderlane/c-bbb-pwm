
#include <bbb_pwm_internal.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_rw_uint32();
void test_rw_int8();

void test_invalid_r_uint32();
void test_invalid_r_int8();

void test_invalid_w_uint32();
void test_invalid_w_int8();

FILE* open_testfile(const char* name, const char* mode);
void close_testfile(FILE* file);

struct test_file* test_file_new(const char* name, const char* mode);
void test_file_delete(struct test_file** tfp);

struct test_file {
	FILE 				 *tf_fp;
	char   				tf_path[1024];
	const char 	 *tf_mode;
};

int 
main() 
{
	test_rw_int8();
	test_rw_uint32();
	
	test_invalid_r_uint32();
	test_invalid_r_int8();

	test_invalid_w_uint32();
	test_invalid_w_int8();
}

void 
test_rw_uint32()
{
	uint32_t data = 0;
	struct test_file* tf;
	tf = test_file_new("/uint32.txt", "w+");

	write_uint32_to_file(tf->tf_fp, UINT32_MAX);
	read_uint32_from_file(tf->tf_fp, &data);
	fprintf(stderr, "Written: %d Read: %d\n", UINT32_MAX, data);
	assert(data == UINT32_MAX);

	write_uint32_to_file(tf->tf_fp, 0);
	read_uint32_from_file(tf->tf_fp, &data);
	fprintf(stderr, "Written: %d Read: %d\n", 0, data);
	assert(data == 0);

	test_file_delete(&tf);
}

void 
test_rw_int8()
{
	int8_t data = 0;
	struct test_file* tf;
	tf = test_file_new("/int8.txt", "w+");

	write_int8_to_file(tf->tf_fp, INT8_MIN);
	read_int8_from_file(tf->tf_fp, &data);
	fprintf(stderr, "Written: %d Read: %d\n", INT8_MIN, data);
	assert(data == INT8_MIN);

	write_int8_to_file(tf->tf_fp, INT8_MAX);
	read_int8_from_file(tf->tf_fp, &data);
	fprintf(stderr, "Written: %d Read: %d\n", INT8_MAX, data);
	assert(data == INT8_MAX);

	write_int8_to_file(tf->tf_fp, 0);
	read_int8_from_file(tf->tf_fp, &data);
	fprintf(stderr, "Written: %d Read: %d\n", 0, data);
	assert(data == 0);

	test_file_delete(&tf);
}


void 
test_invalid_r_uint32()
{
	uint32_t data = 0;
	struct test_file* tf;
	tf = test_file_new("/uint32.txt", "w+");

	// Test NULL ptrs.
	assert(read_uint32_from_file(NULL, &data) != BPRC_OK); 
	assert(read_uint32_from_file(tf->tf_fp, NULL) != BPRC_OK);

	// Test read from empty file.
	assert(read_uint32_from_file(tf->tf_fp, &data) != BPRC_OK);

	test_file_delete(&tf);
}

void 
test_invalid_r_int8()
{
	int8_t data = 0;
	struct test_file* tf;
	tf = test_file_new("/int8.txt", "w+");

	// Test NULL ptrs.
	assert(read_int8_from_file(NULL, &data) != BPRC_OK); 
	assert(read_int8_from_file(tf->tf_fp, NULL) != BPRC_OK);

	// Test read from empty file.
	assert(read_int8_from_file(tf->tf_fp, &data) != BPRC_OK);

	test_file_delete(&tf);
}

void 
test_invalid_w_uint32()
{
	//struct test_file* tf;
	//tf = test_file_new("/uint32.txt", "r");

	// Test NULL ptr.
	assert(write_uint32_to_file(NULL, 0) != BPRC_OK);

	// Test write to read only file.
	//assert(write_uint32_to_file(tf->tf_fp, 0) != BPRC_OK);

	//test_file_delete(&tf);
}

void 
test_invalid_w_int8()
{
	//struct test_file* tf;
	//tf = test_file_new("/int8.txt", "r");

	// Test NULL ptr.
	assert(write_int8_to_file(NULL, 0) != BPRC_OK);

	// Test write to read only file.
	//assert(write_int8_to_file(tf->tf_fp, 0) != BPRC_OK);

	//test_file_delete(&tf);
}

/**
 * @brief 
 *
 * @param name
 * @param mode
 *
 * @return 
 */
struct test_file* 
test_file_new(const char* name, const char* mode)
{
	struct test_file* tf;
 
	assert(name != NULL);
	assert(mode != NULL);
		
	tf = malloc(sizeof(struct test_file));
	assert(tf != NULL);
	
	if(getcwd(tf->tf_path, sizeof(tf->tf_path)) == NULL) {
		fprintf(stderr, "Error getting cwd!\n");
		exit(errno);
	}	

	assert((strlen(tf->tf_path) + strlen(name)) < sizeof(tf->tf_path));
	strcat(tf->tf_path, name);
	
	tf->tf_mode = mode;
	fprintf(stderr, "File: %s Mode: %s\n", tf->tf_path, tf->tf_mode);
	tf->tf_fp = fopen(tf->tf_path, tf->tf_mode);

	assert(tf->tf_fp != NULL);

	return tf;
}

/**
 * @brief 
 *
 * @param tfp
 */
void 
test_file_delete(struct test_file** tfp)
{
	struct test_file* tf;

	assert(tfp != NULL);
	tf = *tfp;

	if(tf->tf_fp != NULL) {
		fclose(tf->tf_fp);
		tf->tf_fp = NULL;
	}

	remove(tf->tf_path);

	free(tf);
	*tfp = NULL;
}
