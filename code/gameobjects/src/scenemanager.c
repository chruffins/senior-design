#include "../include/scenemanager.h"

chrus_scene_manager* chrus_scene_manager_create() {
    chrus_scene_manager *nsm = malloc(sizeof(chrus_scene_manager));
    memset(nsm->scenes, 0, sizeof(scene*)*MAX_SCENES);

    nsm->top = -1;
}

void chrus_scene_manager_destroy(chrus_scene_manager* scene_manager) {
    free(scene_manager);
}