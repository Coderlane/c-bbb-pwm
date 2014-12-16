

#include <bbb_capemgr.h>
#include <stdio.h>

int main() {
	int result = 0;
	struct bbb_capemgr_t* bc;

	bc = bbb_capemgr_new();
	result = bbb_capemgr_enable(bc, "am33xx_pwm");

	if(result < 0) {
		fprintf(stderr, "Error enableing pwm.\n");
	}

	bbb_capemgr_delete(&bc);

	return result; 
}
