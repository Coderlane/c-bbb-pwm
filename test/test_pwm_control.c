/**
 * @file test_pwm_control.c
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-12-22
 */

#include <bbb_pwm_internal.h>

#include "test_macros.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_set_get_duty_cycle();
void test_set_get_period();
void test_set_get_polarity();

void test_invalid_set_duty_cycle();
void test_invalid_set_period();
void test_invalid_set_polarity();

void test_invalid_get_duty_cycle();
void test_invalid_get_period();
void test_invalid_get_polarity();

struct bbb_pwm_t* bbb_pwm_test_new(const char* name, 
		int touch_files, int init_files);
void bbb_pwm_test_delete(struct bbb_pwm_t** bp_ptr);

int 
main() 
{
	test_set_get_duty_cycle();
	test_set_get_period();
	test_set_get_polarity();

	test_invalid_set_duty_cycle();
	test_invalid_set_period();
	test_invalid_set_polarity();

	test_invalid_get_duty_cycle();
	test_invalid_get_period();
	test_invalid_get_polarity();

	
}

void
test_set_get_duty_cycle()
{
	uint32_t duty = 0;
	struct bbb_pwm_t* bp;
	bp = bbb_pwm_test_new("test_set_get_duty_cycle", 1, 1);

	expect_eq(bbb_pwm_claim(bp), BPRC_OK);
	
	expect_eq(bbb_pwm_set_duty_cycle(bp, 100), BPRC_OK);
	expect_eq(bbb_pwm_get_duty_cycle(bp, &duty), BPRC_OK);
	expect_eq(duty, 100);

	expect_eq(bbb_pwm_set_duty_cycle(bp, 0), BPRC_OK);
	expect_eq(bbb_pwm_get_duty_cycle(bp, &duty), BPRC_OK);
	expect_eq(duty, 0);

	expect_eq(bbb_pwm_set_duty_cycle(bp, 10), BPRC_OK);
	expect_eq(bbb_pwm_get_duty_cycle(bp, &duty), BPRC_OK);
	expect_eq(duty, 10);

	bbb_pwm_test_delete(&bp);
	
}

void
test_set_get_period()
{
	uint32_t period = 0;
	struct bbb_pwm_t* bp;
	bp = bbb_pwm_test_new("test_set_get_period", 1, 1);

	expect_eq(bbb_pwm_claim(bp), BPRC_OK);

	expect_eq(bbb_pwm_set_period(bp, 100), BPRC_OK);
	expect_eq(bbb_pwm_get_period(bp, &period), BPRC_OK);
	expect_eq(period, 100);

	expect_eq(bbb_pwm_set_period(bp, 0), BPRC_OK);
	expect_eq(bbb_pwm_get_period(bp, &period), BPRC_OK);
	expect_eq(period, 0);

	expect_eq(bbb_pwm_set_period(bp, 10), BPRC_OK);
	expect_eq(bbb_pwm_get_period(bp, &period), BPRC_OK);
	expect_eq(period, 10);

	bbb_pwm_test_delete(&bp);
	
}

void
test_set_get_polarity()
{
	int8_t polarity = 0;
	struct bbb_pwm_t* bp;
	bp = bbb_pwm_test_new("test_set_get_polarity", 1, 1);

	expect_eq(bbb_pwm_claim(bp), BPRC_OK);

	expect_eq(bbb_pwm_set_polarity(bp, 1), BPRC_OK);
	expect_eq(bbb_pwm_get_polarity(bp, &polarity), BPRC_OK);
	expect_eq(polarity, 1);

	expect_eq(bbb_pwm_set_polarity(bp, -1), BPRC_OK);
	expect_eq(bbb_pwm_get_polarity(bp, &polarity), BPRC_OK);
	expect_eq(polarity, -1);

	bbb_pwm_test_delete(&bp);
	
}

void
test_invalid_set_duty_cycle()
{
	struct bbb_pwm_t* bp;
	bp = bbb_pwm_test_new("test_invalid_set_duty_cycle", 1, 1);

	// Didn't claim.
	expect_neq(bbb_pwm_set_duty_cycle(bp, 10), BPRC_OK);
	expect_eq(bbb_pwm_claim(bp), BPRC_OK);
	
	// Too big.
	expect_neq(bbb_pwm_set_duty_cycle(bp, 101), BPRC_OK);

	bbb_pwm_test_delete(&bp);
	
}

void 
test_invalid_set_period()
{
	struct bbb_pwm_t* bp;
	bp = bbb_pwm_test_new("test_invalid_set_period", 1, 1);

	// Didn't claim.
	expect_neq(bbb_pwm_set_period(bp, 10), BPRC_OK);
	expect_eq(bbb_pwm_claim(bp), BPRC_OK);

	bbb_pwm_test_delete(&bp);
	
}

void 
test_invalid_set_polarity()
{
	struct bbb_pwm_t* bp;
	bp = bbb_pwm_test_new("test_invalid_set_polarity", 1, 1);

	// Didn't claim.
	expect_neq(bbb_pwm_set_polarity(bp, 10), BPRC_OK);
	expect_eq(bbb_pwm_claim(bp), BPRC_OK);

	// Too small
	expect_neq(bbb_pwm_set_polarity(bp, -2), BPRC_OK);
	// Too big
	expect_neq(bbb_pwm_set_polarity(bp, 2), BPRC_OK);
	// 0
	expect_neq(bbb_pwm_set_polarity(bp, 0), BPRC_OK);

	bbb_pwm_test_delete(&bp);
	
}

void 
test_invalid_get_duty_cycle()
{
	uint32_t duty = 0;
	struct bbb_pwm_t* bp;
	bp = bbb_pwm_test_new("test_invalid_set_duty_cycle", 1, 0);

	// Didn't claim, which is ok, but bad data.
	expect_neq(bbb_pwm_get_duty_cycle(bp, &duty), BPRC_OK);
	
	bbb_pwm_test_delete(&bp);
}

void 
test_invalid_get_period()
{
	uint32_t period = 0;
	struct bbb_pwm_t* bp;
	bp = bbb_pwm_test_new("test_invalid_set_period", 1, 0);

	// Didn't claim, which is ok, but bad data.
	expect_neq(bbb_pwm_get_period(bp, &period), BPRC_OK);
	
	bbb_pwm_test_delete(&bp);
}

void 
test_invalid_get_polarity()
{
	int8_t polarity = 0;
	struct bbb_pwm_t* bp;
	bp = bbb_pwm_test_new("test_invalid_set_polarity", 1, 0);

	// Didn't claim, which is ok, but bad data.
	expect_neq(bbb_pwm_get_polarity(bp, &polarity), BPRC_OK);
	
	bbb_pwm_test_delete(&bp);
}

/**
 * @brief 
 *
 * @param name
 * @param touch_files
 * @param init_files
 *
 * @return 
 */
struct bbb_pwm_t*
bbb_pwm_test_new(const char* name, int touch_files, int init_files)
{
	struct bbb_pwm_t* bp;
	char path[1024];

	if(getcwd(path, sizeof(path)) == NULL) {
		fprintf(stderr, "Error getting cwd!\n");
		exit(errno);
	}	
	
	bp = bbb_pwm_new(name, path);
	assert(bp != NULL);
	
	if(touch_files) {
		FILE *fp;
		
		// Touch the duty file and init if necessary.
		fp = fopen(bp->bp_duty_file_path, "w");
		if(init_files) {
			fprintf(fp, "%d", 0);
		}
		assert(fp != NULL);
		fclose(fp);

		// Touch the period file and init if necessary.
		fp = fopen(bp->bp_period_file_path, "w");
		assert(fp != NULL);
		if(init_files) {
			fprintf(fp, "%d", 20000000);
		}
		fclose(fp);

		// Touch the polarity file and init if necessary.
		fp = fopen(bp->bp_polarity_file_path, "w");
		assert(fp != NULL);
		if(init_files) {
			fprintf(fp, "%d", 1);
		}
		fclose(fp);
	}

	return bp;
}

/**
 * @brief 
 *
 * @param bp_ptr
 */
void
bbb_pwm_test_delete(struct bbb_pwm_t** bp_ptr)
{
	struct bbb_pwm_t* bp;
	assert(bp_ptr != NULL);

	bp = *bp_ptr;
	if(bp == NULL) {
		return;
	}
	
	bbb_pwm_unclaim(bp);

	// Delete the files if they existed.
	remove(bp->bp_duty_file_path);
	remove(bp->bp_period_file_path);
	remove(bp->bp_polarity_file_path);

	bbb_pwm_delete(bp_ptr);
}
