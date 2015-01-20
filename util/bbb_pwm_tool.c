
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <bbb_pwm.h>

enum bpt_tool_op_e {
  BPT_INVALID = -1,
  BPT_NO_OPT = 0,
  BPT_HELP = 1,
  BPT_VERSION = 2,
  BPT_LIST = 3
};

int main(int argc, char **argv);
enum bpt_tool_op_e parse_args(int argc, char **argv);
void usage();
void version();
int list_pwms();
int do_pwms();


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
  switch(parse_args(argc, argv)) {
  case BPT_INVALID:
    usage();
    return -1;
  case BPT_HELP:
    usage();
    return 0;
  case BPT_VERSION:
    version();
    return 0;
  case BPT_LIST:
    return list_pwms();
  case BPT_NO_OPT:
    return do_pwms();
  default:
    usage();
    return -1;
  }
  return -1;
}

void
usage()
{
  printf("bbb_pwm_tool - A tool to work with PWMs on a BeagleBone Black.\n\n");

  printf("usage: bbb_pwm_tool [arguements] [<pwm> <get/set> <value>]\t"
         "get/set values for a pwm.\n\n");

  printf("Arguements:\n");
  printf("\t-h\t\tDisplay help.\n");
  printf("\t-l\t\tList all detected PWMs.\n");

  printf("Values:\n");
  printf("\tduty_cycle\tThe duty cycle of the PWM.\n");
  printf("\tpolarity\tThe polarity of the PWM.\n");
  printf("\tperiod\t\tThe period of the PWM.\n");
  printf("\tduty_percent\tThe duty percent of the PWM. 0%% is STOP, 100%% is FULL.\n");
  printf("\tfrequency\tThe frequency of the PWM.\n");
  printf("\trunning\t\tThe running state of the PWM. 0 is OFF, 1 is ON.\n");
}

enum bpt_tool_op_e
parse_args(int argc, char **argv)
{
  int opt;

  if(argc <= 1) {
    exit(0);
  }

  while ((opt = getopt (argc, argv, "hlv")) != -1) {
    switch (opt) {
    case 'h':
      return BPT_HELP;
    case 'l':
      return BPT_LIST;
    case 'v':
      return BPT_VERSION;
    case '?':
      if (isprint (optopt)) {
        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
      } else {
        fprintf (stderr,
                 "Unknown option character `\\x%x'.\n",
                 optopt);
      }
      return BPT_INVALID;

    default:
      exit(-1);
    }
  }

  return BPT_NO_OPT;
}

void version()
{

}

int
list_pwms()
{
  return -1;
}

int
do_pwms()
{
  return -1;
}
