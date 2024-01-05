#include "../include/allocator.h"

static chrus_rbtree* loader_trees[CHRUS_LOADER_END] = {0};

static int chrus_loader_comparator(const void *, const void *);
static void chrus_loader_destructor(const void *);
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
    loader_trees[CHRUS_LOADER_BITMAP] = chrus_rbtree_create((chrus_rbtree_comparator)strcmp, (chrus_rbtree_destructor)al_destroy_bitmap, (chrus_rbtree_inserter)al_load_bitmap);
    //loader_trees[CHRUS_LOADER_SCRIPT] = chrus_rbtree_create(strcmp, );
    loader_trees[CHRUS_LOADER_SAMPLE] = chrus_rbtree_create((chrus_rbtree_comparator)strcmp, (chrus_rbtree_destructor)al_destroy_sample, (chrus_rbtree_inserter)al_load_sample);
}

void *chrus_loader_insert(CHRUS_LOADER index, const void *key) {
    return chrus_rbtree_insert(loader_trees[index], key)->value;
}

void *chrus_loader_get(CHRUS_LOADER index, const void *key) {
    return chrus_rbtree_find(loader_trees[index], key)->value;
}

void chrus_loader_deinit() {
    for (int i = 0; i < CHRUS_LOADER_END; i++) {
        chrus_rbtree_destroy(loader_trees[i]);
    }
}