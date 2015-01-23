/**
 * @file bbb_pwm_tool.c
 * @brief A tool to work with PWMs on a BeagleBone Black.
 * @author Travis Lane
 * @version 0.1.0
 * @date 2015-01-20
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <bbb_pwm.h>

enum pwm_tool_op_e {
  BPT_INVALID = -1,
  BPT_NO_OPT = 0,
  BPT_HELP = 1,
  BPT_VERSION = 2,
  BPT_LIST = 3,
	BPT_GET = 10,
	BPT_SET = 11
};

int main(int argc, char **argv);
enum pwm_tool_op_e parse_args(int argc, char **argv);
void usage();
void version();
int list_pwms();
int do_pwms(int argc, char **argv);
int do_pwm(struct bbb_pwm_t *pwm, char *get_set_str,
           char *opt_str, char *val_str);

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
    return do_pwms(argc, argv);
  default:
    usage();
    return -1;
  }
  return -1;
}

/**
 * @brief
 */
void
usage()
{
  printf("bbb_pwm_tool - A tool to work with PWMs on a BeagleBone Black.\n\n");

  printf("usage: bbb_pwm_tool [arguements] \n"
         "   or: bbb_pwm_tool <pwm> <get/set> <value>\t"
         "get/set values for a pwm.\n\n");

  printf("Arguements:\n");
  printf("\t-h\t\tDisplay help.\n");
  printf("\t-v\t\tDisplay version.\n");
  printf("\t-l\t\tList all detected PWMs.\n");

  printf("Values:\n");
  printf("\tduty_cycle\tThe duty cycle of the PWM.\n");
  printf("\tpolarity\tThe polarity of the PWM.\n");
  printf("\tperiod\t\tThe period of the PWM.\n");
  printf("\tduty_percent\tThe duty percent of the PWM. 0%% is STOP, 100%% is FULL.\n");
  printf("\tfrequency\tThe frequency of the PWM.\n");
  printf("\trunning\t\tThe running state of the PWM. 0 is OFF, 1 is ON.\n");
}

/**
 * @brief
 *
 * @param argc
 * @param argv
 *
 * @return
 */
enum pwm_tool_op_e
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

/**
 * @brief
 */
void version()
{
  printf("bbb_pwm_tool 0.1.0\n");
}

/**
 * @brief
 *
 * @return
 */
int
list_pwms()
{
  struct bbb_pwm_controller_t *bpc = NULL;

  bpc = bbb_pwm_controller_new();

  foreach_pwm(bp, bpc) {
    printf("%s\n", bbb_pwm_get_name(bp));
  }

  bbb_pwm_controller_delete(&bpc);
  return 0;
}

/**
 * @brief
 *
 * @return
 */
int
do_pwms(int argc, char **argv)
{
  char *pwm_name, *get_set_str, *opt_str, *val_str;
  int result, optsrt, optend;
  struct bbb_pwm_t *pwm;
  struct bbb_pwm_controller_t *bpc = NULL;

  bpc = bbb_pwm_controller_new();
  optsrt = optind;
	optend = argc - optsrt;

	if(optsrt - optend < 3 || optsrt - optend > 4) {
		fprintf(stderr, "Error, invalid number of options.");
		result = -3;
		goto out;
	}

	pwm_name = argv[optsrt];
	get_set_str = argv[optsrt];
	opt_str = argv[optsrt];
	if(optsrt - optend == 4) {
		val_str = argv[optsrt];
	}

  pwm = bbb_pwm_controller_get_pwm(bpc, pwm_name); 
  if(pwm == NULL) {
    fprintf(stderr, "Failed to find pwm: %s\n", pwm_name);
		result = -2;
  	goto out;
	}
  
	result = do_pwm(pwm, get_set_str, opt_str, val_str);

out:
  if(bpc != NULL) {
    bbb_pwm_controller_delete(&bpc);
  }
  return result;
}

int
do_pwm(struct bbb_pwm_t *pwm, char *get_set_str, char *opt_str, char *val_str)
{
	int get_set;

	if(strcmp(get_set_str, "set") == 0) {
		get_set = BPT_SET;
	} else if(strcmp(get_set_str, "get") == 0) {
		get_set = BPT_GET;
	} else {
		fprintf(stderr, "Must be get or set, %s is invalid.\n", get_set_str);
		return -4;
	}

	if(strcmp(opt_str, "duty_cycle") == 0) {

	} else if(strcmp(opt_str, "polarity") == 0) {

	} else if(strcmp(opt_str, "period") == 0) {

	} else if(strcmp(opt_str, "duty_percent") == 0) {

	} else if(strcmp(opt_str, "frequency") == 0) {

	} else if(strcmp(opt_str, "running") == 0) {

	} else {
		fprintf(stderr, "Invalid get/set option: %s\n", opt_str);
		return -5;
	}

	return 0;
}
