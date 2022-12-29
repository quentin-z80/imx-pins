#include "imx-pins.hxx"

#include <filesystem>

#include "Fl_Button_Tree.hxx"
#include "Fl_Button_Tree_Item.hxx"
#include "PinSettings.hxx"

#ifdef _WIN32
#include <windows.h>
#else
#include <limits.h>
#include <unistd.h>
#endif

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Check_Button.H>

namespace fs = std::filesystem;

Gui::Gui(int w, int h, const char *title, std::vector<PinSettings*> ps) : Fl_Double_Window(w, h, title)
{
    pinsettings_vec = ps;

    socselect = new Fl_Choice(5, 5, w - 10, 25);
    for (PinSettings *ps : pinsettings_vec)
    {
        socselect->add(ps->name().c_str(), 0, loadsoc, (void *)this);
    }

    pinopts = new Fl_Button_Tree(5, 35, w - 10, 330);
    pinopts->showroot(0);
    pinopts->callback(treecb, this);
    pinopts->when(FL_WHEN_NEVER);
    pinopts->selectmode(FL_TREE_SELECT_SINGLE);

    outbox = new Fl_Input(5, h - 30, w - 10, 25);
    outbox->value("0x0");
    outbox->when(FL_WHEN_ENTER_KEY_ALWAYS);
    outbox->callback(outboxcb, this);
    end();
}

Gui::~Gui()
{
    delete socselect;
    delete outbox;
    delete pinopts;
    for (PinSettings* ps : pinsettings_vec){delete ps;}
};

void Gui::loadsoc(Fl_Widget *w, void *data)
{
    Gui *gui = (Gui *)data;
    gui->pinopts->clear();
    int item = gui->socselect->value();
    PinSettings *ps = gui->pinsettings_vec.at(item);
    Fl_Button_Tree_Item *root = new Fl_Button_Tree_Item(gui->pinopts, "ROOT", BUT_NONE);
    Fl_Button_Tree_Item *parent;
    gui->pinopts->root(root);
    std::cout << "Loading SOC: " << ps->name() << "..." << std::endl;

    gui->outbox->value("0x0");
    gui->pinopts->begin();
    for (PadCtlField *p : ps->getPadCtlFields())
    {
        if (p->type == PADCTL_ENUM)
        {
            parent = (Fl_Button_Tree_Item *)gui->pinopts->add(root, p->name.c_str());
            parent = (Fl_Button_Tree_Item *)parent->replace(new Fl_Button_Tree_Item(gui->pinopts, p->name.c_str(), BUT_NONE));
            parent->user_data(nullptr);
            for (PadCtlValue *v : p->values)
            {
                gui->pinopts->add(parent, v->name.c_str())->replace(new Fl_Button_Tree_Item(gui->pinopts, v->name.c_str(), BUT_RADIO))->user_data(v);
            }
        }
        else if (p->type == PADCTL_BOOL)
        {
            gui->pinopts->add(root, p->name.c_str())->replace(new Fl_Button_Tree_Item(gui->pinopts, p->name.c_str(), BUT_CHECKBOX))->user_data(p->values.at(0));
        }
    }
    gui->pinopts->end();
    gui->pinopts->redraw();
}

void Gui::treecb(Fl_Widget *w, void *data)
{
    Gui *gui = (Gui *)data;
    // Fl_Button_Tree_Item* item = (Fl_Button_Tree_Item*) gui->pinopts->callback_item();
    // PadCtlValue* pcv = (PadCtlValue*) item->user_data();
    int reg = 0;

    for (Fl_Button_Tree_Item *i = (Fl_Button_Tree_Item *)gui->pinopts->first(); i; i = (Fl_Button_Tree_Item *)gui->pinopts->next(i))
    {
        if (i->user_data() == nullptr)
        {
            continue;
        }
        PadCtlValue *pcv = (PadCtlValue *)i->user_data();
        if (i->value() == 1)
        {
            PinSettings::apply(pcv, reg);
        }
    }
    std::stringstream ss;
    ss << "0x" << std::hex << reg;
    gui->outbox->value(ss.str().c_str());
}

void Gui::outboxcb(Fl_Widget *w, void *data)
{
    Gui *gui = (Gui *)data;
    int value = PinSettings::parseValue(gui->outbox->value());
    std::cout << "Setting tree items with value: " << value << std::endl;
    for (Fl_Button_Tree_Item *i = (Fl_Button_Tree_Item *)gui->pinopts->first(); i; i = (Fl_Button_Tree_Item *)gui->pinopts->next(i))
    {
        if (i->user_data() == nullptr)
        {
            continue;
        }
        PadCtlValue *pcv = (PadCtlValue *)i->user_data();
        i->value(PinSettings::check(pcv, value));
    }
}

std::filesystem::path abs_exe_directory() {
    #if defined(_MSC_VER)
        wchar_t path[FILENAME_MAX] = { 0 };
        GetModuleFileNameW(nullptr, path, FILENAME_MAX);
        return fs::path(path).parent_path().string();
    #else
        char path[FILENAME_MAX];
        ssize_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
        return fs::path(std::string(path, (count > 0) ? count: 0)).parent_path().string();
    #endif
}

int main(int argc, char **argv) {
    int ret;
    fs::path p = argv[0];
    fs::path pindir = abs_exe_directory() / ".." / "share" / "imx-pins" / "pinsettings";
    std::cout << "PinSettings directory: " << pindir << " " << std::endl;
    std::vector<PinSettings*> pinsettings_vec;
    std::cout << "Loading PinSettings..." << std::endl;
    for (const auto &entry : fs::directory_iterator(pindir)) {
        pinsettings_vec.push_back(new PinSettings(entry.path()));
    }

    Gui* gui = new Gui(400, 400, "imx-pins", pinsettings_vec);
    gui->show();
    ret = Fl::run();
    delete gui;
    return ret;
}
