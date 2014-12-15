/**
 * @file bbb_pwm_internal.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-10
 */

#include <bbb_pwm.h>

#include <libudev.h>

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
struct bbb_pwm_library_t {
	/**
	 * @brief 
	 */
	char 							 *bpl_cape_path;

	/**
	 * @brief 
	 */
	char 							 *bpl_ocp_path;

	struct bbb_pwm_t 		bpl_pwms[BPC_NUM_PWMS];

	struct udev 			 *bpl_udev;
};

struct bbb_pwm_library_t* bbb_pwm_library_new();

int bbb_pwm_library_init(struct bbb_pwm_library_t *bpl);
void bbb_pwm_library_delete(struct bbb_pwm_library_t **bpl_ptr);
