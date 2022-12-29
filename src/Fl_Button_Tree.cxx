#include "Fl_Button_Tree.hxx"

#include <iostream>

#include "Fl_Button_Tree_Item.hxx"

#include <FL/Fl.H>
#include <FL/Fl_Tree.H>

Fl_Button_Tree::Fl_Button_Tree(int X, int Y, int W, int H, const char *L) : Fl_Tree(X, Y, W, H, L) {

}

int Fl_Button_Tree::handle(int event) {
    if (event == FL_RELEASE) {
        std::cout << "release" << std::endl;
    }
    // int ret;
    // Fl_Button_Tree_Item* item = (Fl_Button_Tree_Item*) find_clicked();
    // std::cout << "event: " << event << std::endl;
    // if (event == FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE && item
    //     && Fl::event_x() > item->label_x()
    //     && Fl::event_x() < item->label_x() + item->label_w()) {
    //         std::cout << "clicked" << std::endl;
    //         item->set();
    //         ret = Fl_Tree::handle(event);
    //         callback_item(item);
    //         do_callback();
    //         return ret;
    //     }
    return Fl_Tree::handle(event);
}
