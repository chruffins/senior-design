#include "../include/chrus.h"

static void must_init(int test, const char *failure) {
    if (test) return;

    printf("%s failed to load!", failure);
    exit(-1);
}

bool chrus_init() {
    //must_init(al_install_audio(), "allegro audio");
    //must_init(al_init_acodec_addon(), "allegro codec");
    //must_init(al_reserve_samples(16), "reserve samples");

    return true;
}

void chrus_deinit() {
    al_uninstall_audio();
}