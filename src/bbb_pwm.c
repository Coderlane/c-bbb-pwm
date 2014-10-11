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
struct bbb_pwm_controller_t* bbb_pwm_controller_new() {

	// NOT IMPLEMNTED.
	return NULL;
}

/**
 * @brief Free a controller.
 *
 * @param bpc_ptr The controller to free.
 */
void bbb_pwm_controller_delete(struct bbb_pwm_controller_t **bpc_ptr) {
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
	// Free associated memory.
	if(bpc->bpc_cape_path != NULL) {
		free(bpc->bpc_cape_path);
		bpc->bpc_cape_path = NULL;
	}	

	if(bpc->bpc_ocp_path != NULL) {
		free(bpc->bpc_ocp_path);
		bpc->bpc_ocp_path = NULL;
	}

	// Free the origional bpc
	free(bpc);
	(*bpc_ptr) = NULL;
}
