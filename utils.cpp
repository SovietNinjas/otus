#include "utils.h"

#include <boost/algorithm/hex.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <filesystem>

std::vector<std::string> utils::dirFiles(const std::string& dirPath, const std::vector<std::string>& excludeDirList,
                                         const std::vector<std::string>& maskList, bool isRecursive) {
    std::vector<std::string> v;
    auto read_directory = [&v, &excludeDirList, &maskList](auto iterator_type) {
        for (const auto& f : boost::make_iterator_range(iterator_type, {})) {
            auto f_path = f.path();
            if (!is_directory(f) && !vectorContains(excludeDirList, f_path.parent_path().string())) {
                std::string file_name = f_path.stem().string();
                stringToLower(file_name);

                if (maskList.empty() || vectorContains(maskList, file_name)) {
                    v.emplace_back(f_path.string());
                }
            }
        }
    };

    isRecursive ? read_directory(std::filesystem::recursive_directory_iterator(dirPath))
                : read_directory(std::filesystem::directory_iterator(dirPath));
    return v;
}

std::string utils::md5(const std::string& s) {
    boost::uuids::detail::md5 hash;
    boost::uuids::detail::md5::digest_type digest;

    hash.process_bytes(s.c_str(), s.size());
    hash.get_digest(digest);

    const auto charDigest = reinterpret_cast<const char*>(&digest);
    std::string result;
    boost::algorithm::hex(charDigest, charDigest + sizeof(boost::uuids::detail::md5::digest_type),
                          std::back_inserter(result));
    return result;
}

utils::TimePoint utils::getTick() { return std::chrono::steady_clock::now(); }

uint64_t utils::getTickDiff(const utils::TimePoint& p) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(utils::getTick() - p).count();
}

void utils::stringToLower(std::string& s) {
    for (size_t i = 0, c = s.size(); i < c; ++i) {
        s[i] = std::move((char)std::tolower((int)s[i]));
    }
}
