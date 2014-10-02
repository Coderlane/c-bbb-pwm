/**
 * @file pulser.c
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-01
 */

#include <pulser_internal.h>

/**
 * @brief 
 *
 * @param on
 * @param off
 * @param low_length
 * @param high_length
 *
 * @return 
 */
struct pse_pulser_t *
pse_pulser_new(pulser_loop loop, void *loop_arg) {
	struct pse_pulser_t *pulser;

	pulser = malloc(sizeof(struct pse_pulser_t));
	assert(pulser != NULL);

	pulser->pp_loop = loop;
	pulser->pp_loop_arg = loop_arg;

	pthread_mutex_init(&(pulser->pp_running_mutex), NULL);

	return pulser;
}

/**
 * @brief 
 *
 * @param pp_ptr
 */
void 
pse_pulser_delete(struct pse_pulser_t **pp_ptr) {
	struct pse_pulser_t *pp;

	if(pp_ptr == NULL) {
		return;
	}

	pp = (*pp_ptr);
	if(pp == NULL) {
		return;
	}

	pse_pulser_stop(pp);
	free(pp);
	(*pp_ptr) = NULL;
}

/**
 * @brief 
 *
 * @param pp
 *
 * @return 
 */
int 
pse_pulser_start(struct pse_pulser_t *pp) {
	int result = 0;

	assert(pp != NULL);

	pthread_mutex_lock(&(pp->pp_running_mutex));
	if(pp->pp_running == true) {
		result = -1;
		goto out;
	}

	pthread_create(&(pp->pp_thread), NULL, 
			pse_pulser_run, pp); 
	pp->pp_running = true;
out:
	pthread_mutex_unlock(&(pp->pp_running_mutex));
	return result;
}

/**
 * @brief 
 *
 * @param pp
 *
 * @return 
 */
int 
pse_pulser_stop(struct pse_pulser_t *pp) {
	assert(pp != NULL);

	pthread_mutex_lock(&(pp->pp_running_mutex));
	pp->pp_running = false;
	pthread_mutex_unlock(&(pp->pp_running_mutex));
	return 0;
}

void *
pse_pulser_run(void *pp_void) {
	struct pse_pulser_t *pp = pp_void;
	assert(pp != NULL);

	// run the main loop
	while(pp->pp_running) {
		pp->pp_loop(pp->pp_loop_arg);
	}
	// Thread is done.
	pthread_exit(pp_void);
}

/**
 * @brief 
 *
 * @param on
 * @param off
 *
 * @return 
 */
struct pse_pwm_pulser_t *
pse_pwm_pulser_new(pulser_on_signal_t on, void *on_arg,
		pulser_off_signal_t off, void *off_arg) {
	return pse_pwm_pulser_new_long(on, on_arg, off, off_arg,
			PSE_PWMP_DEFAULT_LOW_LENGTH, PSE_PWMP_DEFAULT_HIGH_LENGTH);
}

/**
 * @brief 
 *
 * @param on
 * @param off
 * @param low_length
 * @param high_length
 *
 * @return 
 */
struct pse_pwm_pulser_t *
pse_pwm_pulser_new_long(pulser_on_signal_t on, void *on_arg,
		pulser_off_signal_t off, void *off_arg,
		float low_length, float high_length) {
	struct pse_pwm_pulser_t *pulser = NULL;

	pulser = malloc(sizeof(struct pse_pwm_pulser_t));
	assert(pulser != NULL);

	pulser->pwmp_pulser = pse_pulser_new(pse_pwm_pulser_loop, pulser);
	assert(pulser->pwmp_pulser != NULL);

	pulser->pwmp_on_signal = on;
	pulser->pwmp_on_arg = on_arg;
	pulser->pwmp_off_signal = off;
	pulser->pwmp_off_arg = off_arg;

	pulser->pwmp_low_length = msec_to_usec(low_length);
	pulser->pwmp_high_length = msec_to_usec(high_length);

	return pulser;
}

/**
 * @brief 
 *
 * @param pwmp
 *
 * @return 
 */
int 
pse_pwm_pulser_start(struct pse_pwm_pulser_t *pwmp) {
	pse_pulser_start(pwmp->pwmp_pulser);
	return 0;
}

/**
 * @brief 
 *
 * @param pwmp
 *
 * @return 
 */
int 
pse_pwm_pulser_stop(struct pse_pwm_pulser_t *pwmp) {
	pse_pulser_start(pwmp->pwmp_pulser);
	return 0;
}

void pse_pwm_pulser_loop(void *pwmp_void) {
	struct pse_pwm_pulser_t *pwmp = pwmp_void;
	assert(pwmp);


	// CHANGE TO SIMPLE BOOL FOR BOTH CASES
	// THEN ADD MUTEX INTERNALLY TO PREVENT DOUBLE WRITE
	if(pwmp->pwmp_signal_length != pwmp->pwmp_new_signal_length) {
		// Assign new value
		pwmp->pwmp_signal_length = pwmp->pwmp_new_signal_length;
		// Update old value
		pwmp->pwmp_new_signal_length = pwmp->pwmp_signal_length;
		// Determine delay
		pwmp->pwmp_delay_length =
		 	pwmp->pwmp_cycle_length - pwmp->pwmp_signal_length;
	}

	// Turn on signal
	pwmp->pwmp_on_signal(pwmp->pwmp_on_arg);
	// Sleep until we need to turn it off again
	usleep(pwmp->pwmp_signal_length);
	// Turn signal to OFF	
	pwmp->pwmp_off_signal(pwmp->pwmp_off_arg);

	// Sleep until the next cycle.
	usleep(pwmp->pwmp_delay_length);
}

//int pse_pwm_pulser_set_percent(struct pse_pwm_pulser_t *pwmp, float percent);

int
pse_pwm_pulser_set_length(struct pse_pwm_pulser_t *pwmp, float length) {
	assert(pwmp != NULL);

	if(length < pwmp->pwmp_low_length) {
		return PSE_PWMP_TOO_LOW;
	}

	if(length > pwmp->pwmp_high_length) {
		return PSE_PWMP_TOO_HIGH;
	}
	
	pwmp->pwmp_new_signal_length = msec_to_usec(length);
	
	return 0;
}







/**
 * @brief 
 *
 * @param msec
 *
 * @return 
 */
uint16_t
msec_to_usec(float msec) {
	return (msec * 1000);
}
