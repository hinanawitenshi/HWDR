#pragma once

#include <vector>

#include "Classifier.hpp"
#include "Data.hpp"

class Classifier3BP : public Classifier {
public:
    Classifier3BP(int nUnit, const Data *data);
    void train();
    void test();
    void eval();
private:
    int nUnit;
    const Data *data;
    std::vector< std::vector<double> > W0, W1;
    std::vector<double> L;
};
