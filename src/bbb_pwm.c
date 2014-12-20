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

int 
bbb_pwm_controller_probe(struct bbb_pwm_controller_t* bpc)
{


	return BPRC_NOT_IMPLEMENTED;
}

/**
 * @brief 
 *
 * @param bpc
 * @param bp
 *
 * @return 
 */
int 
bbb_pwm_controller_add_pwm(struct bbb_pwm_controller_t* bpc, 
		struct bbb_pwm_t* bp)
{
	struct bbb_pwm_t* cur = NULL;
	int result;

	assert(bpc != NULL);
	assert(bp != NULL);

	cur = bpc->bpc_head_pwm;
	if(cur == NULL) {
		// New list.
		bpc->bpc_head_pwm = bp;
		return BPRC_OK;
	}

	result = strcmp(cur->bp_name, bp->bp_name);

	if(result == 0) {
		// Front was duplicate.
		return BPRC_DUPLICATE;
	}

	if(result < 0) {
		// Insert in front.
		bp->bp_next = cur;
		bpc->bpc_head_pwm = bp;
		return BPRC_OK;
	}

	while(cur->bp_next != NULL) {
		result = strcmp(cur->bp_next->bp_name, bp->bp_name);

		if(result == 0) {
			// Found a duplicate item.
			return BPRC_DUPLICATE;
		}

		if(result < 0) {
			// Found our place.
			bp->bp_next = cur->bp_next;
			cur->bp_next = bp;
			return BPRC_OK;
		}
	}
	// Insert at end.
	cur->bp_next = bp;
	return BPRC_OK;
}

/**
 * @brief 
 *
 * @param bpc
 * @param name
 *
 * @return 
 */
int 
bbb_pwm_controller_remove_pwm(struct bbb_pwm_controller_t* bpc,
		const char* name)
{
	struct bbb_pwm_t* cur;
	int result;

	cur = bpc->bpc_head_pwm;
	result = strcmp(cur->bp_name, name);

	if(result == 0) {
		if(!bbb_pwm_is_unclaimed(cur)) {
			// Make sure it isn't locked.
			return BPRC_BUSY;
		}
		// First item was the one to delete.
		bpc->bpc_head_pwm = cur->bp_next;
		bbb_pwm_delete(&cur);
	}

	if(result < 0) {
		return BPRC_PWM_NOT_FOUND;
	}

	while(cur != NULL) {
		result = strcmp(cur->bp_name, name);

		if(result == 0) {
			if(!bbb_pwm_is_unclaimed(cur)) {
				// Make sure it isn't locked.
				return BPRC_BUSY;
			}
			// First item was the one to delete.
			bpc->bpc_head_pwm = cur->bp_next;
			bbb_pwm_delete(&cur);
		}

		if(result < 0) {
			return BPRC_PWM_NOT_FOUND;
		}
		// Move on.
		cur = cur->bp_next;
	}

	return BPRC_PWM_NOT_FOUND;
}

/**
 * @brief Creates a new bbb_pwm_t.
 *
 * @param name The name of the new pwm.
 *
 * @return A new pwm or NULL on failure.
 */
struct bbb_pwm_t*
bbb_pwm_new(const char* name) 
{
	struct bbb_pwm_t* bp;
	if(name == NULL) {
		return NULL;
	}

	bp = malloc(sizeof(struct bbb_pwm_t));
	assert(bp != NULL);

	bp->bp_next = NULL;
	bp->bp_state = BPS_UNCLAIMED;
	bp->bp_name = (char*) strdup(name);
	assert(bp->bp_name != NULL);

	return bp;
}

/**
 * @brief Deletes a PWM.
 * Will free if not already free
 *
 * @param bp_ptr The pwm to delete.
 */
void
bbb_pwm_delete(struct bbb_pwm_t** bp_ptr) 
{
	struct bbb_pwm_t* bp;
	assert(bp_ptr != NULL);

	bp = *bp_ptr;
	if(bp == NULL) {
		return;
	}
	
	// Only for debugging.
	assert(bbb_pwm_is_unclaimed(bp));

	if(bp->bp_name != NULL) {
		free(bp->bp_name);
	}

	free(bp);
	*bp_ptr = NULL;	
}

/**
 * @brief Check to see if the pwm is busy.
 *
 * @param bp The pwm to check.
 *
 * @return True/False is the pwm busy.
 */
int 
bbb_pwm_is_busy(struct bbb_pwm_t* bp)
{
	assert(bp != NULL);
	if(bp->bp_state == BPS_CLAIMED) {
		// We claim it
		return 0;
	}
	// TODO: Check the files to see if they are in use by someone else.
	return 0;	
}

/**
 * @brief Check to see if the pwm is unclaimed.
 *
 * @param bp The pwm to check.
 *
 * @return True/False is the pwm unclaimed.
 */
int 
bbb_pwm_is_unclaimed(struct bbb_pwm_t* bp)
{
	assert(bp != NULL);
	return bp->bp_state == BPS_UNCLAIMED;
}

/**
 * @brief Check to see if we have claimership of the pwm.
 *
 * @param bp The pwm to check.
 *
 * @return True/False if we have claimership.
 */
int
bbb_pwm_is_claimed(struct bbb_pwm_t* bp)
{
	assert(bp != NULL);
	return bp->bp_state == BPS_CLAIMED;
}
