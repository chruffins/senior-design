#include "gui.h"

gui_dim gui_pos_create(float ax, float ay, float ox, float oy) {
    return (gui_dim){ ax, ox, ay, oy };
}

gui_point gui_pos_to_point(gui_dim pos, int scw, int sch) {
    return (gui_point){ (pos.ax * scw) + pos.ox, (pos.ay * sch) + pos.ay };
}

int gui_dim_to_point(int total, float absolute, int offset) {
    return (int)(total * absolute) + offset;
}

void gui_label_draw(gui_element* this) {
    al_draw_text(this->label.font, this->label.font_color, this->x, this->y, 0, this->label.text);
}

void gui_bitmap_draw(gui_element* this) {
    if (this->bitmap.bitmap) {
        al_draw_bitmap(this->bitmap.bitmap, this->x, this->y, 0);
    }
}

gui_element gui_label_create() {
    gui_element label;
    memset(&label, 0, sizeof(label));

    label.type = GUI_TYPE_LABEL;
    label.x = 50;
    label.y = 50;
    label.label.font = al_create_builtin_font();
    label.label.text = "Hello world!";
    label.label.font_color = al_map_rgb(255, 255, 255);
    label.draw = gui_label_draw;

    return label;
}

gui_element gui_bitmap_create() {
    gui_element bitmap;
    memset(&bitmap, 0, sizeof(bitmap));

    bitmap.type = GUI_TYPE_LABEL;
    bitmap.draw = gui_bitmap_draw;

    return bitmap;
}
