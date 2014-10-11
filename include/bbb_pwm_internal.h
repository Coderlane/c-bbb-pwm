/**
 * @file bbb_pwm_internal.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-10
 */

#include <bbb_pwm.h>

#include <stdlib.h>
#include <assert.h>

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
};
