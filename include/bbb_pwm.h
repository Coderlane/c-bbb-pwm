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
	BPS_FREE = 0,
	BPS_BUSY = 1
};

enum bbb_pwm_return_code_e {
	BPRC_NOT_IMPLEMENTED = -100,
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

const struct bbb_pwm_t* 
bbb_pwm_controller_get(struct bbb_pwm_controller_t* bpc, int pwm_id);


int bbb_pwm_is_busy(struct bbb_pwm_t* bp);
int bbb_pwm_is_free(struct bbb_pwm_t* bp);

#endif
