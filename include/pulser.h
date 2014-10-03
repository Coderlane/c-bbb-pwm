/**
 * @file pulser.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-01
 */

#ifndef PSE_PULSER_H
#define PSE_PULSER_H

#include <stdbool.h>
#include <stdint.h>


typedef int (*pulser_on_signal_t)(void *input);
typedef int (*pulser_off_signal_t)(void *input);

/**
 * @brief 
 */
#define PSE_PWMP_DEFAULT_HIGH_LENGTH 2.0f

/**
 * @brief 
 */
#define PSE_PWMP_DEFAULT_LOW_LENGTH 1.0f

/**
 * @brief 
 */
#define PSE_PWMP_DEFAULT_CYCLE_LENGTH 20.0f

/**
 * @brief 
 */
struct pse_pwm_pulser_t;

/**
 * @brief 
 */
struct pse_ppm_pulser_t;

enum pse_status {
	/**
	 * @brief 
	 */
	PSE_OK = 0
};

enum pse_pwmp_status {
	/**
	 * @brief 
	 */
	PSE_PWMP_OK = PSE_OK,
	/**
	 * @brief 
	 */
	PSE_PWMP_TOO_LOW = -1,
	/**
	 * @brief 
	 */
	PSE_PWMP_TOO_HIGH = -2
};

struct pse_pwm_pulser_t *pse_pwm_pulser_new(
		pulser_on_signal_t on, void *on_arg, 
		pulser_off_signal_t off, void *off_arg);

struct pse_pwm_pulser_t *pse_pwm_pulser_new_long(
		pulser_on_signal_t on, void *on_arg, 
		pulser_off_signal_t off, void *off_arg,
		float low_length, float high_length);

int pse_pwm_pulser_start(struct pse_pwm_pulser_t *pwmp);
int pse_pwm_pulser_stop(struct pse_pwm_pulser_t *pwmp);
//int pse_pwm_pulser_set_percent(struct pse_pwm_pulser_t *pwmp, float percent);
int pse_pwm_pulser_set_signal_length(struct pse_pwm_pulser_t *pwmp, 
		float length);
int pse_pwm_pulser_set_cycle_length(struct pse_pwm_pulser_t *pwmp, 
		float length); 
#endif /* PULSER_H */
