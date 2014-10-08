/**
 * @file tunable_pwm.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-05
 */

#ifndef TUNABLE_PWM_H
#define TUNABLE_PWM_H

enum tpwm_duty_limit_e {
	TPWM_DUTY_MAX_PERCENT = 100,
	TPWM_DUTY_MIN_PERCENT = -100 
};

enum tpwmcs_duty_defaults_e {
	TPWMCS_DUTY_DEFAULT_HIGH = 20000,
	TPWMCS_DUTY_DEFAULT_OFF = 10000,
	TPWMCS_DUTY_DEFAULT_LOW = 5000
};

struct tpwm_controller_t;
struct tpwm_controller_settings_t;

#endif /* TUNABLE_PWM_H */
