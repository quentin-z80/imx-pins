#ifndef IMX_PINS_H
#define IMX_PINS_H

#include <vector>
#include <string>

#include "PinSettings.hxx"
#include "Fl_Button_Tree.hxx"

#include <FL/Fl_Widget.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Double_Window.H>

class Gui : public Fl_Double_Window {
    std::vector<PinSettings*> pinsettings_vec;
    Fl_Choice* socselect;
    Fl_Button_Tree* pinopts;
    Fl_Input* outbox;
public:
    Gui(int w, int h, const char *title, std::vector<PinSettings*> ps);

    ~Gui();

    static void loadsoc(Fl_Widget* w, void* data);

    static void treecb(Fl_Widget* w, void* data);

    static void outboxcb(Fl_Widget* w, void* data);
};

int main(int argc, char** argv);

#endif // IMX_PINS_H
