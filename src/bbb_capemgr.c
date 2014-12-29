/**
 * @file bbb_capemgr.c
 * @brief This file contains the source behind the capemgr.
 * The capemgr helps to work with devices exposed through the
 * bone_capemgr interface. I use udev to make detection easier. 
 * @author Travis Lane
 * @version 0.1.2
 * @date 2014-12-25
 */

#include "bbb_capemgr_internal.h"

/**
 * @brief Creates an initializes a new capemgr.
 *
 * @return A new capemgr or NULL on failure.
 */
struct bbb_capemgr_t* 
bbb_capemgr_new()
{
	struct bbb_capemgr_t* bc;
	bc = calloc(sizeof(struct bbb_capemgr_t), 1);
	bc->bc_udev = udev_new();

	if(bbb_capemgr_init(bc) < 0) {
		bbb_capemgr_delete(&bc);
	}
	return bc;
}

/**
 * @brief Destroys a capemgr. 
 *
 * @param bc_ptr A pointer to the capemgr to destroy.
 */
void 
bbb_capemgr_delete(struct bbb_capemgr_t** bc_ptr)
{
	struct bbb_capemgr_t* bc;
	assert(bc_ptr != NULL);

	bc = *bc_ptr;
	if(bc == NULL)
		return;

	if(bc->bc_udev != NULL) {
		udev_unref(bc->bc_udev);
		bc->bc_udev = NULL;
	}
	
	if(bc->bc_capemgr_path != NULL) {
		free(bc->bc_capemgr_path);
		bc->bc_capemgr_path = NULL;
	}

	if(bc->bc_slots_path != NULL) {
		free(bc->bc_slots_path);
		bc->bc_slots_path = NULL;
	}

	free(bc);
	*bc_ptr = NULL;
}

/**
 * @brief Initializes a capemgr.
 * Initially finds the path to the capemgr.
 * Then finds the path to the slots file.
 *
 * @param bc The capemgr to initialize.
 *
 * @return -1 on failure, >1 on success.
 */
int 
bbb_capemgr_init(struct bbb_capemgr_t* bc)
{
	bc->bc_capemgr_path = bbb_capemgr_find_path(bc);
	if(bc->bc_capemgr_path == NULL) {
		return -1;
	}	
	
	return asprintf(&(bc->bc_slots_path), "%s/%s", bc->bc_capemgr_path, "slots");
}

/**
 * @brief Finds the bone-capemgr path.
 * Typically /sys/devices/bone-capemgr.9
 * However, this may change so we use udev to find it. 
 *
 * @param bc The capemgr to probe.
 *
 * @return A copy of the path to the capemgr.
 */
char*
bbb_capemgr_find_path(struct bbb_capemgr_t* bc)
{
	char *result = NULL;
	struct udev_enumerate *enumer = NULL;
	struct udev_list_entry *devs = NULL; 
	struct udev_list_entry *dev_entry = NULL; 


	// Set up the enumeration
	enumer = udev_enumerate_new(bc->bc_udev);
	assert(enumer != NULL);

	// Look for the input values
	udev_enumerate_add_match_sysattr(enumer, "driver", "bone-capemgr");

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
 * @brief Checks to see if a device is already enabled on the capemgr.
 *
 * @param bc The capemgr to search.
 * @param dev The device to search for.
 * @param[out] out_devid The device id found, or -1.
 *
 * @return A status code, -1 on failure, 0 on not found, 1 on found.
 */
int 
bbb_capemgr_find(struct bbb_capemgr_t* bc, char* dev, int *out_devid)
{
	int result = 0;
	FILE* slots;
	ssize_t read;
	size_t len;
	char* line = NULL;

	assert(bc != NULL);
	assert(dev != NULL);
	assert(out_devid != NULL);

	*out_devid = -1;

	slots = fopen(bc->bc_slots_path, "r");
	if(slots == NULL) {
		result = -1;
		goto out;
	}
	
	while((read = getline(&line, &len, slots)) != -1) {
		char* found;
		// Check for the device in this line.
		if((found  = strstr(line, dev)) != NULL) {
			// tok on the devid
			strtok(found, ":");
			// Fetch devid
			*out_devid = atoi(found);

			result = 1;
			goto out;
		}
	}	

out:
	if(line != NULL) 
		free(line);

	if(slots != NULL)
		fclose(slots);

	return result;
}

/**
 * @brief Enable a device.
 *
 * @param bc The capemgr to enable the device on.
 * @param dev The device to enable.
 *
 * @return A status code, 0 on success, -1 on failure.
 */
int 
bbb_capemgr_enable(struct bbb_capemgr_t* bc, char* dev)
{
	int result = 0, found, devid;
	FILE* slots = NULL;

	found = bbb_capemgr_find(bc, dev, &devid);

	if(found < 0) {
		// Failed to open file.
		result = found;
		goto out;	
	}

	if(found > 0) {
		// Already enabled.
		result = 0;
		goto out;
	}

	slots = fopen(bc->bc_slots_path, "a");
	if(slots == NULL) {
		result = -1;
		goto out;
	}

	fprintf(slots, "%s\n", dev);

out:
	if(slots != NULL)
		fclose(slots);

	return result;
}

/**
 * @brief Disable a device. 
 * 
 * @param bc The capemgr to disable the device on.
 * @param dev The device to disable.
 *
 * @return A status code 0 on success, -1 on failure.
 */
int 
bbb_capemgr_disable(struct bbb_capemgr_t* bc, char* dev)
{
	int result = 0, found, devid;
	FILE* slots = NULL;

	found = bbb_capemgr_find(bc, dev, &devid);

	if(found < 0) {
		// Failed to open file.
		result = found;
		goto out;	
	}

	if(found == 0) {
		// Already disabled.
		result = 0;
		goto out;
	}
	
	// Because file IO to enable/disable devices is sane.
	slots = fopen(bc->bc_slots_path, "a");
	if(slots == NULL) {
		result = -1;
		goto out;
	}

	fprintf(slots, "-%d\n", devid);

out:
	if(slots != NULL)
		fclose(slots);

	return result;
}

