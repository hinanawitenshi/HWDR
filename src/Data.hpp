#pragma once

#include <string>
#include <vector>

class Data {
public:
    virtual std::string name() const = 0;
    virtual void show() const = 0;
    virtual int dim() const = 0;
    virtual void printImg(const std::vector<uint8_t> &img) const = 0;
    virtual void fetchNextTrain(std::vector<uint8_t> &img,
        uint8_t &label) const = 0;
    virtual void getTest(std::vector< std::vector<uint8_t> > &imgs,
        std::vector<uint8_t> &labels) const = 0;
    virtual void pickTest(std::vector<uint8_t> &img, uint8_t &label) const = 0;
};
