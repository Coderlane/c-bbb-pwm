/**
 * @file bbb_pwm.c
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-10
 */

#include <bbb_pwm_internal.h>

/**
 * @brief Create a new controller.
 *
 * @return A new controller, or NULL on failure.
 */
struct bbb_pwm_controller_t* 
bbb_pwm_controller_new()
{
	struct bbb_pwm_controller_t* bpc = NULL;
	
	bpc = calloc(sizeof(struct bbb_pwm_controller_t), 1);
	assert(bpc != NULL);
	
	if(bbb_pwm_controller_init(bpc) != BPRC_OK) {
		bbb_pwm_controller_delete(&bpc);
	}
	
	return bpc;
}

/**
 * @brief Free a controller.
 *
 * @param bpc_ptr The controller to free.
 */
void 
bbb_pwm_controller_delete(struct bbb_pwm_controller_t **bpc_ptr) 
{
	struct bbb_pwm_controller_t *bpc;
	// Check the ptr.
	if(bpc_ptr == NULL) {
		return;
	}
	// Check the referenced ptr.
	bpc = (*bpc_ptr);
	if(bpc == NULL) {
		return;
	}

	// Free the origional bpc
	free(bpc);
	(*bpc_ptr) = NULL;
}

/**
 * @brief Initializes a bbb pwm controller.
 *
 * @param bpc The bbb pwm controller to initialize.
 *
 * @return A status code.
 */
int
bbb_pwm_controller_init(struct bbb_pwm_controller_t *bpc)
{
	bpc->bpc_capemgr = bbb_capemgr_new();
	if(bpc->bpc_capemgr == NULL) {
		return BPRC_NO_CAPEMGR;
	}	
	
	if(bbb_capemgr_enable(bpc->bpc_capemgr, "am33xx_pwm") < 0) {
		return BPRC_NO_PWM;
	}

	return BPRC_OK;
}

/**
 * @brief Gets a pointer to a specific pwm unit.
 *
 * @param bpc The controller.
 * @param pwm_id The ID of the pwm unit to get.
 *
 * @return The pwm unit found, or NULL if out of range.
 */
const struct bbb_pwm_t* 
bbb_pwm_controller_get(struct bbb_pwm_controller_t* bpc, int pwm_id)
{
	assert(bpc != NULL);
	if(pwm_id > bpc->bpc_num_pwms || pwm_id < 0) {
		return NULL;
	}
	return &(bpc->bpc_pwms[pwm_id]);
}
