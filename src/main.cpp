#include <cstdlib>
#include <ctime>

#include "Menu.hpp"

int main() {
    srand(time(NULL));
    Menu *menu = Menu::getInstance();
    return menu->start();
}
