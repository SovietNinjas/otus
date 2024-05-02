#pragma once

#ifdef WIN32
#include <Eigen/Dense>
#else
#include <eigen3/Eigen/Dense>
#endif

#include "classifier.h"

namespace mnist {
class MlpClassifier : public Classifier {
   public:
    MlpClassifier(const Eigen::MatrixXf &, const Eigen::MatrixXf &);

    size_t numClasses() const override;

    size_t predict(const features_t &) const override;

    probas_t predictProba(const features_t &) const override;

   private:
    Eigen::MatrixXf w1_, w2_;
};
}  // namespace mnist
