#pragma once

class Data {
public:
    virtual std::string name() const = 0;
    virtual void show() const = 0;
};
