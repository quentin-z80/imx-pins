#!/usr/bin/env python3

from fltk import *

import glob
import os
import json

class PadValue:
    def __init__(self, name: str, value: int) -> None:
        self.__name = name
        self.__value = value

    def name(self):
        return self.__name

    def escaped_name(self) -> str:
        return self.__name.replace("/", r"\/")

    def value(self):
        return self.__value

    def __repr__(self) -> str:
        return f'PadValue("{self.__name}", {self.__value})'

class PadOption:
    def __init__(self, fields: int, name: str, opt_type: str, values: list[dict] = []) -> None:
        self.__fields = fields
        self.__name = name
        self.__type = opt_type
        self.__values = []

        if opt_type == "bool":
            self.__values.append(PadValue("Enabled", 1))
            self.__values.append(PadValue("Disabled", 0))
        elif opt_type == "bool-inverted":
            self.__values.append(PadValue("Enabled", 0))
            self.__values.append(PadValue("Disabled", 1))
        elif opt_type == "enum":
            for item in values:
                value = self.parseValue(item["value"])
                self.__values.append(PadValue(item["name"], value))
        else:
            raise ValueError("Invalid value type")

    @staticmethod
    def parseValue(value: str) -> int:
        if value.startswith("0x"):
            return int(value, 16)
        elif value.startswith("0b"):
            return int(value, 2)
        else:
            return int(value)

    def __repr__(self) -> str:
        return f'PadOption({self.__fields}, "{self.__name}", "{self.__type}", {self.__values})'

    def name(self):
        return self.__name

    def escaped_name(self) -> str:
        return self.__name.replace("/", r"\/")

    def fields(self):
        return self.__fields

    @DeprecationWarning
    def field(self):
        """get index of first field for bit shifting
        """
        return (self.__fields&-self.__fields).bit_length()-1

    def __iter__(self):
        return iter(self.__values)

class PinSettings:
    def __init__(self, file: str) -> None:
        with open(file) as f:
            fdata = f.read()
        jdata = json.loads(fdata)
        self.__name = jdata["NAME"]
        self.__data = []
        self.__fname = file
        for item in jdata["PAD_CTL"]:
            bitmask = self.parseField(item["fields"])
            if "values" in item:
                values = item["values"]
            else:
                values = []
            pad = PadOption(bitmask, item["name"], item["type"], values)
            self.__data.append(pad)

    @staticmethod
    def parseField(field: str) -> int:
        """return bitmask for field
        >>> parseField("5")
        32
        >>> parseField("2:0")
        7
        """

        if ":" in field:
            start, end = field.split(":")
            start = int(start)
            end = int(end)
            return (1 << (start-end+1)) - 1 << end
        else:
            return 1 << int(field)

    def __str__(self) -> str:
        return f"<PinSettings: {self.__name}>"

    def __repr__(self) -> str:
        return f'PinSettings("{self.__fname}")'

    def __iter__(self):
        return iter(self.__data)

    def name(self):
        return self.__name

    @classmethod
    def getall(cls, path: str) -> list:
        """get all pin settings from path
        """
        files = glob.glob(os.path.join(path, "*.json"))
        return [cls(file) for file in files]

class Gui(Fl_Double_Window):

    def __init__(self, w: int, h: int, label: str):
        super().__init__(w, h, label)

        self.soclist = PinSettings.getall("pinsettings")
        print(self.soclist)

        self.cursoc = -1
        self.valueitems = []

        self.socselect = Fl_Choice(5, 5, w-10, 25)
        for i in range(len(self.soclist)):
            self.socselect.add(self.soclist[i].name(), 0, self.loadsoc, i)

        self.pinopts = Fl_Tree(5, 35, w-10, 330)
        self.pinopts.when(FL_WHEN_CHANGED)
        self.pinopts.showroot(0)
        self.pinopts.callback(self.treecb)
        self.pinopts.selectmode(FL_TREE_SELECT_SINGLE)

        self.outbox = Fl_Input(5, h-30, w-10, 25)
        self.outbox.value("0x0")
        self.outbox.when(FL_WHEN_ENTER_KEY_ALWAYS)
        #self.outbox.callback(self.outbox_cb)

    def treecb(self, wid: Fl_Tree):
        print("test")

    def loadsoc(self, wid: Fl_Choice, index: int):
        """loads pin config options into pinopts tree when soc is selected
        """
        self.pinopts.clear()
        self.outbox.value("0x0")
        print(self.soclist[index])
        self.cursoc = index
        for item in self.soclist[index]:
            titem = self.pinopts.add(item.escaped_name())
            titem.user_data(item)
            for val in item:
                print(val.name())
                self.valueitems.append(Fl_Round_Button(0, 0, 350, 0, val.name()))
                valitem = self.pinopts.add(titem, val.name())
                print(valitem.widget())

        self.pinopts.redraw()

    def valbutcb(self, wid: Fl_Round_Button):
        """callback for value buttons
        """
        print(wid.user_data())

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
            #self.pinopts.check_none()
            for i in range(len(self.soclist[self.cursoc])):

                padopt = self.soclist[self.cursoc][i]
                #if (val & padopt.fields()) == (padopt.value() << padopt.field()):
                    #self.pinopts.set_checked(i+1)

            self.pinopts.redraw()

win = Gui(400, 400, "imx-pins")

win.show()

Fl.run()
