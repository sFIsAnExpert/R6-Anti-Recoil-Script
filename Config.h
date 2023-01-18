#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <map>

namespace fs = std::filesystem;

std::map<std::string, DWORD> keyList = {};

void initList() {
    if (keyList.empty()) {
        keyList.insert({ "0", 0x30 });
        keyList.insert({ "1", 0x31 });
        keyList.insert({ "2", 0x32 });
        keyList.insert({ "3", 0x33 });
        keyList.insert({ "4", 0x34 });
        keyList.insert({ "5", 0x35 });
        keyList.insert({ "6", 0x36 });
        keyList.insert({ "7", 0x37 });
        keyList.insert({ "8", 0x38 });
        keyList.insert({ "9", 0x39 });
        keyList.insert({ "F1", 0x70 });
        keyList.insert({ "F2", 0x71 });
        keyList.insert({ "F3", 0x72 });
        keyList.insert({ "F4", 0x73 });
        keyList.insert({ "F5", 0x74 });
        keyList.insert({ "F6", 0x75 });
        keyList.insert({ "F7", 0x76 });
        keyList.insert({ "F8", 0x77 });
        keyList.insert({ "F9", 0x78 });
        keyList.insert({ "F10", 0x79 });
        keyList.insert({ "F11", 0x7A });
        keyList.insert({ "F12", 0x7B });
        keyList.insert({ "A", 0x41 });
        keyList.insert({ "B", 0x42 });
        keyList.insert({ "C", 0x43 });
        keyList.insert({ "D", 0x44 });
        keyList.insert({ "E", 0x45 });
        keyList.insert({ "F", 0x46 });
        keyList.insert({ "G", 0x47 });
        keyList.insert({ "H", 0x48 });
        keyList.insert({ "I", 0x49 });
        keyList.insert({ "J", 0x4A });
        keyList.insert({ "K", 0x4B });
        keyList.insert({ "L", 0x4C });
        keyList.insert({ "M", 0x4D });
        keyList.insert({ "N", 0x4E });
        keyList.insert({ "O", 0x4F });
        keyList.insert({ "P", 0x50 });
        keyList.insert({ "Q", 0x51 });
        keyList.insert({ "R", 0x52 });
        keyList.insert({ "S", 0x53 });
        keyList.insert({ "T", 0x54 });
        keyList.insert({ "U", 0x55 });
        keyList.insert({ "V", 0x56 });
        keyList.insert({ "W", 0x57 });
        keyList.insert({ "X", 0x58 });
        keyList.insert({ "Y", 0x59 });
        keyList.insert({ "Z", 0x5A });
        keyList.insert({ "TAB", 0x09 });
        keyList.insert({ "BACKSPACE", 0x08 });
        keyList.insert({ "LSHIFT", 0xA0 });
        keyList.insert({ "RSHIFT", 0xA1 });
        keyList.insert({ "LCTRL", 0xA2 });
        keyList.insert({ "RCTRL", 0xA3 });
        keyList.insert({ "ALT", 0x12 });
        keyList.insert({ "CAPS", 0x14 });
        keyList.insert({ "ESC", 0x1B });
    }
}

void WriteFile(std::string path, std::vector<std::string> list) {
    std::ofstream file(path, std::ofstream::out | std::ofstream::trunc);
    if (file.is_open()) {
        for (std::string rV : list) {
            file << rV << "\n";
        }
    }
}

std::vector<std::string> ReadFile(std::string path) {
    std::ifstream file(path);
    if (file.is_open()) {
        std::string rV = ""; std::vector<std::string> list;
        while (std::getline(file, rV)) {
            list.push_back(rV);
        }

        return list;
    }
}

struct ConfigR6 {
    static inline std::string fPath = "config.txt";
};

struct KeyVec {
    std::string Name = "";
    DWORD Key = 0;
};

struct GuiVec {
    float Value = 0;
};

struct GuiVecStr {
    std::string Value = "";
};

struct Vec2d {
    int x = 0, y = 0, x2 = 0, y2 = 0;
    std::string Name = "";
};

GuiVec getGuiVec(std::string key) {
    initList();
    std::vector<std::string> rFile = ReadFile(ConfigR6::fPath);
    GuiVec vec;
    for (std::string str : rFile) {
        if (str.contains("\"")) {
            std::string rTmp = str.substr(str.find_first_of('.') + 1, str.find_last_of('"') - 1);
            if (rTmp.contains(key)) {
                std::string rV = str.substr(str.find_first_of(':') + 1);
                vec.Value = std::stof(rV);

                return vec;
            }
        }
    }
}

GuiVecStr getGuiVecStr(std::string key) {
    initList();
    std::vector<std::string> rFile = ReadFile(ConfigR6::fPath);
    GuiVecStr vec;
    for (std::string str : rFile) {
        if (str.contains("\"")) {
            std::string rTmp = str.substr(str.find_first_of('.') + 1, str.find_last_of('"') - 1);
            if (rTmp.contains(key)) {
                std::string rV = str.substr(str.find_first_of(':') + 1);
                vec.Value = rV;
                return vec;
            }
        }
    }
}

KeyVec getKey(std::string key) {
    initList();
    std::vector<std::string> rFile = ReadFile(ConfigR6::fPath);
    KeyVec vec;
    for (std::string str : rFile) {
        if (str.contains("\"")) {
            std::string rTmp = str.substr(str.find_first_of('.') + 1, str.find_last_of('"')-1);
            if (rTmp.contains(key)) {
                std::string rV = str.substr(str.find_first_of(':')+1);
                std::map<std::string, DWORD>::iterator it;
                for (it = keyList.begin(); it != keyList.end(); it++) {
                    if (it->first == rV) {
                        vec.Name = rV; vec.Key = it->second;
                        return vec;
                    }
                }
            }
        }
    }
}

std::vector<Vec2d> getData() {
    std::vector<Vec2d> vecList = {};
    if (fs::exists(ConfigR6::fPath)) {
        std::vector<std::string> rFile = ReadFile(ConfigR6::fPath);
        for (std::string str : rFile) {
            Vec2d vec2d;
            if (!str.contains("\"")) {
                std::string rV = str.substr(0, str.find_first_of(':'));
                vec2d.Name = rV;
                rV.clear();
                rV = str.substr(str.find_first_of(':') + 1, str.length());
                std::string Y = rV.substr(rV.find_first_of('Y') + 2, rV.find_first_of(','));
                std::string X = rV.substr(rV.find_first_of('X') + 2);
                rV.clear();
                rV = str.substr(str.find_first_of(';') + 1, str.length());
                std::string Y2 = rV.substr(rV.find_first_of('Y') + 3, rV.find_first_of(','));
                std::string X2 = rV.substr(rV.find_first_of('X') + 3);

                vec2d.x2 = std::stoi(X2);
                vec2d.y2 = std::stoi(Y2);
                vec2d.x = std::stoi(X);
                vec2d.y = std::stoi(Y);

                vecList.push_back(vec2d);
            }
            
        }
    }
    else
        MessageBoxA(0, "Config Doesn't Exist", "Error", 0);

    return vecList;
}
