/**
 * @file bbb_pwm.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-10
 */


enum bbb_pwm_status_e {
	BPS_NOT_IMPLEMENTED = -500,
	BPS_ERROR = -2,
	BPS_OK = 0
};

struct bbb_pwm_t;
struct bbb_pwm_tuning_t;

enum bbb_pwm_status_e bbb_pwm_setup();
enum bbb_pwm_status_e bbb_pwm_cleanup();
