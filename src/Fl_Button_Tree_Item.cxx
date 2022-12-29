#include "Fl_Button_Tree_Item.hxx"

#include <iostream>

#include <FL/Fl.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>

Fl_Button_Tree_Item::Fl_Button_Tree_Item(Fl_Button_Tree* tree, const char* l, ButtonType type) : Fl_Tree_Item(tree) {
    label(l);
    this->type = type;
    switch (type) {
    case BUT_RADIO:
        button = new Fl_Round_Button(0, 0, 20, 20);
        break;
    case BUT_CHECKBOX:
        button = new Fl_Check_Button(0, 0, 20, 20);
        break;
    case BUT_NONE:
        button = nullptr;
        return;
    default:
        break;
    }
    button->clear();
    button->callback(but_cb, this);
}

Fl_Button_Tree_Item::~Fl_Button_Tree_Item() {
    delete button;
}

void Fl_Button_Tree_Item::but_cb(Fl_Widget* w, void* data) {
    Fl_Button_Tree_Item* item = (Fl_Button_Tree_Item*) data;
    int val = item->button->value();
    std::cout << "button callback: " << val << std::endl;
    item->tree()->deselect_all();
    item->select();
    item->tree()->redraw();
    item->tree()->callback_item(item);
    item->tree()->do_callback();
}

int Fl_Button_Tree_Item::draw_item_content(int render) {
    int X=label_x(), Y=label_y(), W=label_w(), H=label_h();
    const char *text = label() ? label() : "";
    if ( render ) {
        fl_color(drawbgcolor()); fl_rectf(X,Y,W,H);
        fl_font(labelfont(), labelsize());
        fl_color(drawfgcolor());
        if (type == BUT_NONE) {
            fl_draw(text, X,Y,W,H, FL_ALIGN_LEFT);
        } else {
            fl_draw(text, X+20,Y,W,H, FL_ALIGN_LEFT);
            button->position(X, Y);
            ((Fl_Widget*)button)->draw();
        }
    }
    int lw=0, lh=0;
    fl_measure(text, lw, lh);
    return X + lw + 20;
}

void Fl_Button_Tree_Item::set() {
    switch (type) {
        case BUT_RADIO:
            selected ? button->clear() : button->set();
            selected = !selected;
            deset_siblings();
            break;
        case BUT_CHECKBOX:
            selected ? button->clear() : button->set();
            selected = !selected;
            break;
        default:
            return;
    }
    button->redraw();
}

int Fl_Button_Tree_Item::value() {
    return button->value();
}

void Fl_Button_Tree_Item::deset() {
    if (type == BUT_NONE) return;
    button->clear();
    selected = false;
}

void Fl_Button_Tree_Item::deset_siblings() {
    //iterate over siblings and deselect them
    Fl_Tree_Item* parent = this->parent();
    for (int i = 0; i < parent->children(); i++) {
        if (parent->child(i) != this) {
            ((Fl_Button_Tree_Item*)parent->child(i))->deset();
        }
    }
}
