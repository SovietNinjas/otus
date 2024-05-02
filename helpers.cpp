#include "helpers.h"

namespace mnist {
Eigen::MatrixXf readMatFromFile(std::size_t rows, std::size_t cols, const std::filesystem::path &matPath) {
    std::ifstream matFile(matPath.native());
    Eigen::MatrixXf matRes(rows, cols);

    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            std::string tmp;
            matFile >> tmp;
            double val = std::stod(tmp);
            matRes(i, j) = (float)val;
        }
    }

    return matRes;
}

void readFeatures(std::vector<std::string> &pixels, Classifier::features_t &features) {
    features.clear();
    for (const auto &pixel : pixels) {
        features.push_back(std::stof(pixel));
    }
}

std::vector<std::vector<std::string>> readCsv(const std::filesystem::path &csvPath) {
    std::vector<std::vector<std::string>> result;
    std::fstream csv(csvPath);

    for (std::string line; std::getline(csv, line);) {
        result.push_back(utilities::split(line, ','));
    }

    return result;
}

}  // namespace mnist

namespace utilities {
std::vector<std::string> split(const std::string &str, char d) {
    std::vector<std::string> v;
    if (!str.empty()) {
        size_t pos = 0, posLast = 0;
        while ((pos = str.find(d, posLast)) != std::string::npos) {
            if (pos != 0) {
                if ((pos - posLast) > 0) {
                    v.emplace_back(str.substr(posLast, pos - posLast));
                }
                posLast = ++pos;
            } else {
                ++posLast;
            }
        }

        if (pos == std::string::npos) {
            size_t sz = str.size();
            if (posLast < sz) {
                v.emplace_back(str.substr(posLast, sz - posLast));
            }
        }
    }
    return v;
}
}  // namespace utilities
