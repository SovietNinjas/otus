#pragma once

#include "helpers.h"
#include "mlp_classifier.h"

class AccuracyFinder
{
public:
    AccuracyFinder(std::size_t input, std::size_t hidden, std::size_t output, const std::string &classfilierPath);
    ~AccuracyFinder();

    float getAccuracy(const std::string& csvPath);

private:
    std::size_t input_;
    std::size_t hidden_;
    std::size_t output_;
    mnist::MlpClassifier* classifier_;
};
