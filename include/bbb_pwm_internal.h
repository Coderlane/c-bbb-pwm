/**
 * @file bbb_pwm_internal.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-10
 */


#ifndef BBB_PWM_INTERNAL_H
#define BBB_PWM_INTERNAL_H

#include <bbb_pwm.h>
#include <bbb_capemgr.h>

#include <libudev.h>

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief The number of pwms onboard.
 */
//#define BPC_NUM_PWMS 4

struct bbb_pwm_t {
	enum bbb_pwm_state_e    bp_state;
	char 									 *bp_name;
	struct bbb_pwm_t		 	 *bp_next;
};

/**
 * @brief 
 */
struct bbb_pwm_controller_t {
	/**
	 * @brief The number of pwms detected.
	 */
	int 										bpc_num_pwms;

	/**
	 * @brief The individual pwms.
	 */
	struct bbb_pwm_t 	 		 *bpc_head_pwm;

	/**
	 * @brief The capemanager.
	 */
	struct bbb_capemgr_t 	 *bpc_capemgr;
};

int bbb_pwm_controller_init(struct bbb_pwm_controller_t* bpc);
int bbb_pwm_controller_add_pwm(struct bbb_pwm_controller_t* bpc,
		struct bbb_pwm_t* bp);
int bbb_pwm_controller_remove_pwm(struct bbb_pwm_controller_t* bpc,
	 	const char* name);

struct bbb_pwm_t* bbb_pwm_new(const char* name);
void bbb_pwm_delete(struct bbb_pwm_t** bp_ptr);
#endif
