/**
 * @file test_pwm_control.c
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-12-22
 */

#include <bbb_pwm_internal.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_sg_duty_cycle();
void test_sg_period();
void test_sg_polarity();

int 
main() 
{
	test_sg_duty_cycle();
	test_sg_period();
	test_sg_polarity();

	return 0;
}

void
test_sg_duty_cycle()
{
//	struct bbb_pwm_t* bp;
//	bp = bbb_pwm_new("test", "
}

void
test_sg_period()
{

}

void
test_sg_polarity()
{

}

struct bbb_pwm_t*
bbb_pwm_test_new(const char* name)
{
	struct bbb_pwm_t* bp;
	char path[1024];

	if(getcwd(path, sizeof(path)) == NULL) {
		fprintf(stderr, "Error getting cwd!\n");
		exit(errno);
	}	

	bp = bbb_pwm_new(name, path);
	assert(bp != NULL);
	return bp;
}

/*
int bbb_pwm_set_duty_cycle(struct bbb_pwm_t* bp, uint32_t duty);
int bbb_pwm_set_period(struct bbb_pwm_t* bp, uint32_t period);
int bbb_pwm_set_polarity(struct bbb_pwm_t* bp, int8_t polarity);

int bbb_pwm_get_duty_cycle(struct bbb_pwm_t* bp, uint32_t* out_duty);
int bbb_pwm_get_period(struct bbb_pwm_t* bp, uint32_t* out_period);
int bbb_pwm_get_polarity(struct bbb_pwm_t* bp, int8_t* out_polarity);
*/

