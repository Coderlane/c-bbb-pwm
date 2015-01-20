
#include <assert.h>
#include <libgen.h>
#include <stdio.h>
#include <unistd.h>


#include <bbb_pwm.h>

int main(int argc, char **argv);
int parse_args(int argc, char **argv);
void usage();

/**
 * @brief
 *
 * @param argc
 * @param argv
 *
 * @return
 */
int
main(int argc, char **argv)
{
  if(parse_args(argc, argv) < 0) {
    usage();
  }

}

void
usage()
{
  printf("usage: bbb_pwm_tool [arguements]\n");
  printf("usage: bbb_pwm_tool [pwm] [get/set] [value]\n\n");

  printf("Arguements:\n");
  printf("Values:\n");
}

int
parse_args(int argc, char **argv)
{
  if(argc <= 1) {
		usage();
    return -1;
	}
	
	if(argv[1][0] == '-') {
		// Parse arguements
		return 0;
	} else if(argc == 4) {
		// Get/Set the various arguements.
		return 0;
	} else {
		// Invalid.
  	return -2;
	}
}
