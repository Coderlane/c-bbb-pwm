/**
 * @file pulser_internal.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-01
 */

#ifndef PSE_PULSER_INTERNAL_H
#define PSE_PULSER_INTERNAL_H

#include <pulser.h>

#include <stdatomic.h>
#include <stdlib.h>

#include <assert.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

typedef void (*pulser_loop)(void *input);
/**
 * @brief 
 */
struct pse_pulser_t {
	/**
	 * @brief 
	 */
	atomic_bool						pp_running;

	/**
	 * @brief 
	 */
	pthread_mutex_t       pp_running_mutex;

	/**
	 * @brief 
	 */
	pthread_t	 						pp_thread;

	/**
	 * @brief 
	 */
	pulser_loop      		 	pp_loop;

	/**
	 * @brief 
	 */
	void  	   					 *pp_loop_arg;
};

/**
 * @brief 
 */
struct pse_pwm_pulser_t {
	/**
	 * @brief 
	 */
	uint16_t 							pwmp_high_length;

	/**
	 * @brief 
	 */
	uint16_t 							pwmp_low_length;

	/**
	 * @brief 
	 */
	uint16_t							pwmp_signal_length;

	/**
	 * @brief 
	 */
	uint16_t							pwmp_new_signal_length;

	/**
	 * @brief This is the remaining time to delay until emitting the next set.
	 * pwmp_delay_length = pwmp_cycle_length - pwmp_signal_length
	 */
	uint16_t							pwmp_delay_length;

	/**
	 * @brief 
	 */
	uint16_t 							pwmp_cycle_length;

	/**
	 * @brief 
	 */
	uint16_t		 					pwmp_new_cycle_length;

	/**
	 * @brief 
	 */
	atomic_bool						pwmp_update_timing;

	/**
	 * @brief 
	 */
	pthread_mutex_t 			pwmp_update_timing_mutex;

	/**
	 * @brief 
	 */
	pulser_on_signal_t    pwmp_on_signal;

	/**
	 * @brief 
	 */
	void                 *pwmp_on_arg;

	/**
	 * @brief 
	 */
	pulser_off_signal_t   pwmp_off_signal;

	/**
	 * @brief 
	 */
	void                 *pwmp_off_arg;

	/**
	 * @brief 
	 */
	struct pse_pulser_t  *pwmp_pulser;
};

/**
 * @brief 
 */
struct pse_ppm_pulser_t {
	/**
	 * @brief 
	 */
	uint16_t 							ppmp_high_length;

	/**
	 * @brief 
	 */
	uint16_t 							ppmp_low_length;


	/**
	 * @brief 
	 */
	pulser_on_signal_t    ppmp_on_signal;

	/**
	 * @brief 
	 */
	void                 *ppmp_on_arg;

	/**
	 * @brief 
	 */
	pulser_off_signal_t   ppmp_off_signal;

	/**
	 * @brief 
	 */
	void                 *ppmp_off_arg;

	/**
	 * @brief 
	 */
	struct pse_pulser_t  *ppmp_pulser;
};

// PULSER
struct pse_pulser_t *pse_pulser_new(pulser_loop loop, void *loop_arg);
void pse_pulser_delete(struct pse_pulser_t **pp_ptr);

int pse_pulser_start(struct pse_pulser_t *pp);
int pse_pulser_stop(struct pse_pulser_t *pp);

void *pse_pulser_run(void *pp_void);

// PWM_PULSER
void pse_pwm_pulser_loop(void *pwmp);
void pse_pwm_pulser_default_dynamic_lengths(struct pse_pwm_pulser_t *pwmp);

// GENERIC
uint16_t msec_to_usec(float msec);

#endif /* PSE_PULSER_INTERNAL_H */
