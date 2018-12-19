#include <cstdio>
#include <sys/stat.h>
#include <string>

#include "DataMnist.hpp"

DataMnist::DataMnist() {
    printf("loading the dataset...\n");

    // open files
    FILE *trainingImgF = fopen("./.mnist/train-images-idx3-ubyte", "r");
    FILE *trainingLblF = fopen("./.mnist/train-labels-idx1-ubyte", "r");
    FILE *testImgF = fopen("./.mnist/t10k-images-idx3-ubyte", "r");
    FILE *testLblF = fopen("./.mnist/t10k-labels-idx1-ubyte", "r");
    if (!(trainingImgF && trainingLblF && testImgF && testLblF)) {
        printf("loading failed, file not exist\n");
        return;
    }

    // load data
    loadIdx3(trainingImg, trainingImgF);
    loadIdx1(trainingLbl, trainingLblF);
    loadIdx3(testImg, testImgF);
    loadIdx1(testLbl, testLblF);

    // close files
    fclose(trainingImgF);
    fclose(trainingLblF);
    fclose(testImgF);
    fclose(testLblF);

    printf("loading complete\n");
}

void DataMnist::fetchNextTrain(std::vector<uint8_t> &img,
    uint8_t &label) const {
    static int pos = 0;
    std::vector<uint8_t> ret(trainingImg[pos]);
    img = ret;
    label = trainingLbl[pos];
    pos++;
    if (pos == trainingImg.size())
        pos = 0;
}

std::string DataMnist::name() const {
    return std::string("MNIST");
}

int DataMnist::dim() const {
    return 784;
}

void DataMnist::getTest(std::vector< std::vector<uint8_t> > &imgs,
    std::vector<uint8_t> &labels) const {
    imgs = testImg;
    labels = testLbl;
}

void DataMnist::show() const {
    int pos = rand() % trainingImg.size();
    printf("The %d image in the training set:\n", pos);
    printImg(trainingImg[pos]);
    printf("Label: %d\n", trainingLbl[pos]);
    pos = rand() % testImg.size();
    printf("The %d image in the test set:\n", pos);
    printImg(testImg[pos]);
    printf("Label: %d\n", testLbl[pos]);
}

void DataMnist::pickTest(std::vector<uint8_t> &img, uint8_t &label) const {
    int pos = rand() % testImg.size();
    img = testImg[pos];
    label = testLbl[pos];
}

void DataMnist::printImg(const std::vector<uint8_t> &img) const {
    for (int row = 0; row < 28; row++) {
        for (int col = 0; col < 28; col++) {
            uint8_t pix = img[row * 28 + col];
            if (!pix) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void DataMnist::loadIdx3(std::vector< std::vector<uint8_t> > &v, FILE *f) {
    int magicNum = 0;
    fread(&magicNum, sizeof(int), 1, f);
    bool littleEndian = magicNum != 2051;
    magicNum = convertEndian(magicNum);
    int num = 0, nrow = 0, ncol = 0;
    fread(&num, sizeof(int), 1, f);
    fread(&nrow, sizeof(int), 1, f);
    fread(&ncol, sizeof(int), 1, f);
    if (littleEndian) {
        num = convertEndian(num);
        nrow = convertEndian(nrow);
        ncol = convertEndian(ncol);
    }
    v.resize(num);
    for (int i = 0; i < num; i++) {
        std::vector<uint8_t> img(nrow * ncol);
        for (int j = 0; j < nrow * ncol; j++) {
            uint8_t pixel = 0;
            fread(&pixel, sizeof(uint8_t), 1, f);
            img[j] = pixel;
        }
        v[i] = img;
    }
}

void DataMnist::loadIdx1(std::vector<uint8_t> &v, FILE *f) {
    int magicNum = 0;
    fread(&magicNum, sizeof(int), 1, f);
    bool littleEndian = magicNum != 2049;
    int num = 0;
    fread(&num, sizeof(int), 1, f);
    if (littleEndian) num = convertEndian(num);
    v.resize(num);
    for (int i = 0; i < num; i++) {
        uint8_t label = 0;
        fread(&label, sizeof(uint8_t), 1, f);
        v[i] = label;
    }
}

int DataMnist::convertEndian(int num) {
    unsigned int buf[4];
    buf[0] = (num & 0xFF000000) >> 24;
    buf[1] = (num & 0x00FF0000) >> 8;
    buf[2] = (num & 0x0000FF00) << 8;
    buf[3] = (num & 0x000000FF) << 24;
    return buf[0] | buf[1] | buf[2] | buf[3];
}
