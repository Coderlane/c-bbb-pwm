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
		goto out;
	}

	bbb_pwm_controller_probe(bpc);

out:
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

	while(bpc->bpc_head_pwm) {
		// Unclaim before we remove it.
		bbb_pwm_unclaim(bpc->bpc_head_pwm);
		bbb_pwm_controller_remove_pwm(bpc, bpc->bpc_head_pwm->bp_name);
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
 * @brief Probe the filesystem for obvious PWM devices.
 *
 * @param bpc The controller to probe on.
 *
 * @return A status code.
 */
int 
bbb_pwm_controller_probe(struct bbb_pwm_controller_t* bpc)
{

	assert(bpc != NULL);
	
	return BPRC_NOT_IMPLEMENTED;
}

/**
 * @brief Adds a PWM to a controller. 
 *
 * @param bpc The controller to add to.
 * @param bp The PWM to add.
 *
 * @return A status code.
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

	result = strcmp(bp->bp_name, cur->bp_name);

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
		result = strcmp(bp->bp_name, cur->bp_next->bp_name);

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
		// Increment cur.
		cur = cur->bp_next;
	}
	// Insert at end.
	cur->bp_next = bp;
	return BPRC_OK;
}

/**
 * @brief Removes a PWM from a controller.
 * Note, the PWM must not be in use.
 *
 * @param bpc The comtroller to remove the pwm from.
 * @param name The name of the PWM to remove.
 *
 * @return A status code.
 */
int 
bbb_pwm_controller_remove_pwm(struct bbb_pwm_controller_t* bpc,
		const char* name)
{
	struct bbb_pwm_t* cur;
	int result;

	assert(bpc != NULL);
	assert(name != NULL);

	cur = bpc->bpc_head_pwm;
	if(cur == NULL) {
		return BPRC_PWM_NOT_FOUND;
	}

	result = strcmp(cur->bp_name, name);

	if(result == 0) {
		if(!bbb_pwm_is_unclaimed(cur)) {
			// Make sure it isn't locked.
			return BPRC_BUSY;
		}
		// First item was the one to delete.
		bpc->bpc_head_pwm = cur->bp_next;
		bbb_pwm_delete(&cur);
		bpc->bpc_num_pwms--;
		return BPRC_OK;
	}

	if(result > 0) {
		return BPRC_PWM_NOT_FOUND;
	}

	while(cur->bp_next != NULL) {
		result = strcmp(cur->bp_next->bp_name, name);

		if(result == 0) {
			// Found our target.
			struct bbb_pwm_t* tmp = cur->bp_next;

			if(!bbb_pwm_is_unclaimed(cur)) {
				// Make sure it isn't locked.
				return BPRC_BUSY;
			}

			// Remove it from the linked list.
			cur->bp_next = tmp->bp_next;
			bbb_pwm_delete(&tmp);
			bpc->bpc_num_pwms--;

			return BPRC_OK;
		}

		if(result > 0) {
			return BPRC_PWM_NOT_FOUND;
		}
		// Move on.
		cur = cur->bp_next;
		assert(cur != NULL);
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

	bp = calloc(sizeof(struct bbb_pwm_t), 1);
	assert(bp != NULL);

	bp->bp_state = BPS_UNCLAIMED;
	bp->bp_name = (char*) strdup(name);
	assert(bp->bp_name != NULL);

	return bp;
}

/**
 * @brief Deletes a PWM.
 * This will unclaim if claimed.
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

	// Unclaim, since we are freeing.	
	bbb_pwm_unclaim(bp);	
	
	// Free what has been allocated. 
	if(bp->bp_name != NULL) {
		free(bp->bp_name);
	}

	if(bp->bp_duty_file_path != NULL) {
		free(bp->bp_duty_file_path);
	}

	if(bp->bp_period_file_path != NULL) {
		free(bp->bp_period_file_path);
	}

	if(bp->bp_polarity_file_path != NULL) {
		free(bp->bp_polarity_file_path);
	}

	free(bp);
	*bp_ptr = NULL;	
}

/**
 * @brief Claims a pwm for later setting values.
 * If someone else has claimed this pwm, we fail and report BUSY.
 *
 * @param bp The pwm to claim.
 *
 * @return A status code.
 */
int 
bbb_pwm_claim(struct bbb_pwm_t* bp)
{
	int result = BPRC_OK;

	assert(bp != NULL);
	assert(bp->bp_duty_file_path != NULL);
	assert(bp->bp_period_file_path != NULL);
	assert(bp->bp_polarity_file_path != NULL);

	// Open the necessary files.
	bp->bp_duty_file = fopen(bp->bp_duty_file_path, "w+");
	if(bp->bp_duty_file == NULL) {
		result = BPRC_BUSY;
		goto out;
	}
	
	bp->bp_period_file = fopen(bp->bp_period_file_path, "w+");
	if(bp->bp_period_file == NULL) {
		result = BPRC_BUSY;
		goto out;
	}
	
	bp->bp_polarity_file = fopen(bp->bp_polarity_file_path, "w+");
	if(bp->bp_polarity_file == NULL) {
		result = BPRC_BUSY;
		goto out;
	}

	// Load the cached values.
	result = get_duty_from_file(bp->bp_duty_file, &(bp->bp_duty));
	if(result != BPRC_OK) {
		goto out;
	}
	
	result = get_period_from_file(bp->bp_period_file, &(bp->bp_period));
	if(result != BPRC_OK) {
		goto out;
	}
	
	result = get_polarity_from_file(bp->bp_polarity_file, &(bp->bp_polarity));
	if(result != BPRC_OK) {
		goto out;
	}

out:
	if(result != BPRC_OK) {
		// On failure, unclaim will force a cleanup.
		bbb_pwm_unclaim(bp);
	}
	return result;
}

/**
 * @brief Unclaims a pwm.
 *
 * @param bp The pwm to unclaim.
 *
 * @return A status code.
 */
int 
bbb_pwm_unclaim(struct bbb_pwm_t* bp)
{
	assert(bp != NULL);

	bp->bp_state = BPS_UNCLAIMED;
	// Close the duty file.	
	if(bp->bp_duty_file != NULL) {
		fclose(bp->bp_duty_file);
		bp->bp_duty_file = NULL;
	}
	// Close the period file.
	if(bp->bp_period_file != NULL) {
		fclose(bp->bp_period_file);
		bp->bp_period_file = NULL;
	}
	// Close the polarity file.
	if(bp->bp_polarity_file != NULL) {
		fclose(bp->bp_polarity_file);
		bp->bp_polarity_file = NULL;
	}
	return BPRC_OK;
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

/**
 * @brief Set the duty of the pwm.
 * NOTE: The pwm must be claimed to set anything on it.
 *
 * @param bp The pwm to set.
 * @param duty The duty to set the pwm to.
 *
 * @return A status code.
 */
int 
bbb_pwm_set_duty(struct bbb_pwm_t* bp, float duty)
{
	assert(bp != NULL);

	if(!bbb_pwm_is_claimed(bp)) {
		return BPRC_NOT_CLAIMED;
	}

	if(duty > 100 || duty < 0) {
		return BPRC_RANGE;
	}

	// TODO Write to the duty file.
	return BPRC_NOT_IMPLEMENTED;
}

/**
 * @brief Set the period of the pwm.
 * NOTE: The pwm must be claimed to set anything on it.
 *
 * @param bp The pwm to set.
 * @param period The period to set it to.
 *
 * @return A status code. 
 */
int 
bbb_pwm_set_period(struct bbb_pwm_t* bp, float period)
{
	assert(bp != NULL);

	if(!bbb_pwm_is_claimed(bp)) {
		return BPRC_NOT_CLAIMED;
	}

	if(period < 0) {
		// TODO determin upper period limit.
		return BPRC_RANGE;
	}

	// TODO Write to the period file.
	return BPRC_NOT_IMPLEMENTED;

}

/**
 * @brief Sets the polarity of the pwm.
 * NOTE: The pwm must be claimed to set anything on it.
 *
 * @param bp The pwm to set.
 * @param polarity The polarity to set the pwm to.
 *
 * @return A status code.
 */
int 
bbb_pwm_set_polarity(struct bbb_pwm_t* bp, int polarity)
{
	assert(bp != NULL);

	if(!bbb_pwm_is_claimed(bp)) {
		return BPRC_NOT_CLAIMED;
	}

	if(polarity < -1 || polarity > 1) {
		// TODO Verify these limits.
		return BPRC_RANGE;
	}

	// TODO Write to the polarity file.
	return BPRC_NOT_IMPLEMENTED;
}

/**
 * @brief Gets the duty of a pwm.
 * If the pwm isn't claimed, we attempt to open the right file for reading.
 * Note that this may fail if someone else owns it.
 *
 * @param bp The pwm to read from.
 * @param[out] out_duty THe duty read.
 *
 * @return A status code.
 */
int 
bbb_pwm_get_duty(struct bbb_pwm_t* bp, float* out_duty)
{
	FILE* duty_file = NULL;
	int result = BPRC_OK;
	
	assert(bp != NULL);
	assert(out_duty != NULL);

	if(bbb_pwm_is_claimed(bp)) {
		*out_duty = bp->bp_duty;
		goto out;
	}

	duty_file = fopen(bp->bp_duty_file_path, "r");
	if(duty_file == NULL) {
		result = BPRC_BUSY;
		goto out;
	}
	
	result = get_duty_from_file(duty_file, out_duty);
out:
	if(duty_file != NULL) {
		fclose(duty_file);
	}
	return result;
}

/**
 * @brief Gets the period of a pwm.
 * If the pwm isn't claimed, we attempt to open the right file for reading.
 * Note that this may fail if someone else owns it.
 * 
 * @param bp The pwm to read from.
 * @param[out] out_period The period read.
 * 
 * @return A status code.
 */
int 
bbb_pwm_get_period(struct bbb_pwm_t* bp, float* out_period)
{
	FILE* period_file = NULL;
	int result = BPRC_OK;

	assert(bp != NULL);
	assert(out_period != NULL);
	
	if(bbb_pwm_is_claimed(bp)) {
		*out_period = bp->bp_period;
		goto out;
	}

	period_file = fopen(bp->bp_period_file_path, "r");
	if(period_file == NULL) {
		result = BPRC_BUSY;
		goto out;
	}
	
	result = get_period_from_file(period_file, out_period);
out:
	if(period_file != NULL) {
		fclose(period_file);
	}
	return result;
}

/**
 * @brief Gets the polarity of a pwm. 
 * If the pwm isn't claimed, we attempt to open the right file for reading.
 * Note that this may fail if someone else owns it.
 *
 * @param bp The pwm to read from. 
 * @param[out] out_polarity The polarity read.
 *
 * @return A status code.
 */
int 
bbb_pwm_get_polarity(struct bbb_pwm_t* bp, int* out_polarity)
{
	FILE* polarity_file = NULL;
	int result = BPRC_OK;

	assert(bp != NULL);
	assert(out_polarity != NULL);

	if(bbb_pwm_is_claimed(bp)) {
		*out_polarity = bp->bp_polarity;
		goto out;
	}

	polarity_file = fopen(bp->bp_polarity_file_path, "r");
	if(polarity_file == NULL) {
		result = BPRC_BUSY;
		goto out;
	}

	result = get_polarity_from_file(polarity_file, out_polarity);
out:
	if(polarity_file != NULL) {
		fclose(polarity_file);
	}
	return result;
}

/**
 * @brief Gets the duty of a pwm from a file.
 *
 * @param file The file to get the duty from.
 * @param[out] out_duty THe duty read.
 *
 * @return A status code. 
 */
int
get_duty_from_file(FILE* file, float* out_duty)
{
	assert(out_duty != NULL);
	if(file == NULL) {
		return BPRC_BAD_FILE;
	}
	return BPRC_NOT_IMPLEMENTED;
}

/**
 * @brief Gets the period of a pwm from a file.
 *
 * @param file The file to get the polarity from.
 * @param[out] out_period The period read.
 *
 * @return A status code.
 */
int 
get_period_from_file(FILE* file, float* out_period)
{
	assert(out_period != NULL);
	if(file == NULL) {
		return BPRC_BAD_FILE;
	}
	return BPRC_NOT_IMPLEMENTED;
}

/**
 * @brief Gets the polarity of a pwm from a file.
 *
 * @param file The file to get the polarity from.
 * @param[out] out_polarity The polarity read.
 *
 * @return A status code.
 */
int 
get_polarity_from_file(FILE* file, int* out_polarity)
{
	assert(out_polarity != NULL);
	if(file == NULL) {
		return BPRC_BAD_FILE;
	}
	return BPRC_NOT_IMPLEMENTED;
}
