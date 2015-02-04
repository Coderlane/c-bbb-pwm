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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#include <bbb_pwm.h>

enum pwm_tool_op_e {
  BPT_INVALID = -1,
  BPT_NO_OPT = 0,
  BPT_HELP = 1,
  BPT_VERSION = 2,
  BPT_LIST = 3
};

enum pwm_tool_gs_e {
  BPT_GET = 0,
  BPT_SET = 1
};

enum pwm_tool_func_e {
  BPT_DUTY_CYCLE_FUNC,
  BPT_PERIOD_FUNC,
  BPT_POLARITY_FUNC,
  BPT_DUTY_PERCENT_FUNC,
  BPT_FREQUENCY_FUNC,
  BPT_RUNNING_FUNC,
  BPT_INVALID_FUNC
};

typedef int (*pwm_tool_func_t)(struct bbb_pwm_t *, enum pwm_tool_gs_e, char *);

int main(int argc, char **argv);
enum pwm_tool_op_e parse_args(int argc, char **argv);
void usage();
void version();
int list_pwms();
int do_pwms(int argc, char **argv);
int do_pwm(struct bbb_pwm_t *pwm, char *get_set_str,
           char *opt_str, char *val_str);

int do_duty_cycle(struct bbb_pwm_t *pwm, enum pwm_tool_gs_e get_set,
                  char *val_str);

int do_period(struct bbb_pwm_t *pwm, enum pwm_tool_gs_e get_set, char *val_str);
int do_polarity(struct bbb_pwm_t *pwm, enum pwm_tool_gs_e get_set,
                char *val_str);
int do_duty_percent(struct bbb_pwm_t *pwm, enum pwm_tool_gs_e get_set,
                    char *val_str);
int do_frequency(struct bbb_pwm_t *pwm, enum pwm_tool_gs_e get_set,
                 char *val_str);
int do_running(struct bbb_pwm_t *pwm, enum pwm_tool_gs_e get_set,
               char *val_str);

static pwm_tool_func_t pwm_tool_func_arr[] = {
  do_duty_cycle, do_period, do_polarity,
  do_duty_percent, do_frequency, do_running
};

static char *pwm_tool_func_strs_arr[] = {
  "duty_cycle", "period", "polarity", "duty_percent",
  "frequency", "running"
};

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
 * @brief Print the usage.
 */
void
usage()
{
  printf("bbb_pwm_tool - A tool to work with PWMs on a BeagleBone Black.\n\n");

  printf("usage: bbb_pwm_tool [arguements] \n"
         "   or: bbb_pwm_tool <pwm> <get/set> <option> [value]\t"
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
 * @brief Print the version.
 */
void version()
{
  printf("bbb_pwm_tool 0.1.0\n");
}

/**
 * @brief Parse the input arguements.
 *
 * @param argc The number of arguements.
 * @param argv The arguements.
 *
 * @return An enum representing what was input.
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
 * @brief List the current pwms.
 *
 * @return A status code.
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
 * @brief Attempts to run a command on the pwms.
 *
 * @param argc The number of arguements.
 * @param argv The arguements.
 *
 * @return A status code.
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
  optend = argc;

  if(optend - optsrt < 3 || optend - optsrt > 4) {
    fprintf(stderr, "Error, invalid number of options.\n");
    result = -3;
    goto out;
  }

  pwm_name = argv[optsrt];
  get_set_str = argv[optsrt + 1];
  opt_str = argv[optsrt + 2];
  if(optend - optsrt == 4) {
    val_str = argv[optsrt + 3];
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

/**
 * @brief Attempt to run a command for a pwm.
 *
 * @param pwm The pwm to run the command on.
 * @param get_set_str "get" or "set".
 * @param opt_str The option string.
 * @param val_str The value to if set is chosen.
 *
 * @return A status code.
 */
int
do_pwm(struct bbb_pwm_t *pwm, char *get_set_str, char *opt_str, char *val_str)
{
  enum pwm_tool_gs_e get_set, result = 0;

  assert(pwm != NULL);
  assert(get_set_str != NULL);
  assert(opt_str != NULL);

  if(strcmp(get_set_str, "set") == 0) {
    get_set = BPT_SET;
    assert(val_str != NULL);

    if(bbb_pwm_claim(pwm) != BPRC_OK) {
      fprintf(stderr, "Error could not claim pwm.\n");
      result = -3;
      goto out;
    }
  } else if(strcmp(get_set_str, "get") == 0) {
    get_set = BPT_GET;
    if(bbb_pwm_claim(pwm) != BPRC_OK) {
      fprintf(stderr, "Error could not claim pwm.\n");
      result = -3;
      goto out;
    }
 
  } else {
    fprintf(stderr,
            "Error, must be get or set, %s is invalid.\n", get_set_str);
    result = -4;
    goto out;
  }

  for(int i = 0; i <= BPT_INVALID_FUNC; i++) {
    if(i == BPT_INVALID_FUNC) {
      fprintf(stderr, "Error, invalid option string: %s\n", opt_str);
      result = -5;
      goto out;
    } else if(strcmp(pwm_tool_func_strs_arr[i], opt_str) == 0 ) {
      result = pwm_tool_func_arr[i](pwm, get_set, val_str);
      goto out;
    }
  }
out:
  if(pwm != NULL) {
    if(bbb_pwm_is_claimed(pwm)) {
      bbb_pwm_unclaim(pwm);
    }
  }

  return result;
}

/**
 * @brief
 *
 * @param pwm
 * @param get_set
 * @param val_str
 *
 * @return
 */
int
do_duty_cycle(struct bbb_pwm_t *pwm, enum pwm_tool_gs_e get_set, char *val_str)
{
  int result;
  uint32_t duty;

  assert(pwm != NULL);
  assert(bbb_pwm_is_claimed(pwm));
  assert(get_set == BPT_SET || get_set == BPT_GET);

  if(get_set == BPT_SET) {
    // Try setting the duty cycle.
    sscanf(val_str, "%" SCNu32 "", &duty);
    result = bbb_pwm_set_duty_cycle(pwm, duty);
    if(result != BPRC_OK) {
      fprintf(stderr, "Error setting pwm duty cycle.\n");
    }
  } else {
    // Try getting the duty cycle.
    result = bbb_pwm_get_duty_cycle(pwm, &duty);
    if(result == BPRC_OK) {
      printf("%"PRIu32"\n", duty);
    } else {
      fprintf(stderr, "Error getting pwm duty cycle.");
    }
  }
  return result;
}

/**
 * @brief
 *
 * @param pwm
 * @param get_set
 * @param val_str
 *
 * @return
 */
int
do_period(struct bbb_pwm_t *pwm, enum pwm_tool_gs_e get_set, char *val_str)
{
  int result;
  uint32_t period;

  assert(pwm != NULL);
  assert(bbb_pwm_is_claimed(pwm));
  assert(get_set == BPT_SET || get_set == BPT_GET);

  if(get_set == BPT_SET) {
    // Try setting the period cycle.
    sscanf(val_str, "%" SCNu32 "", &period);
    result = bbb_pwm_set_period(pwm, period);
    if(result != BPRC_OK) {
      fprintf(stderr, "Error setting pwm period.\n");
    }
  } else {
    // Try getting the period cycle.
    result = bbb_pwm_get_period(pwm, &period);
    if(result == BPRC_OK) {
      printf("%"PRIu32"\n", period);
    } else {
      fprintf(stderr, "Error getting pwm period.\n");
    }
  }
  return result;
}

/**
 * @brief
 *
 * @param pwm
 * @param get_set
 * @param val_str
 *
 * @return
 */
int
do_polarity(struct bbb_pwm_t *pwm, enum pwm_tool_gs_e get_set, char *val_str)
{
  int result;
  int8_t polarity;

  assert(pwm != NULL);
  assert(bbb_pwm_is_claimed(pwm));
  assert(get_set == BPT_SET || get_set == BPT_GET);

  if(get_set == BPT_SET) {
    // Try setting the polarity cycle.
    sscanf(val_str, "%" SCNd8 "", &polarity);
    result = bbb_pwm_set_polarity(pwm, polarity);
    if(result != BPRC_OK) {
      fprintf(stderr, "Error setting pwm polarity.\n");
    }
  } else {
    // Try getting the polarity cycle.
    result = bbb_pwm_get_polarity(pwm, &polarity);
    if(result == BPRC_OK) {
      printf("%"PRId8"\n", polarity);
    } else {
      fprintf(stderr, "Error getting pwm polarity.\n");
    }
  }
  return result;
}

/**
 * @brief
 *
 * @param pwm
 * @param get_set
 * @param val_str
 *
 * @return
 */
int
do_duty_percent(struct bbb_pwm_t *pwm, enum pwm_tool_gs_e get_set,
                char *val_str)
{
  int result;
  float duty;

  assert(pwm != NULL);
  assert(bbb_pwm_is_claimed(pwm));
  assert(get_set == BPT_SET || get_set == BPT_GET);

  if(get_set == BPT_SET) {
    // Try setting the duty percent.
    sscanf(val_str, "%f", &duty);
    result = bbb_pwm_set_duty_percent(pwm, duty);
    if(result != BPRC_OK) {
      fprintf(stderr, "Error setting duty percent.\n");
    }
  } else {
    // Try getting the duty percent.
    result = bbb_pwm_get_duty_percent(pwm, &duty);
    if(result == BPRC_OK) {
      printf("%f\n", duty);
    } else {
      fprintf(stderr, "Error getting pwm duty percent.\n");
    }
  }
  return result;
}

int
do_frequency(struct bbb_pwm_t *pwm, enum pwm_tool_gs_e get_set, char *val_str)
{
  int result;
  uint32_t frequency;

  assert(pwm != NULL);
  assert(bbb_pwm_is_claimed(pwm));
  assert(get_set == BPT_SET || get_set == BPT_GET);

  if(get_set == BPT_SET) {
    // Try setting the frequency.
    sscanf(val_str, "%"SCNu32, &frequency);
    result = bbb_pwm_set_frequency(pwm, frequency);
    if(result != BPRC_OK) {
      fprintf(stderr, "Error setting pwm frequency.\n");
    }
  } else {
    // Try getting the frequency.
    result = bbb_pwm_get_frequency(pwm, &frequency);
    if(result == BPRC_OK) {
      printf("%"PRIu32"\n", frequency);
    } else {
      fprintf(stderr, "Error getting pwm frequency.\n");
    }
  }
  return result;
}

int
do_running(struct bbb_pwm_t *pwm, enum pwm_tool_gs_e get_set, char *val_str)
{
  int result;

  assert(pwm != NULL);
  assert(get_set == BPT_SET || get_set == BPT_GET);

  if(get_set == BPT_SET) {
    if(atoi(val_str)) {
      result = bbb_pwm_start(pwm);
      if(result != BPRC_OK) {
        fprintf(stderr, "Error starting pwm.\n");
      }
    } else {
      result = bbb_pwm_stop(pwm);
      if(result != BPRC_OK) {
        fprintf(stderr, "Error stoping pwm.\n");
      }
    }
  } else {
    if(bbb_pwm_is_running(pwm)) {
      printf("1\n");
    } else {
      printf("0\n");
    }
    result = 0;
  }

  return result;
}
