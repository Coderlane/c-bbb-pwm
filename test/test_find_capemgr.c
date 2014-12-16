
#include <bbb_pwm_internal.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char *capemgr_path = NULL;
	struct udev *probe_udev;

	probe_udev = udev_new();
	if(probe_udev == NULL) {
		fprintf(stderr, "Failed to setup udev.\n");
		return -1;
	}

	capemgr_path = find_device_syspath(probe_udev, "driver", "bone-capemgr");
	if(capemgr_path == NULL) {
		fprintf(stderr, "Failed to find capemgr_path.\n");
	}
	
	printf("capemgr path: %s\n", capemgr_path);
	free(capemgr_path);
	return 0;
}

