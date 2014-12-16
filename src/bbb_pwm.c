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
	
	bpc->bpc_udev = udev_new();
	assert(bpc->bpc_udev != NULL);

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
	int result = BPRC_OK;
	char *capemgr_path = NULL;
	char *capemgr_slots_path = NULL;
	
	capemgr_path = find_device_syspath(bpc->bpc_udev, "driver", "bone-capemgr");
	if(capemgr_path == NULL) {
		result = BPRC_NO_CAPEMGR;
		goto out;
	}
	
	capemgr_slots_path = push_path(capemgr_path, "slots");
	if(capemgr_slots_path == NULL) {
		result = BPRC_NO_MEM;
		goto out;
	}

out:
	if(capemgr_path != NULL) {
		free(capemgr_path);
	}
	if(capemgr_slots_path != NULL) {
		free(capemgr_slots_path);
	}
	return result;
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

/**
 * @brief 
 *
 * @param bpc
 *
 * @return 
 */
char *
find_device_syspath(struct udev *probe_udev, char *sysattr, char *value) 
{
	char *result = NULL;
	struct udev_enumerate *enumer = NULL;
	struct udev_list_entry *devs = NULL; 
	struct udev_list_entry *dev_entry = NULL; 


	// Set up the enumeration
	enumer = udev_enumerate_new(probe_udev);
	assert(enumer != NULL);

	// Look for the input values
	udev_enumerate_add_match_sysattr(enumer, sysattr, value);

	// Scan and get the list of items found.
	udev_enumerate_scan_devices(enumer);
	devs = udev_enumerate_get_list_entry(enumer);
	if(devs == NULL) {
		goto out;
	}

	udev_list_entry_foreach(dev_entry, devs) {
		result = strdup(udev_list_entry_get_name(dev_entry));
		// Just grab the first one.
		goto out;
	}

out:
	udev_enumerate_unref(enumer);
	return result;
}

/**
 * @brief Push other onto base.
 * Assumes base doesn't have a trailing /
 * Assumes other doesn't have a leading /
 *
 * @param base The base to push other onto.
 * @param other The other part of the path to push onto base.
 *
 * @return The resultant path if successful, else NULL
 */
char *push_path(char *base, char *other)
{
	char *result = NULL;
	
	if(asprintf(&result, "%s/%s", base, other) < 0) {
		return NULL;
	}

	return result;
}
