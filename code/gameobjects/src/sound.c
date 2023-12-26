#include "../include/sound.h"

chrus_sound *chrus_sound_create(const char *source) {
    chrus_sound *new_sound = malloc(sizeof(chrus_sound));
    new_sound->stream = NULL;

    chrus_sound_load(new_sound, source);
    // well... even if you mess up loading the song, there should be some default properties
    new_sound->speed = 1;
    new_sound->pan = ALLEGRO_AUDIO_PAN_NONE;
    new_sound->gain = 1;
    new_sound->playmode = ALLEGRO_PLAYMODE_ONCE;

    return new_sound;
}

void chrus_sound_load(chrus_sound* restrict this, const char *source) {
    if (this->stream != NULL) {
        al_destroy_audio_stream(this->stream);
    }
    this->stream = NULL;

    if (source != NULL) {
        this->stream = al_load_audio_stream(source, 4, 4096);
    } else {
        return;
    }

    if (this->stream != NULL) {
        this->length = al_get_audio_stream_length_secs(this->stream);
        al_attach_audio_stream_to_mixer(this->stream, al_get_default_mixer());
    } else {
        printf("you seem to have failed to load this audio file...\n");
        this->length = 0;
    }
}

void chrus_sound_play(chrus_sound* restrict this) {
    if (this->stream == NULL) return;
    bool result = al_set_audio_stream_playing(this->stream, true);
    if (!result) {
        printf("sound did not play\n");
    } else {
        printf("sound played successfully\n");
    }
}

void chrus_sound_stop(chrus_sound* restrict this) {
    if (this->stream == NULL) return;
    al_set_audio_stream_playing(this->stream, false);
}

void chrus_sound_free(chrus_sound* restrict this) {
    printf("freeing sound\n");
    if (this->stream != NULL) {
        al_set_audio_stream_playing(this->stream, false);
        al_destroy_audio_stream(this->stream);
    }

    free(this);
}