

#ifndef BBB_CAPEMGR_H
#define BBB_CAPEMGR_H

struct bbb_capemgr_t;

struct bbb_capemgr_t* bbb_capemgr_new();
void bbb_capemgr_delete(struct bbb_capemgr_t** bc_ptr);

int bbb_capemgr_enable(struct bbb_capemgr_t* bc, char* dev);
int bbb_capemgr_disable(struct bbb_capemgr_t* bc, char* dev);

#endif
