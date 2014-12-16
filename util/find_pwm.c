

#include <bbb_capemgr.h>
#include <bbb_capemgr_internal.h>
#include <stdio.h>

int main() {
	int result = 0, devid;
	struct bbb_capemgr_t* bc;

	bc = bbb_capemgr_new();
	result = bbb_capemgr_find(bc, "am33xx_pwm", &devid);

	if(result < 0) {
		fprintf(stderr, "Error finding pwm.\n");
	} else {
		printf("ID: %d\n", devid);
	}

	bbb_capemgr_delete(&bc);

	return result; 
}
