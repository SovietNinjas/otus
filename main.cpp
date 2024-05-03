#include <allocator.h>

#include <iostream>
#include <map>
#include <vector>

#include "custom_list.h"
#include "utils.h"

template <typename Key, typename Value>
using MapAllocator = custom::Allocator<std::pair<const Key, Value>>;

int main(int, char **) {
    {
        std::map<int, int> m;
        utils::fill(m);

        utils::print(m, "map with common alloc");
    }

    {
        std::map<int, int, std::less<int>, MapAllocator<int, int>> m;
        utils::fill(m);

        utils::print(m, "map with custom alloc");
    }

    {
        custom::List<int> l;
        utils::fill(l);

        utils::print(l, "custom list with common alloc");
    }

    {
        custom::List<int, custom::Allocator<int>> l;
        utils::fill(l);

        utils::print(l, "custom list with custom alloc");
    }

    return 0;
}
