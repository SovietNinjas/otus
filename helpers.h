#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#ifdef WIN32
#include <Eigen/Dense>
#else
#include <eigen3/Eigen/Dense>
#endif

#include "classifier.h"

namespace mnist {

Eigen::MatrixXf readMatFromFile(size_t rows, size_t cols, const std::filesystem::path &);

void readFeatures(std::vector<std::string> &, Classifier::features_t &);

std::vector<std::vector<std::string>> readCsv(const std::filesystem::path &);

}  // namespace mnist

namespace utilities {

std::vector<std::string> split(const std::string &, char);

}
