/**
 * @file bbb_pwm.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-10
 */

enum bbb_pwm_state_e {
	BPS_FREE = 0,
	BPS_BUSY = 1
};

enum bbb_pwm_return_code_e {
	BPRC_NOT_IMPLEMENTED = -500,
	BPRC_ERROR = -2,
	BPRC_OK = 0
};

struct bbb_pwm_t;
struct bbb_pwm_tuning_t;

enum bbb_pwm_return_code_e bbb_pwm_setup();
enum bbb_pwm_return_code_e bbb_pwm_cleanup();
