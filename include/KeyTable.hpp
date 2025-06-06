#pragma once
#ifndef __KEY_TABLE_HPP__
#define __KEY_TABLE_HPP__

#include <Windows.h>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cctype>


namespace KeyTable {

inline std::string toUpper(const std::string &str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return std::toupper(c);
    });
    return result;
}

// Mapping table for keyboard keys.
// The key is a string (as read from the ini), and the value is the corresponding virtual-key code.
static const std::unordered_map<std::string, int> map = {
    // Letters (only uppercase)
    {"A", 'A'}, {"B", 'B'}, {"C", 'C'}, {"D", 'D'}, {"E", 'E'},
    {"F", 'F'}, {"G", 'G'}, {"H", 'H'}, {"I", 'I'}, {"J", 'J'},
    {"K", 'K'}, {"L", 'L'}, {"M", 'M'}, {"N", 'N'}, {"O", 'O'},
    {"P", 'P'}, {"Q", 'Q'}, {"R", 'R'}, {"S", 'S'}, {"T", 'T'},
    {"U", 'U'}, {"V", 'V'}, {"W", 'W'}, {"X", 'X'}, {"Y", 'Y'}, {"Z", 'Z'},

    // Digits
    {"0", '0'}, {"1", '1'}, {"2", '2'}, {"3", '3'}, {"4", '4'},
    {"5", '5'}, {"6", '6'}, {"7", '7'}, {"8", '8'}, {"9", '9'},

    // Function keys
    {"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3}, {"F4", VK_F4}, {"F5", VK_F5},
    {"F6", VK_F6}, {"F7", VK_F7}, {"F8", VK_F8}, {"F9", VK_F9}, {"F10", VK_F10},
    {"F11", VK_F11}, {"F12", VK_F12},

    // Arrow keys and common keys
    {"VK_LEFT", VK_LEFT}, {"VK_RIGHT", VK_RIGHT}, {"VK_UP", VK_UP}, {"VK_DOWN", VK_DOWN},
    {"SPACE", VK_SPACE}, {"ENTER", VK_RETURN}, {"RETURN", VK_RETURN}, {"ESC", VK_ESCAPE},
    {"BACKSPACE", VK_BACK}, {"TAB", VK_TAB}, {"SHIFT", VK_SHIFT}, {"CTRL", VK_CONTROL},
    {"ALT", VK_MENU}, {"CAPSLOCK", VK_CAPITAL}, {"NUMLOCK", VK_NUMLOCK}, {"SCROLLLOCK", VK_SCROLL},
    {"PRINTSCREEN", VK_SNAPSHOT}, {"PAUSE", VK_PAUSE}, {"INSERT", VK_INSERT}, {"DELETE", VK_DELETE},
    {"HOME", VK_HOME}, {"END", VK_END}, {"PGUP", VK_PRIOR}, {"PGDN", VK_NEXT}
};

} // namespace KeyTable

#endif // __KEY_TABLE_HPP__