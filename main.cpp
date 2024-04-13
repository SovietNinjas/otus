#include <assert.h>

#include <iostream>
#include <numeric>
#include <unordered_map>

template <typename ValueT, ValueT DefaultValue>
class Matrix {
   private:
    using Key = std::pair<std::size_t, std::size_t>;
    struct Hash {
        std::size_t operator()(Key key) const noexcept {
            const auto val = h(key.second);
            return (h(key.first) ^ val) + val;
        }

       private:
        std::hash<std::size_t> h{};
    };

   public:
    using BufferT = std::unordered_map<Key, ValueT, Hash>;

    class Wrapper {
       public:
        Wrapper(std::size_t pos, BufferT& buff) : pos_({pos, 0}), buff_(buff) {}

        Wrapper operator[](std::size_t inx) {
            pos_.second = inx;

            if (buff_.contains(pos_)) {
                value_ = buff_[pos_];
            }

            return *this;
        }

        Wrapper operator=(const ValueT& val) {
            buff_[pos_] = val;
            return *this;
        }

        bool operator==(const ValueT& value) const noexcept { return value == value_; }

       private:
        ValueT value_ = DefaultValue;
        Key pos_{};
        BufferT& buff_;
    };

    class Iterator {
       public:
        using IteratorT = BufferT::const_iterator;
        explicit Iterator(IteratorT it) : it_(it) {}

        IteratorT& operator++() { return ++it_; }
        auto operator<=>(const Iterator&) const noexcept = default;
        std::tuple<int, int, ValueT> operator*() const {
            return std::make_tuple(it_->first.first, it_->first.second, it_->second);
        }

       private:
        IteratorT it_{};
    };

    Iterator begin() const noexcept { return Iterator(data_.cbegin()); }
    Iterator end() const noexcept { return Iterator(data_.cend()); }

    Wrapper operator[](std::size_t inx) { return Wrapper(inx, data_); }
    std::size_t size() const noexcept { return data_.size(); }

   private:
    BufferT data_{};
};

int main(int, char**) {
    // бесконечная матрица int заполнена значениями -1
    Matrix<int, -1> matrix;
    assert(matrix.size() == 0);  // все ячейки свободны
    auto a = matrix[0][0];
    assert(a == -1);
    assert(matrix.size() == 0);
    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);

    ((matrix[100][100] = 314) = 0) = 217;

    // выведется одна строка
    // 100100217
    for (auto c : matrix) {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }

    std::cout << "ok" << std::endl;

    return 0;
}
