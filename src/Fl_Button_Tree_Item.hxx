#ifndef FL_BUTTON_TREE_ITEM_H
#define FL_BUTTON_TREE_ITEM_H

#include "Fl_Button_Tree.hxx"

#include <FL/Fl_Tree.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>


enum ButtonType {BUT_CHECKBOX, BUT_RADIO, BUT_NONE};

class Fl_Button_Tree_Item : public Fl_Tree_Item {

    Fl_Button* button;
    ButtonType type;
    bool selected = false;

public:
    Fl_Button_Tree_Item(Fl_Button_Tree* tree, const char* l, ButtonType type);

    ~Fl_Button_Tree_Item();

    static void but_cb(Fl_Widget* w, void* data);

    int draw_item_content(int render);

    void toggle();

    int value();

    void value(int val);

    void clear();

    void clear_siblings();

};

class Fl_Dummy_Round : public Fl_Round_Button {
    Fl_Button_Tree* tree;
public:
    Fl_Dummy_Round(int x, int y, int w, int h, Fl_Button_Tree* tree);

    int handle(int event);
};

class Fl_Dummy_Check : public Fl_Check_Button {
    Fl_Button_Tree* tree;
public:
    Fl_Dummy_Check(int x, int y, int w, int h, Fl_Button_Tree* tree);

    int handle(int event);
};

#endif // FL_BUTTON_TREE_ITEM_H
