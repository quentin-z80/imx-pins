#include "PinSettings.hxx"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

PinSettings::PinSettings(std::string fname) {
    std::ifstream f(fname, std::ifstream::in);
    f >> _data;
    socname = _data["NAME"];

    for (json::iterator it = _data["PAD_CTL"].begin(); it != _data["PAD_CTL"].end(); ++it) {
        PadCtlField* pcf = new PadCtlField;
        pcf->mask = maskField((*it)["fields"]);
        pcf->shift = shiftField((*it)["fields"]);
        pcf->name = (*it)["name"];
        pcf->type = parseType((*it)["type"]);
        if (pcf->type == PADCTL_ENUM) {
            for (json::iterator it2 = (*it)["values"].begin(); it2 != (*it)["values"].end(); ++it2) {
                PadCtlValue* pcv = new PadCtlValue;
                pcv->value = parseValue((*it2)["value"]);
                pcv->name = (*it2)["name"];
                pcv->field = pcf;
                pcf->values.push_back(pcv);
            }
        } else if (pcf->type == PADCTL_BOOL){
            PadCtlValue* pcv = new PadCtlValue;
            pcv->field = pcf;
            pcv->value = 1;
            pcv->name = pcf->name;
            pcf->values.push_back(pcv);
        }
        padctlfields.push_back(pcf);
    }
};

PinSettings::~PinSettings() {
    for (auto pcf : padctlfields) {
        for (auto pcv : pcf->values) {
            delete pcv;
        }
        delete pcf;
    }
}

std::vector<PadCtlField*> PinSettings::getPadCtlFields() { return padctlfields; };

std::string PinSettings::name() { return socname; };

PadCtlType PinSettings::parseType(std::string type) {
    if (type == "enum") return PADCTL_ENUM;
    return PADCTL_BOOL;
}

int PinSettings::maskField(std::string field) {
    if (field.find(':') != std::string::npos) {
        int start = std::stoi(field.substr(0, field.find(':')));
        int end = std::stoi(field.substr(field.find(':')+1));
        return (1 << (start-end+1)) - 1 << end;
    }
    return 1 << std::stoi(field);
}

int PinSettings::shiftField(std::string field) {
    if (field.find(':') != std::string::npos) {
        return std::stoi(field.substr(field.find(':')+1));
    }
    return std::stoi(field);
}

int PinSettings::parseValue(std::string value) {
    if (value.find("0x") != std::string::npos) {
        return std::stoi(value.substr(2), nullptr, 16);
    }
    if (value.find("0b") != std::string::npos) {
        return std::stoi(value.substr(2), nullptr, 2);
    }
    return std::stoi(value);
}

void PinSettings::apply(PadCtlValue* pcv, int &reg) {
    reg &= ~(pcv->field->mask);
    reg |= pcv->value << pcv->field->shift;
}

int PinSettings::check(PadCtlValue* pcv, int reg) {
    return ((reg & pcv->field->mask) >> pcv->field->shift) == pcv->value;
}

std::ostream& operator<<(std::ostream& os, const PinSettings& ps) {
        return os << "<PinSettings: " << ps.socname << ">";
}
