#pragma once

#include <cstddef>
#include <vector>

namespace mnist {

class Classifier {
   public:
    using features_t = std::vector<float>;
    using probas_t = std::vector<float>;

    virtual ~Classifier() {}

    virtual size_t numClasses() const = 0;

    virtual size_t predict(const features_t &) const = 0;

    virtual probas_t predictProba(const features_t &) const = 0;
};

}  // namespace mnist
