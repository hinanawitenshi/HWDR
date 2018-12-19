#include <random>
#include <cmath>

#include "ClassifierBPNet.hpp"

ClassifierBPNet::ClassifierBPNet(int nUnit, int nEpoch, double learningRate,
    const Data *data) : nUnit(nUnit), nEpoch(nEpoch),
    learningRate(learningRate), data(data) {
    W1.resize(data->dim());
    for (int i = 0; i < W1.size(); i++) W1[i].resize(nUnit);
    W2.resize(nUnit);
    for (int i = 0; i < W2.size(); i++) W2[i].resize(10);
    B1.resize(nUnit);
    B2.resize(10);
    Z0.resize(data->dim());
    Z1.resize(nUnit);
    Z2.resize(10);
    A1.resize(nUnit);
    A2.resize(10);
}

std::string ClassifierBPNet::name() {
    return std::string("bp-net");
}

void ClassifierBPNet::train() {
    normalInit();
    for (int i = 0; i < nEpoch; i++) {
        // init input
        std::vector<uint8_t> input;
        uint8_t label;
        data->fetchNextTrain(input, label);
        setZ0(input);

        forwardFeed();
        backPropagation(label);

        // output something
        if (i % 10000 == 0) {
            double correct = test();
            printf("epoch=%d(%d) correct=%.5f\n", i, nEpoch, correct);
        }
    }
}

uint8_t ClassifierBPNet::maxA2() {
    double max = 0.0;
    int maxLabel = 0;
    for (int j = 0; j < A2.size(); j++) {
        if (A2[j] > max) {
            max = A2[j];
            maxLabel = j;
        }
    }
    return (uint8_t)maxLabel;
}

double ClassifierBPNet::test() {
    std::vector< std::vector<uint8_t> > testImgs;
    std::vector<uint8_t> testLabels;
    data->getTest(testImgs, testLabels);
    int nCorrect = 0;
    for (int i = 0; i < testImgs.size(); i++) {
        setZ0(testImgs[i]);
        forwardFeed();
        uint8_t maxLabel = maxA2();
        if (maxLabel == testLabels[i]) {
            nCorrect++;
        }
    }
    return nCorrect / (double)testImgs.size();
}

void ClassifierBPNet::softmax(std::vector<double> &src,
    std::vector<double> &dst) {
    double sum = 0.0;
    for (int i = 0; i < src.size(); i++) {
        sum += exp(src[i]);
    }
    for (int i = 0; i < dst.size(); i++) {
        dst[i] = exp(src[i]) / sum;
    }
}

double ClassifierBPNet::softmaxD(double prob) {
    return prob * (1 - prob);
}

void ClassifierBPNet::setZ0(const std::vector<uint8_t> &img) {
    for (int i = 0; i < img.size(); i++) {
        Z0[i] = (double)img[i] / 255.0;
    }
}

void ClassifierBPNet::backPropagation(uint8_t label) {
    // compute error
    std::vector<double> E2(10), E1(nUnit);
    for (int i = 0; i < E2.size(); i++) {
        E2[i] = costPD(label, i) * softmaxD(A2[i]);
    }
    // printf("Label: %d\n", label);
    // for (int row = 0; row < 28; row++) {
    //     for (int col = 0; col < 28; col++) {
    //         double pix = Z0[row * 28 + col];
    //         if (!pix) {
    //             printf("*");
    //         } else {
    //             printf(" ");
    //         }
    //     }
    //     printf("\n");
    // }
    // printf("Z2: ");
    // for (int i = 0; i < Z2.size(); i++) printf("%f ", Z2[i]); printf("\n");
    // printf("A2: ");
    // for (int i = 0; i < A2.size(); i++) printf("%f ", A2[i]); printf("\n");
    // printf("E2: ");
    // for (int i = 0; i < E2.size(); i++) printf("%f ", E2[i]); printf("\n");
    for (int i = 0; i < E1.size(); i++) {
        E1[i] = 0.0;
        for (int j = 0; j < W2[i].size(); j++) {
            E1[i] += W2[i][j] * E2[j];
        }
        E1[i] *= reluD(Z1[i]);
    }
    // printf("E1: ");
    // for (int i = 0; i < E1.size(); i++) printf("%f ", E1[i]); printf("\n");
    // printf("A1: ");
    // for (int i = 0; i < A1.size(); i++) printf("%f ", A1[i]); printf("\n");
    // for (int i = 0; i < Z2.size(); i++) {
    //     if (Z2[i] != Z2[i]) {
    //         abort();
    //     }
    // }

    // update weights
    for (int i = 0; i < W1.size(); i++) {
        for (int j = 0; j < W1[i].size(); j++) {
            W1[i][j] -= learningRate * Z0[i] * E1[j];
        }
    }
    for (int i = 0; i < B1.size(); i++) {
        B1[i] -= learningRate * E1[i];
    }
    for (int i = 0; i < W2.size(); i++) {
        for (int j = 0; j < W2[i].size(); j++) {
            W2[i][j] -= learningRate * A1[i] * E2[j];
        }
    }
    for (int i = 0; i < B2.size(); i++) {
        B2[i] -= learningRate * E2[i];
    }
}

void ClassifierBPNet::forwardFeed() {
    for (int i = 0; i < Z1.size(); i++) Z1[i] = 0.0;
    for (int i = 0; i < Z2.size(); i++) Z2[i] = 0.0;

    // compute the hidden layer
    for (int i = 0; i < W1.size(); i++) {
        for (int j = 0; j < W1[i].size(); j++) {
            Z1[j] += Z0[i] * W1[i][j];
        }
    }
    for (int i = 0; i < B1.size(); i++) {
        Z1[i] += B1[i];
        // ReLU activation
        A1[i] = relu(Z1[i]);
    }

    // compute the output
    for (int i = 0; i < W2.size(); i++) {
        for (int j = 0; j < W2[i].size(); j++) {
            Z2[j] += Z1[i] * W2[i][j];
        }
    }
    for (int i = 0; i < B2.size(); i++) {
        Z2[i] += B2[i];
    }
    // softmax function
    softmax(Z2, A2);
}

double ClassifierBPNet::cost(uint8_t label) {
    double C = 0.0;
    for (int i = 0; i < A2.size(); i++) {
        if (i == label) {
            C += pow(A2[i] - 1.0, 2);
        } else {
            C += pow(A2[i], 2);
        }
    }
    return 0.5 * C;
}

void ClassifierBPNet::pick() {
    std::vector<uint8_t> img;
    uint8_t label;
    data->pickTest(img, label);
    printf("Image:\n");
    data->printImg(img);
    printf("Label: %d\n", label);
    setZ0(img);
    forwardFeed();
    printf("Output: %d\n", maxA2());
}

double ClassifierBPNet::costPD(uint8_t label, int i) {
    if (label == i) {
        return A2[i] - 1.0;
    }
    return A2[i];
}

double ClassifierBPNet::relu(double x) {
    return fmax(0, x);
}

double ClassifierBPNet::reluD(double x) {
    return x > 0.0 ? 1.0 : 0.0;
}

void ClassifierBPNet::normalInit() {
    normalInit1D(B1);
    normalInit1D(B2);
    normalInit2D(W1);
    normalInit2D(W2);
}

void ClassifierBPNet::normalInit1D(std::vector<double> &v) {
    std::default_random_engine generator;
    std::normal_distribution<double> dist(0.0, 0.1);
    for (int i = 0; i < v.size(); i++) {
        v[i] = dist(generator);
    }
}

void ClassifierBPNet::normalInit2D(std::vector< std::vector<double> > &v) {
    std::default_random_engine generator;
    std::normal_distribution<double> dist(0.0, 0.1);
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
            v[i][j] = dist(generator);
        }
    }
}
