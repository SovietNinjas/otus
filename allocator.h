#pragma once

#include <cstdint>
#include <cstring>  // std::memcpy
#include <memory>

namespace custom {

template <typename T, size_t Size>
class Allocator {
   public:
    using value_type = T;

    Allocator() noexcept = default;
    ~Allocator() { operator delete(pool_); }

    template <typename U, size_t SizeOther>
    Allocator(const Allocator<U, SizeOther>&) noexcept {}

    value_type* allocate(std::size_t size);

    inline void deallocate(value_type* pValue, [[maybe_unused]] std::size_t) { allocSize_ = cap_ = 0; }

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

   private:
    inline value_type* allocate_(std::size_t size) { return static_cast<T*>(operator new(sizeof(T) * size)); }

    value_type* pool_ = nullptr;
    size_t allocSize_ = 0, cap_ = 0;
};

template <typename T, size_t Size>
T* Allocator<T, Size>::allocate(std::size_t size) {
    T* res = nullptr;
    if (cap_ + size >= allocSize_) {
        const auto newPoolSize = (cap_ + size) % Size;

        auto newPool = allocate_(allocSize_);
        std::memcpy(newPool, pool_, allocSize_);
        deallocate();
    }

    res = pool_ + cap_;
    cap_ += size;

    return res;
}

template <typename Value, typename Allocator = custom::Allocator<Value>>
class Vector {};

}  // namespace custom
