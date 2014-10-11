/**
 * @file tunable_pwm_internal.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-05
 */


#ifndef TUNABLE_PWM_INTERNAL_H
#define TUNABLE_PWM_INTERNAL_H

#include <tunable_pwm.h>
#include <stdint.h>

struct tpwm_library_t {
	


};


/**
 * @brief Settings for a tunable pwm controller.
 */
struct tpwm_controller_settings_t {

	/**
	 * @brief The high duty length.
	 */
	int16_t 	tpwmcs_duty_high;

	/**
	 * @brief The stop duty length.
	 */
	int16_t 	tpwmcs_duty_stop;

	/**
	 * @brief The low duty length.
	 */
	int16_t 	tpwmcs_duty_low;

	/**
	 * @brief The cycle length.
	 */
	uint16_t 	tpwmcs_cycle;
};

/**
 * @brief A controller of a pwm source.
 */
struct tpwm_controller_t {
	
	/**
	 * @brief Settings related to the controller.
	 */
	struct tpwm_controller_settings_t tpmpc_settings;

	/**
	 * @brief The scaled percentage of the duty cycle.
	 */
	int16_t tpwmc_duty_percent;
};

#endif /* TUNABLE_PWM_INTERNAL_H */
