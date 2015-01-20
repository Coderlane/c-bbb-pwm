
#include <assert.h>
#include <libgen.h>
#include <stdio.h>
#include <unistd.h>


#include <bbb_pwm.h>

int main(int argc, char **argv);
int parse_args(int argc, char **argv);
void usage();

static char *prog_name = NULL;

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

  printf("usage: %s\n", prog_name);
}

int
parse_args(int argc, char **argv)
{
  if(argc < 1) {
    return -1;
  }

  prog_name = basename(argv[0]);
  assert(prog_name != NULL);

  return -1;
}
