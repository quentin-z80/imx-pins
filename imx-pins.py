#!/usr/bin/env python3

from tabnanny import check
from fltk import *

import glob
import os
import json

class PadOption:
    def __init__(self, name: str, value: int, fields: int) -> None:
        self.__name = name
        self.__value = value
        self.__fields = fields

    def __str__(self) -> str:
        return f"<{self.__name}: {self.__value} << {self.field()}>"

    def name(self):
        return self.__name

    def value(self):
        return self.__value

    def fields(self):
        return self.__fields

    def field(self):
        """get index of first field for bit shifting
        """
        return (self.__fields&-self.__fields).bit_length()-1

class PinSettings:
    def __init__(self, file: str) -> None:
        with open(file) as f:
            fdata = f.read()
        jdata = json.loads(fdata)
        self.__name = jdata["NAME"]
        self.__data = []
        for name, settings in jdata["PAD_CTL"].items():
            self.__data.append(PadOption(name, settings["value"], settings["fields"]))

    def __str__(self) -> str:
        return f"<PinSettings: {self.__name}>"

    def __repr__(self) -> str:
        return str(self)

    def find(self, item: str) -> PadOption:
        for i in self.__data:
            if i.name() == item:
                return i
        raise KeyError

    def __getitem__(self, index: int) -> PadOption:
        return self.__data[index]

    def __len__(self) -> int:
        return len(self.__data)

    def __iter__(self):
        return iter(self.__data)

    def name(self) -> str:
        return self.__name

    @classmethod
    def getall(cls, dir: str) -> list["PinSettings"]:
        lst = []
        for root, subdirs, files in os.walk(dir):
            for file in files:
                if file.endswith(".json"):
                    fpath = os.path.join(root, file)
                    lst.append(cls(fpath))
        return lst

class Gui(Fl_Double_Window):

    def __init__(self, w: int, h: int, label: str):
        super().__init__(w, h, label)

        self.soclist = PinSettings.getall("pinsettings")
        print(self.soclist)

        self.cursoc = -1

        self.socselect = Fl_Choice(5, 5, w-10, 25)
        for i in range(len(self.soclist)):
            self.socselect.add(self.soclist[i].name(), 0, self.loadsoc, i)

        self.pinopts = Fl_Check_Browser(5, 35, w-10, 330)
        self.pinopts.when(FL_WHEN_CHANGED)
        self.pinopts.callback(self.setoutput)

        self.outbox = Fl_Input(5, h-30, w-10, 25)
        self.outbox.value("0x0")
        self.outbox.when(FL_WHEN_ENTER_KEY_ALWAYS)
        self.outbox.callback(self.outbox_cb)

    def loadsoc(self, wid: Fl_Choice, index: int):
        """loads pin config options into pinopts when soc is selected
        """
        self.pinopts.clear()
        self.outbox.value("0x0")
        print(self.soclist[index])
        self.cursoc = index
        for item in self.soclist[index]:
            self.pinopts.add(item.name())
        self.pinopts.redraw()

    def setoutput(self, wid: Fl_Check_Browser):
        """sets output value when a pin config option is changed
        """
        outval = 0x0
        for i in range(wid.nitems()):
            if wid.checked(i+1):
                padopt = self.soclist[self.cursoc][i]
                print(padopt)
                outval |= padopt.value() << padopt.field()
        print(outval)
        self.outbox.value(hex(outval))

    def outbox_cb(self, wid: Fl_Input):
        # parse value from str
        # can be 0xfff, 0b111, etc
        if self.cursoc == -1:
            return
        try:
            val = int(wid.value(), 0)
        except:
            return
        else:
            print(bin(val))
            self.pinopts.check_none()
            for i in range(len(self.soclist[self.cursoc])):

                padopt = self.soclist[self.cursoc][i]
                if (val & padopt.fields()) == (padopt.value() << padopt.field()):
                    self.pinopts.set_checked(i+1)

            self.pinopts.redraw()

win = Gui(400, 400, "imx-pins")

win.show()

Fl.run()