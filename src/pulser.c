/**
 * @file pulser.c
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-01
 */

#include <pulser.h>

struct pwm_pulser_t *
pwm_pulser_new(pulser_on_signal_t on, pulser_off_signal_t off) {
	return pwm_pulser_new_long(on, off, 
			PWMP_DEFAULT_LOW_LENGTH, PWMP_DEFAULT_HIGH_LENGTH);
}

struct pwm_pulser_t *
pwm_pulser_new_long(pulser_on_signal_t on, pulser_off_signal_t off,
		float low_length, float high_length) {
	struct pwm_pulser_t *pulser = NULL;

	pulser = malloc(sizeof(struct pwm_pulser_t));

	assert(pulser != NULL);

	pulser->pwmp_on_signal = on;
	pulser->pwmp_off_signal = off;
	pulser->pwmp_low_length 	= low_length;
	pulser->pwmp_high_length 	= high_length;

	return pulser;
}

int pwm_pulser_start(struct pwm_pulser_t *pwmp) {

	return 0;
}

int pwm_pulser_stop(struct pwm_pulser_t *pwmp) {

	return 0;
}
