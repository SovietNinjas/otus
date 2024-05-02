#include "accuracy_finder.h"

AccuracyFinder::AccuracyFinder(std::size_t input, std::size_t hidden, std::size_t output,
                               const std::string& classfilierPath)
    : input_(input), hidden_(hidden), output_(output), classifier_(nullptr) {
#ifdef WIN32
    char pathSep = '\\';
#else
    char pathSep = '/';
#endif

    auto l1 = mnist::readMatFromFile(input_, hidden_, classfilierPath + pathSep + "w1.txt");
    auto l2 = mnist::readMatFromFile(hidden_, output_, classfilierPath + pathSep + "w2.txt");

    classifier_ = new mnist::MlpClassifier(l1.transpose(), l2.transpose());
}

AccuracyFinder::~AccuracyFinder() {}

float AccuracyFinder::getAccuracy(const std::string& csvPath) {
    mnist::MlpClassifier::features_t features;

    auto file = mnist::readCsv(csvPath);
    float all = (float)file.size();
    float right = 0;

    for (auto& fileString : file) {
        std::size_t expected = std::stoi(fileString.front());
        fileString.erase(fileString.begin());

        mnist::readFeatures(fileString, features);
        auto pred = classifier_->predict(features);

        if (expected == pred) {
            right++;
        }
    }

    float accuracy = right / all;
    return accuracy;
}
