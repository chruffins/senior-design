#include "../include/allocator.h"

static chrus_rbtree* loader_trees[CHRUS_LOADER_END] = {0};

static int chrus_loader_comparator(chrus_rbkey, chrus_rbkey);
static void chrus_loader_destructor(chrus_rbkey);
//static void* chrus_loader_insertinator(const void *);

static int chrus_loader_comparator(chrus_rbkey search, chrus_rbkey nodekey) {
    return strcmp(search.keyptr, nodekey.keyptr);
}

static void chrus_loader_destructor(chrus_rbkey something) {
    return;
}

static inline void* load_bitmap(chrus_rbkey key);
static inline void* load_sample(chrus_rbkey key);

//static void* chrus_loader_insertinator(const void *key) {
    // this function needs to deal wi
//}
static inline void* load_bitmap(chrus_rbkey key) {
    return chrus_load_bitmap(key.keyptr);
}

static inline void* load_sample(chrus_rbkey key) {
    return al_load_sample(key.keyptr);
}

static inline void* load_font(chrus_rbkey key) {
    return chrus_font_create(key.keyptr);
}

void chrus_loader_init() {
    loader_trees[CHRUS_LOADER_BITMAP] = chrus_rbtree_create(chrus_loader_comparator, (chrus_rbtree_destructor)al_destroy_bitmap, load_bitmap);
    //loader_trees[CHRUS_LOADER_SCRIPT] = chrus_rbtree_create(strcmp, );
    loader_trees[CHRUS_LOADER_SAMPLE] = chrus_rbtree_create(chrus_loader_comparator, (chrus_rbtree_destructor)al_destroy_sample, load_sample);
    loader_trees[CHRUS_LOADER_FONT] = chrus_rbtree_create(chrus_loader_comparator, (chrus_rbtree_destructor)chrus_font_destroy, load_font);
}

void *chrus_loader_insert(CHRUS_LOADER index, chrus_rbkey key) {
    return chrus_rbtree_insert(loader_trees[index], key)->value;
}

void *chrus_loader_get(CHRUS_LOADER index, chrus_rbkey key) {
    return chrus_rbtree_find(loader_trees[index], key)->value;
}

void chrus_loader_deinit() {
    for (int i = 0; i < CHRUS_LOADER_END; i++) {
        if (!loader_trees[i]) continue;
        chrus_rbtree_destroy(loader_trees[i]);
    }
}