#ifndef FL_BUTTON_TREE_ITEM_H
#define FL_BUTTON_TREE_ITEM_H

#include "Fl_Button_Tree.hxx"

#include <FL/Fl_Tree.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>


enum ButtonType {BUT_CHECKBOX, BUT_RADIO, BUT_NONE};

class Fl_Dummy_Checkbox : public Fl_Check_Button {
public:
    Fl_Dummy_Checkbox(int x, int y, int w, int h, const char *l=0) : Fl_Check_Button(x, y, w, h, l) {}
};

class Fl_Dummy_Radio : public Fl_Round_Button {
public:
    Fl_Dummy_Radio(int x, int y, int w, int h, const char *l=0) : Fl_Round_Button(x, y, w, h, l) {}
};

class Fl_Button_Tree_Item : public Fl_Tree_Item {

    Fl_Button* button;
    ButtonType type;
    bool selected = false;

public:
    Fl_Button_Tree_Item(Fl_Button_Tree* tree, const char* l, ButtonType type);

    ~Fl_Button_Tree_Item();

    static void but_cb(Fl_Widget* w, void* data);

    int draw_item_content(int render);

    void set();

    int value();

    void deset();

    void deset_siblings();

};

#endif // FL_BUTTON_TREE_ITEM_H
