#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "Menu.hpp"
#include "DataMnist.hpp"
#include "ClassifierBPNet.hpp"

Menu::Menu() {
    data = nullptr;
    classifier = nullptr;
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
        printf("NULL\n");
    } else {
        printf("%s\n", data->name().c_str());
    }
    printf("Classifier: ");
    if (!classifier) {
        printf("NULL\n\n");
    } else {
        printf("%s\n\n", classifier->name().c_str());
    }
    printf("load-mnist -- Load data (MNIST).\n");
    if (data) printf("show -- Show some data.\n");
    if (data) printf("bp-net -- Train and test a 3-layer BP network(784-20-10).\n");
    if (classifier) printf("test -- Pick an image from the test set.\n");
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
    } else if (cmd == "bp-net") {
        classifier = new ClassifierBPNet(20, 80000, 0.01, data);
        classifier->train();
    } else if (cmd == "test" || cmd == "t") {
        classifier->pick();
    } else {
        printf("[ERROR]invalid command\n");
    }
}
