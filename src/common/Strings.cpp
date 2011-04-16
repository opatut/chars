#include "Strings.hpp"

void define_strings() {
    StringManager::Add("mouse_moved");
    StringManager::Add("mouse_pressed");
    StringManager::Add("mouse_released");
    StringManager::Add("key_pressed");
    StringManager::Add("key_released");

    StringManager::Add("user_login");
    StringManager::Add("user_logout");

    StringManager::Add("char_goto");
    StringManager::Add("key_forward");
    StringManager::Add("key_back");
    StringManager::Add("key_left");
    StringManager::Add("key_right");
}
