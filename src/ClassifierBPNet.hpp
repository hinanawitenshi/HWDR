#pragma once

#include <vector>

#include "Classifier.hpp"
#include "Data.hpp"

class ClassifierBPNet : public Classifier {
public:
    ClassifierBPNet(int nUnit, int nEpoch, double learningRate, const Data *data);
    void train();
    double test();
    void pick();
    std::string name();
private:
    void normalInit();
    void normalInit1D(std::vector<double> &v);
    void normalInit2D(std::vector< std::vector<double> > &v);
    void setZ0(const std::vector<uint8_t> &img);
    uint8_t maxA2();
    void forwardFeed();
    void backPropagation(uint8_t label);
    double cost(uint8_t label);
    double costPD(uint8_t label, int i);
    double relu(double x);
    double reluD(double x);
    void softmax(std::vector<double> &src, std::vector<double> &dst);
    double softmaxD(double prob);

    int nUnit, nEpoch;
    double learningRate;
    const Data *data;
    std::vector< std::vector<double> > W1, W2;
    std::vector<double> Z0, B1, B2, Z1, Z2, A1, A2;
};
