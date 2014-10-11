/**
 * @file bbb_pwm.h
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-10
 */


enum bbb_pwm_status_e {
	BPL_STATUS_NOT_IMPLEMENTED = -500,
	BPL_STATUS_ERROR = -2,
	BPL_STATUS_OK = 0
};

struct bbb_pwm_t;
struct bbb_pwm_tuning_t;
struct bbb_pwm_controller_t;

struct bbb_pwm_controller_t* bbb_pwm_controller_new();
void bbb_pwm_controller_delete(struct bbb_pwm_controller_t **bpc_ptr);
