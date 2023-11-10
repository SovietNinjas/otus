#include <allocator.h>

#include <iostream>
#include <map>
#include <vector>

#include "proj_info.h"

//template <typename Key, typename Value, std::size_t Size>
//using MapAllocator = custom::Allocator<std::pair<const Key, Value>, Size>;

int main(int, char **) {
    project_info::printProjectInfo();

    // создание экземпляра std::map<int, int>
    std::vector<int, custom::Allocator<int, 1000>> v{};
//    std::map<int, int, std::less<int>, MapAllocator<int, int, 1000>> m{};
//    std::map<int, int, std::less<int>> m{};

    // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    // создание экземпляра std::map<int, int> с новым аллокатором, ограниченным 10
    // элементами
    // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    // вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере
    // создание экземпляра своего контейнера для хранения значений типа int
    // заполнение 10 элементами от 0 до 9
    // создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором,
    // ограниченным 10 элементами
    // заполнение 10 элементами от 0 до 9
    // вывод на экран всех значений, хранящихся в контейнере

//    m.emplace(1, 1);

    return 0;
}
