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
#include <bbb_capemgr.h>

#include <libudev.h>

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>

/**
 * @brief Represents a detected pwm.
 */
struct bbb_pwm_t {
	
	/**
	 * @brief The current state of the pwm.
	 */
	enum bbb_pwm_state_e    bp_state;

	/**
	 * @brief The name of this pwm.
	 */
	char 									 *bp_name;

	/**
	 * @brief The cached pwm duty cycle. 
	 */
	uint32_t 									bp_duty_cycle;

	/**
	 * @brief The cached pwm period (in nanoseconds).
	 */
	uint32_t 									bp_period;

	/**
	 * @brief The cached pwm polarity.
	 */
	int8_t 										bp_polarity;

	/**
	 * @brief The file path of the duty file.
	 */
	char 									 *bp_duty_file_path;

	/**
	 * @brief The file path of the period file.
	 */
	char 									 *bp_period_file_path;

	/**
	 * @brief The file path of the polarity file.
	 */
	char 									 *bp_polarity_file_path;

	/**
	 * @brief The file with the pwm's duty data.
	 */
	FILE 									 *bp_duty_file;

	/**
	 * @brief The file with the pwm's period data.
	 */
	FILE 									 *bp_period_file;

	/**
	 * @brief The file with the pwm's polarity data.
	 */
	FILE 									 *bp_polarity_file;

	/**
	 * @brief The next PWM in the list of PWMs, NULL signifies the end.
	 */
	struct bbb_pwm_t		 	 *bp_next;
};

/**
 * @brief A controller enables/disables pwm functionality.
 * It also detects pwms and makes them avaliable for use.
 */
struct bbb_pwm_controller_t {
	
	/**
	 * @brief The number of pwms detected.
	 */
	int8_t 									bpc_num_pwms;

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
int bbb_pwm_controller_probe(struct bbb_pwm_controller_t* bpc);

struct bbb_pwm_t* bbb_pwm_new(const char* name, const char* root_path);
void bbb_pwm_delete(struct bbb_pwm_t** bp_ptr);

int read_uint32_from_file(FILE* file, uint32_t* out_data);
int read_int8_from_file(FILE* file, int8_t* out_data);

int write_uint32_to_file(FILE* file, uint32_t data);
int write_int8_to_file(FILE* file, int8_t data);

#endif /* BBB_PWM_INTERNAL_H */
