#include "../include/sound.h"

chrus_sound* chrus_sound_create(const char* source) {
    chrus_sound *new_sound = malloc(sizeof(chrus_sound));
    new_sound->sample = NULL;

    chrus_sound_load(new_sound, source);
    new_sound->speed = 1;
    new_sound->pan = ALLEGRO_AUDIO_PAN_NONE;
    new_sound->gain = 1;
    new_sound->playmode = ALLEGRO_PLAYMODE_ONCE;

    return new_sound;
}

void chrus_sound_load(chrus_sound* restrict this, const char *source) {
    if (this->sample != NULL) {
        // TODO: figure this out
    }
    this->sample = NULL;

    if (source != NULL) {
        this->sample = chrus_loader_insert(CHRUS_LOADER_SAMPLE, source);
        this->sample = al_load_sample(source);
    }

    if (this->sample != NULL) return;

    this->sound = al_create_sample_instance(this->sample);
}

void chrus_sound_play(chrus_sound* restrict this) {
    if (!this->sound) return;

    al_play_sample_instance(this->sound);
}

void chrus_sound_stop(chrus_sound* restrict this) {
    if (!this->sound) return;

    al_stop_sample_instance(this->sound);
}

void chrus_sound_destroy(chrus_sound* restrict this) {
    printf("freeing sound\n");
    if (this->sound) {
        al_stop_sample_instance(this->sound);
        al_destroy_sample_instance(this->sound);
    }
    
}

chrus_audiostream *chrus_audiostream_create(const char *source) {
    chrus_audiostream *new_sound = malloc(sizeof(chrus_audiostream));
    new_sound->stream = NULL;

    chrus_audiostream_load(new_sound, source);
    // well... even if you mess up loading the song, there should be some default properties
    new_sound->speed = 1;
    new_sound->pan = ALLEGRO_AUDIO_PAN_NONE;
    new_sound->gain = 1;
    new_sound->playmode = ALLEGRO_PLAYMODE_ONCE;

    return new_sound;
}

void chrus_audiostream_load(chrus_audiostream* restrict this, const char *source) {
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

void chrus_audiostream_play(chrus_audiostream* restrict this) {
    if (this->stream == NULL) return;
    bool result = al_set_audio_stream_playing(this->stream, true);
    if (!result) {
        printf("stream did not play\n");
    } else {
        printf("stream played successfully\n");
    }
}

void chrus_audiostream_stop(chrus_audiostream* restrict this) {
    if (this->stream == NULL) return;
    al_set_audio_stream_playing(this->stream, false);
}

void chrus_audiostream_destroy(chrus_audiostream* restrict this) {
    printf("freeing stream\n");
    if (this->stream != NULL) {
        al_set_audio_stream_playing(this->stream, false);
        al_destroy_audio_stream(this->stream);
    }

    free(this);
}