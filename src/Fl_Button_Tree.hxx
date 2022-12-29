#ifndef FL_BUTTON_TREE_H
#define FL_BUTTON_TREE_H

#include <FL/Fl_Tree.H>

class Fl_Button_Tree : public Fl_Tree {

public:
    Fl_Button_Tree(int X, int Y, int W, int H, const char *L=0);

    int handle(int event);

};

#endif // FL_BUTTON_TREE_H
