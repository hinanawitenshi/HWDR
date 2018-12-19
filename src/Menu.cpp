#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "Menu.hpp"
#include "DataMnist.hpp"

Menu::Menu() {
    data = nullptr;
}

Menu *Menu::getInstance() {
    static Menu *instance = nullptr;
    if (instance) {
        return instance;
    }
    instance = new Menu();
    return instance;
}

int Menu::start() {
    while (true) {
        showMenu();
        std::string cmd = getCommand();
        if (cmd == "exit") {
            break;
        }
        execCommand(cmd);
        printf("\nPress any key to return to menu...");
        std::cin.get();
    }
    return 0;
}

void Menu::showMenu() {
    system("clear");
    printf("Welcome to Handwritten Digit Recognition!\n\n");
    printf("Loaded data: ");
    if (!data) {
        printf("NULL\n\n");
    } else {
        printf("%s\n\n", data->name().c_str());
    }
    printf("load-mnist -- Load data (MNIST).\n");
    printf("show -- Show some data.\n");
    printf("3-bp -- Perform a 3-layer BP network.\n");
    printf("exit -- Exit the program.\n\n");
}

std::string Menu::getCommand() {
    std::string cmd;
    printf("Command: ");
    std::cin >> cmd;
    std::cin.get();
    return cmd;
}

void Menu::execCommand(std::string cmd) {
    if (cmd == "load-mnist") {
        data = new DataMnist();
    } else if (cmd == "show") {
        if (!data) {
            printf("[ERROR]data not loaded");
        } else {
            data->show();
        }
    } else if (cmd == "3-bp") {

    } else {
        printf("[ERROR]invalid command\n");
    }
}
