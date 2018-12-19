#pragma once

#include <vector>

#include "Data.hpp"

class DataMnist : public Data {
public:
    DataMnist();
    // DataMnist(const DataMnist &obj);
    // ~DataMnist();
    std::string name() const;
    void show() const;
private:
    void loadIdx3(std::vector< std::vector<uint8_t> > &v, FILE *f);
    void loadIdx1(std::vector<uint8_t> &v, FILE *f);
    int convertEndian(int num);
    void printImg(const std::vector<uint8_t> &img) const;

    std::vector< std::vector<uint8_t> > trainingImg;
    std::vector<uint8_t> trainingLbl;
    std::vector< std::vector<uint8_t> > testImg;
    std::vector<uint8_t> testLbl;
};
