#include <assert.h>

#include <iostream>
#include <numeric>
#include <unordered_map>

template <typename ValueT, ValueT DefaultValue>
class Matrix {
   public:
    struct SecondStage;
    using MapT = std::unordered_map<std::size_t, SecondStage>;

    struct ThirdStage {
        ThirdStage operator=(ValueT val) {
            currentVal_ = val;
            return pos.ref->setForce(pos.inx, *this);
        }

        auto operator<=>(const ValueT& other) const { return currentVal_ <=> other; }
        bool operator==(const ValueT& other) const { return currentVal_ == other; }

        ValueT currentVal_ = DefaultValue;
        struct {
            SecondStage* ref;
            std::size_t inx = 0;
        } pos;
    };

    struct SecondStage {
        using DataT = std::unordered_map<std::size_t, ThirdStage>;
        ThirdStage operator[](std::size_t inx) {
            if (d.contains(inx)) {
                return d[inx];
            }
            return ThirdStage{.pos = {this, inx}};
        }

        ThirdStage setForce(std::size_t inx, const ThirdStage& val) { return d[inx] = val; }

        inline std::size_t size() const { return d.size(); }

        DataT d;
    };

    SecondStage& operator[](std::size_t inx) { return data_[inx]; }

    // const SecondStage& operator[](std::size_t inx) const { return data_[inx]; }

    inline std::size_t size() const {
        return std::accumulate(std::cbegin(data_), std::cend(data_), static_cast<size_t>(0),
                               [](auto acc, const auto& secondStage) { return acc + secondStage.second.size(); });
    }

    class View {
       public:
        struct Position {
            MapT::const_iterator first{};
            SecondStage::DataT::const_iterator second{};
        };

        View(Position pos) : pos_(pos) {}

        std::tuple<int, int, ValueT> operator*() const {
            return std::make_tuple(pos_.first->first, pos_.second->first, pos_.second->second.currentVal_);
        }

        bool operator!=(const View& other) const {
            return std::tie(pos_.first, pos_.second) != std::tie(other.pos_.first, other.pos_.second);
        }

        View& operator++() {
            if (++pos_.second == pos_.first->second.d.cend()) {
                ++pos_.first;
                pos_.second = pos_.first->second.d.cbegin();
            }
            return *this;
        }

       private:
        Position pos_{};
    };

    View begin() { return View({data_.cbegin(), data_[0].d.cbegin()}); }
    View end() {
        auto it = std::prev(cend(data_));
        return View({it, it->second.d.cend()});
    }

   private:
    MapT data_{};
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
    // выведется одна строка
    // 100100314
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

// Спроектировать 2 - мерную разреженную бесконечную матрицу, заполненную значениями по умолчанию.

// Матрица должна хранить только занятые элементы - значения которых хотя бы раз присваивались.
// Присвоение в ячейку значения по умолчанию освобождает ячейку.

// Необходимо уметь отвечать на вопрос - сколько ячеек реально занято? Необходимо уметь проходить по всем занятым
// ячейкам. Порядок не имеет значения.Возвращается позиция ячейки и ее значение.При чтении элемента из свободной ячейки
// возвращать значение по умолчанию.При запуске программы необходимо создать матрицу с пустым значением 0, заполнить
// главную диагональ матрицы(от[0, 0] до[9, 9]) значениями от 0 до 9. Второстепенную диагональ(от[0, 9] до[9, 0])
// значениями от 9 до 0. Необходимо вывести фрагмент матрицы от[1, 1] до[8, 8]. Между столбцами пробел.Каждая строка
// матрицы на новой строке консоли.Вывести количество занятых ячеек.Вывести все занятые ячейки вместе со своими
// позициями.Опционально реализовать N
// - мерную матрицу.Опционально реализовать каноническую форму оператора `=`,
// допускающую выражения `((matrix[100][100] = 314) = 0) = 217`
// Самоконтроль - индексация оператором `[]` -
// количество занятых ячеек должно быть
// 18 после выполнения примера выше Проверка Задание считается выполненным успешно,
// если после анализа кода, установки пакета и запуска приложения появился фрагмент матрицы,
// количество ячеек и список всех значений
