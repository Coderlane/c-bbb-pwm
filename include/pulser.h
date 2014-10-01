/**
 * @file pulser.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-01
 */

#ifndef PULSER_H
#define PULSER_H

#include <stdint.h>
#include <stdlib.h>

#include <assert.h>
#include <time.h>
#include <pthread.h>

typedef int (*pulser_on_signal_t)(void *input);
typedef int (*pulser_off_signal_t)(void *input);

/**
 * @brief 
 */
#define PWMP_DEFAULT_HIGH_LENGTH 2.0f

/**
 * @brief 
 */
#define PWMP_DEFAULT_LOW_LENGTH 1.0f

struct pwm_pulser_t {
	/**
	 * @brief 
	 */
	float 							pwmp_high_length;

	/**
	 * @brief 
	 */
	float 							pwmp_low_length;

	/**
	 * @brief 
	 */
	pthread_t	 					pwmp_thread;
	
	/**
	 * @brief 
	 */
	pulser_on_signal_t 		pwmp_on_signal;

	/**
	 * @brief 
	 */
	pulser_off_signal_t 	pwmp_off_signal;
};

struct pwm_pulser_t *pwm_pulser_new(pulser_on_signal_t on, 
		pulser_off_signal_t off);
struct pwm_pulser_t *pwm_pulser_new_long(pulser_on_signal_t on, 
		pulser_off_signal_t off, float low_length, float high_length);

int pwm_pulser_start(struct pwm_pulser_t *pwmp);
int pwm_pulser_stop(struct pwm_pulser_t *pwmp);

#endif /* PULSER_H */
