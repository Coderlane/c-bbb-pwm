
#include <libgen.h>
#include <stdio.h>
#include <unistd.h>


#include <bbb_pwm.h>

int main(int argc, char **argv);
int usage(char *prog_path);

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
	usage(argv[0]);
}

int 
usage(char *prog_path)
{
	char *prog_name = basename(prog_path);

	printf("usage: %s\n", prog_name);
}
