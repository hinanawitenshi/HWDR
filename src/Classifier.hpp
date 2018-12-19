#pragma once

class Classifier {
public:
    virtual std::string name() = 0;
    virtual void train() = 0;
    virtual void pick() = 0;
private:
};
