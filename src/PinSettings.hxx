#ifndef PINSETTINGS_H
#define PINSETTINGS_H

#include <iostream>
#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>

enum PadCtlType {PADCTL_BOOL, PADCTL_ENUM};

struct PadCtlField;

struct PadCtlValue {
    int value;
    std::string name;
    PadCtlField* field;
};

struct PadCtlField {
    int mask;
    int shift;
    std::string name;
    PadCtlType type;
    std::vector<PadCtlValue*> values;
};

class PinSettings {
    nlohmann::json _data;
    std::string socname;
    std::vector<PadCtlField*> padctlfields;
public:
    PinSettings(std::string fname);

    std::vector<PadCtlField*> getPadCtlFields();

    std::string name();

    friend std::ostream& operator<<(std::ostream& os, const PinSettings& ps);

    static PadCtlType parseType(std::string type);

    static int maskField(std::string field);

    static int shiftField(std::string field);

    static int parseValue(std::string value);

    static void apply(PadCtlValue* pcv, int &reg);

};

std::ostream& operator<<(std::ostream& os, const PinSettings& ps);

#endif // PINSETTINGS_H
