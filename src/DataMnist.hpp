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
    int dim() const;
    void fetchNextTrain(std::vector<uint8_t> &img, uint8_t &label) const;
    void getTest(std::vector< std::vector<uint8_t> > &imgs,
        std::vector<uint8_t> &labels) const;
    void pickTest(std::vector<uint8_t> &img, uint8_t &label) const;
    void printImg(const std::vector<uint8_t> &img) const;
private:
    void loadIdx3(std::vector< std::vector<uint8_t> > &v, FILE *f);
    void loadIdx1(std::vector<uint8_t> &v, FILE *f);
    int convertEndian(int num);

    std::vector< std::vector<uint8_t> > trainingImg;
    std::vector<uint8_t> trainingLbl;
    std::vector< std::vector<uint8_t> > testImg;
    std::vector<uint8_t> testLbl;
};
