#pragma once

#include <string>

#include "Data.hpp"
#include "Classifier.hpp"

class Menu {
public:
    static Menu *getInstance();

    int start();
private:
    Menu();
    void showMenu();
    std::string getCommand();
    void execCommand(std::string cmd);

    Data *data;
    Classifier *classifier;
};
