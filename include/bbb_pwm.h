/**
 * @file bbb_pwm.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-10
 */

#ifndef BBB_PWM_H
#define BBB_PWM_H

enum bbb_pwm_state_e {
	/**
	 * @brief The pwm is unclaimed.
	 */
	BPS_UNCLAIMED = 0,
	
	/**
	 * @brief The pwm is claimed.
	 */
	BPS_CLAIMED = 1
};

enum bbb_pwm_return_code_e {
	BPRC_NOT_IMPLEMENTED = -100,
	BPRC_BAD_FILE = -57,
	BPRC_RANGE = -56,
	BPRC_NOT_CLAIMED = -55,
	BPRC_BUSY = -54,
	BPRC_PWM_NOT_FOUND = -53,
	BPRC_DUPLICATE = -52,
	BPRC_NO_PWM = -51,
	BPRC_NO_CAPEMGR = -50,
	BPRC_NO_MEM = -3,
	BPRC_ERROR = -2,
	BPRC_OK = 0
};

struct bbb_pwm_t;
struct bbb_pwm_controller_t;

struct bbb_pwm_controller_t* bbb_pwm_controller_new();
void bbb_pwm_controller_delete(struct bbb_pwm_controller_t** bpc_ptr);

int bbb_pwm_claim(struct bbb_pwm_t* bp);
int bbb_pwm_unclaim(struct bbb_pwm_t* bp);

int bbb_pwm_is_unclaimed(struct bbb_pwm_t* bp);
int bbb_pwm_is_claimed(struct bbb_pwm_t* bp);

int bbb_pwm_set_duty(struct bbb_pwm_t* bp, float duty);
int bbb_pwm_set_period(struct bbb_pwm_t* bp, float period);
int bbb_pwm_set_polarity(struct bbb_pwm_t* bp, int polarity);

int bbb_pwm_get_duty(struct bbb_pwm_t* bp, float* out_duty);
int bbb_pwm_get_period(struct bbb_pwm_t* bp, float* out_period);
int bbb_pwm_get_polarity(struct bbb_pwm_t* bp, int* out_polarity);

#endif
