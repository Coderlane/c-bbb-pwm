/**
 * @file bbb_capemgr.h
 * @brief THe interface for the capemgr.
 * These functions help work with the bone_capemgr.
 * You can emable components as you see fit.
 * Currently disabling is disabled.
 * It seems to be broken on the kernel side.
 * @author Travis Lane
 * @version 0.1.2
 * @date 2014-12-25
 */


#ifndef BBB_CAPEMGR_H
#define BBB_CAPEMGR_H

struct bbb_capemgr_t;

struct bbb_capemgr_t* bbb_capemgr_new();
void bbb_capemgr_delete(struct bbb_capemgr_t** bc_ptr);

int bbb_capemgr_enable(struct bbb_capemgr_t* bc, char* dev);
int bbb_capemgr_disable(struct bbb_capemgr_t* bc, char* dev);

#endif
