#pragma once

#include <cstdint>
#include <cstring>  // std::memcpy
#include <memory>

namespace custom {

#if 0
template <typename T>
class Allocator {
   public:
    using value_type = T;

    Allocator() noexcept = default;
    ~Allocator() { operator delete[](pool_); }

    template <typename U, size_t SizeOther>
    Allocator(const Allocator<U>&) noexcept {}

    value_type* allocate(std::size_t size);

    inline void deallocate([[maybe_unused]]value_type* pValue, std::size_t size) {
        cap_ -= size;
    }

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

   private:
    inline value_type* allocate_(std::size_t size) { return static_cast<T*>(operator new(sizeof(T) * size)); }

    value_type* pool_ = nullptr;
    size_t allocSize_ = 0, cap_ = 0;
};

template <typename T>
bool operator!=([[maybe_unused]]Allocator<T> first, [[maybe_unused]]Allocator<T> second) {
    return false;
}

template <typename T>
T* Allocator<T>::allocate(std::size_t size) {
    if (cap_ + size >= allocSize_) {
        const auto newPoolSize = (cap_ + size) % 4 + cap_ + size;

        auto newPool = allocate_(newPoolSize);
        std::memcpy(newPool, pool_, allocSize_);
        operator delete[](pool_);
        pool_ = newPool;
        allocSize_ = newPoolSize;
    }

    auto res = pool_ + cap_;
    cap_ += size;

    return res;
}
#else

template <typename T, size_t Size>
class Allocator {
   public:
    using value_type = T;

    Allocator() noexcept = default;
    ~Allocator() { operator delete[](pool_); }

    template <typename U, size_t SizeOther>
    Allocator(const Allocator<U, SizeOther>&) noexcept {}

    value_type* allocate(std::size_t size);

    inline void deallocate([[maybe_unused]]value_type* pValue, std::size_t size) {
        cap_ -= size;
    }

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

   private:
    inline value_type* allocate_(std::size_t size) { return static_cast<T*>(operator new(sizeof(T) * size)); }

    value_type* pool_ = nullptr;
    size_t allocSize_ = 0, cap_ = 0;
};

template <typename T, size_t Size>
bool operator!=([[maybe_unused]]Allocator<T, Size> first, [[maybe_unused]]Allocator<T, Size> second) {
    return false;
}

template <typename T, size_t Size>
T* Allocator<T, Size>::allocate(std::size_t size) {
    if (cap_ + size >= allocSize_) {
        const auto newPoolSize = (cap_ + size) % Size + cap_ + size;

        auto newPool = allocate_(newPoolSize);
        std::memcpy(newPool, pool_, allocSize_);
        operator delete[](pool_);
        pool_ = newPool;
        allocSize_ = newPoolSize;
    }

    auto res = pool_ + cap_;
    cap_ += size;

    return res;
}

template <typename Value, size_t Size, typename Allocator = custom::Allocator<Value, Size>>
class Vector {};
#endif

}  // namespace custom
