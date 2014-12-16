
#include <bbb_pwm_internal.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char *capemgr_path = NULL;
	struct udev *probe_udev;

	probe_udev = udev_new();
	assert(probe_udev != NULL);

	capemgr_path = find_device_syspath(probe_udev, "driver", "bone-capemgr");
	assert(capemgr_path != NULL);

	return 0;
}

