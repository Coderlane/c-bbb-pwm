/**
 * @file bbb_pwm_internal.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-10
 */


#ifndef BBB_PWM_INTERNAL_H
#define BBB_PWM_INTERNAL_H

#define _GNU_SOURCE

#include <bbb_pwm.h>

#include <libudev.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/**
 * @brief The number of pwms onboard.
 */
//#define BPC_NUM_PWMS 4

struct bbb_pwm_t {
	uint8_t				bp_id;
	
};

/**
 * @brief 
 */
struct bbb_pwm_controller_t {
	/**
	 * @brief The number of pwms detected.
	 */
	int 								bpc_num_pwms;

	/**
	 * @brief The individual pwms.
	 */
	struct bbb_pwm_t 	 *bpc_pwms;

	/**
	 * @brief The udev reference.
	 */
	struct udev 			 *bpc_udev;
};

int bbb_pwm_controller_init(struct bbb_pwm_controller_t *bpc);
char *find_device_syspath(struct udev *probe_udev, char *sysattr, char *value);
char *push_path(char *base, char *other);

#endif
