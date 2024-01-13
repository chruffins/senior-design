#include "../include/serializer.h"

static inline void fprintspaces(FILE* restrict fp, int tabs);
static inline void fprintnewfield(FILE* restrict fp, bool last);
static void fprintint(FILE* restrict fp, const char* key, int data, int tabs, bool last);
static void fprintptr(FILE* restrict fp, const char* key, void* data, int tabs, bool last);
static void fprintstr(FILE* restrict fp, const char* key, const char* data, int tabs, bool last);
static void fprintfloat(FILE* restrict fp, const char* key, double data, int tabs, bool last);
static const char *safestr(const char* str);

static const char *chrus_node_type_to_str(enum CHRUS_NODE_TYPES);

static void serialize_children(FILE* restrict fp, chrus_node_vec* children, int tabs);
static void serialize_node(FILE* restrict fp, chrus_node* node, int tabs);

static inline void fprintspaces(FILE* restrict fp, int tabs) {
    fprintf(fp, "%*c", tabs * 2, ' ');
}

static inline void fprintnewfield(FILE* restrict fp, bool last) {
    if (!last) {
        fputc(',', fp);
    }
    fputc('\n', fp);
}

static void fprintint(FILE* restrict fp, const char* key, int data, int tabs, bool last) {
    fprintspaces(fp, tabs);
    fprintf(fp, "\"%s\": \"%d\"", key, data);
    fprintnewfield(fp, last);
}

static void fprintptr(FILE* restrict fp, const char* key, void* data, int tabs, bool last) {
    fprintspaces(fp, tabs);
    fprintf(fp, "\"%s\": \"%p\"", key, data);
    fprintnewfield(fp, last);
}

static void fprintstr(FILE* restrict fp, const char* key, const char* data, int tabs, bool last) {
    fprintspaces(fp, tabs);
    fprintf(fp, "\"%s\": \"%s\"", key, safestr(data));
    fprintnewfield(fp, last);
}

static void fprintfloat(FILE* restrict fp, const char* key, double data, int tabs, bool last) {
    fprintspaces(fp, tabs);
    fprintf(fp, "\"%s\": \"%f\"", key, data);
    fprintnewfield(fp, last);
}

static const char *safestr(const char* str) {
    if (str == NULL) return "";

    return str;
}

static const char *chrus_node_type_to_str(enum CHRUS_NODE_TYPES type) {
    switch (type)
    {
    case CHRUS_NODE_UNINITIALIZED:
        return "uninitd";
    case CHRUS_NODE_CAMERA:
        return "camera";
    case CHRUS_NODE_SCRIPT:
        return "script";
    case CHRUS_NODE_SPRITE:
        return "sprite";
    case CHRUS_NODE_SOUND:
        return "sound";
    case CHRUS_NODE_AUDIOSTREAM:
        return "audiostream";
    }
    return "invalid";
}

static void serialize_children(FILE* restrict fp, chrus_node_vec* children, int tabs) {
    fprintspaces(fp, tabs);
    fprintf(fp, "\"children\": [\n");
    for (int i = 0; i < children->size-1; i++) {
        serialize_node(fp, children->data[i], tabs+1);
        fputs(",\n", fp);
    }
    serialize_node(fp, children->data[children->size-1], tabs+1);
    fputc('\n', fp);
    fprintspaces(fp, tabs);
    fprintf(fp, "]\n");
}

static void serialize_node(FILE* restrict fp, chrus_node* node, int tabs) {
    fprintspaces(fp, tabs);
    fprintf(fp, "{\n");
    fprintstr(fp, "name", node->name, tabs+1, false);
    fprintstr(fp, "type", chrus_node_type_to_str(node->type), tabs+1, false);
    fprintptr(fp, "parent", node->parent, tabs+1, true);
    fprintspaces(fp, tabs);
    fputc('}', fp);
}

void chrus_serializer_save_scene(chrus_scene* scene, const char* filename) {
    FILE *fp = fopen(filename, "w");
    //int level = 0;
    fprintf(fp, "{\n");
    //fprintspaces(fp, 1);
    fprintstr(fp, "name", scene->name, 1, false);
    fprintptr(fp, "current_camera", scene->current_camera, 1, false);
    serialize_children(fp, &scene->children, 1);
    fprintf(fp, "}");
    //fprintf(fp, "{\n  \"name\": \"%s\",\n  \"current_camera\": \"%p\"}", safestr(scene->name), (void*)scene->current_camera);

    fclose(fp);
}