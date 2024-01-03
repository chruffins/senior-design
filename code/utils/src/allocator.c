#include "../include/allocator.h"

static int chrus_loader_comparator(const void *, const void *);
static void chrus_loader_destructor(const void *, const void *);
//static void* chrus_loader_insertinator(const void *);

static int chrus_loader_comparator(const void *search, const void *nodekey) {
    return strcmp(search, nodekey);
}

static void chrus_loader_destructor(const void *something) {
    return;
}

//static void* chrus_loader_insertinator(const void *key) {
    // this function needs to deal wi
//}

void chrus_loader_init() {
    loader_trees[CHRUS_LOADER_BITMAP] = chrus_rbtree_create(strcmp, al_destroy_bitmap, al_load_bitmap);
    //loader_trees[CHRUS_LOADER_SCRIPT] = chrus_rbtree_create(strcmp, );
    loader_trees[CHRUS_LOADER_SOUND] = chrus_rbtree_create(strcmp, al_destroy_samp)
}