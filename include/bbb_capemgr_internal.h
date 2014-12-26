/**
 * @file bbb_capemgr_internal.h
 * @brief Internal code for the capemgr.
 * Node: Don't include this file, it is unnecessary.
 * @author Travis Lane
 * @version 0.1.0
 * @date 2014-12-25
 */


#ifndef BBB_CAPEMGR_INTERNAL_H
#define BBB_CAPEMGR_INTERNAL_H

#define _GNU_SOURCE

#include <bbb_capemgr.h>

#include <libudev.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct bbb_capemgr_t {
	char 				 *bc_capemgr_path;
	char 				 *bc_slots_path;
	struct udev  *bc_udev;
};

int bbb_capemgr_init(struct bbb_capemgr_t* bc);
int bbb_capemgr_find(struct bbb_capemgr_t* bc, char* dev, int* out_devid);

char* bbb_capemgr_find_path(struct bbb_capemgr_t* bc);
#endif /* BBB_CAPEMGR_INTERNAL_H */
