/**
 * @file bbb_pwm.c
 * @brief 
 * @author Travis Lane
 * @version 
 * @date 2014-10-10
 */

#include <bbb_pwm_internal.h>

/**
 * @brief 
 *
 * @return 
 */
int
bbb_pwm_setup() 
{

	return BPRC_NOT_IMPLEMENTED;
}

/**
 * @brief 
 *
 * @return 
 */
int
bbb_pwm_cleanup()
{

	return BPRC_NOT_IMPLEMENTED;
}

/**
 * @brief Create a new library.
 *
 * @return A new library, or NULL on failure.
 */
struct bbb_pwm_library_t* 
bbb_pwm_library_new()
{
	struct bbb_pwm_library_t* bpl_ptr = NULL;
	bpl_ptr = calloc(sizeof(struct bbb_pwm_library_t), 1);
	assert(bpl_ptr != NULL);
	return bpl_ptr;
}

/**
 * @brief Free a library.
 *
 * @param bpl_ptr The library to free.
 */
void 
bbb_pwm_library_delete(struct bbb_pwm_library_t **bpl_ptr) 
{
	struct bbb_pwm_library_t *bpl;
	// Check the ptr.
	if(bpl_ptr == NULL) {
		return;
	}
	// Check the referenced ptr.
	bpl = (*bpl_ptr);
	if(bpl == NULL) {
		return;
	}
	// Free associated memory.
	if(bpl->bpl_cape_path != NULL) {
		free(bpl->bpl_cape_path);
		bpl->bpl_cape_path = NULL;
	}	

	if(bpl->bpl_ocp_path != NULL) {
		free(bpl->bpl_ocp_path);
		bpl->bpl_ocp_path = NULL;
	}

	// Free the origional bpl
	free(bpl);
	(*bpl_ptr) = NULL;
}

int
bbb_pwm_library_init(struct bbb_pwm_library_t *bpl) 
{

	return BPRC_NOT_IMPLEMENTED;
}

