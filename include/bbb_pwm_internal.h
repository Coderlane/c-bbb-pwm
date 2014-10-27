/**
 * @file bbb_pwm_internal.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-10
 */

#include <bbb_pwm.h>

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @brief The number of pwms onboard.
 */
#define BPC_NUM_PWMS 4


struct bbb_pwm_t {
	uint8_t				bp_id;
};

/**
 * @brief 
 */
struct bbb_pwm_controller_t {
	/**
	 * @brief 
	 */
	char 				*bpc_cape_path;

	/**
	 * @brief 
	 */
	char 				*bpc_ocp_path;

	struct bbb_pwm_t 		bpc_pwms[BPC_NUM_PWMS];
};

struct bbb_pwm_controller_t* bbb_pwm_controller_new();
void bbb_pwm_controller_delete(struct bbb_pwm_controller_t **bpc_ptr);
