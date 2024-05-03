#pragma once

#include <cstdint>
#include <cstring>  // std::memcpy
#include <memory>

namespace custom {

template <typename T>
struct pointer_traits {
    using reference = T&;
    using const_reference = const T&;
};
//-----------------------------------------------------------------------------
template <>
struct pointer_traits<void> {};

template <typename T, size_t Size = 10>
class Allocator : public pointer_traits<T> {
   public:
    using value_type = T;

    Allocator() noexcept = default;
    ~Allocator() noexcept {}

    template <typename U, size_t SizeOther>
    Allocator(const Allocator<U, SizeOther>& other) noexcept {}

    value_type* allocate(std::size_t size, const void* = nullptr);

    inline void deallocate([[maybe_unused]] value_type* pValue, std::size_t size) {
        pos_ -= size;
    }

    template <typename U>
    struct rebind {
        using other = Allocator<U>;
    };

   private:
    inline value_type* allocate_(std::size_t size) { return reinterpret_cast<T*>(operator new(sizeof(T) * size)); }

    value_type* pool_ = nullptr;
    size_t allocSize_ = 0, pos_ = 0;
};

template <typename T, size_t Size>
T* Allocator<T, Size>::allocate(std::size_t size, const void*) {
    if (pool_ == nullptr) {
        allocSize_ = Size;
        pool_ = allocate_(allocSize_);

    } else if (const auto s = pos_ + size; s > allocSize_) {
        const auto newPoolSize = Size - (s % Size) + s;
        auto newPool = allocate_(newPoolSize);
        std::memcpy((void*)newPool, (void*)pool_, allocSize_ * sizeof(T));
        operator delete[](pool_);
        pool_ = newPool;
        allocSize_ = newPoolSize;
    }

    const auto res = pool_ + pos_;
    pos_ += size;

    return res;
}

template <typename T1, std::size_t N, typename T2>
bool operator==(const Allocator<T1, N>& lhs, const Allocator<T2, N>& rhs) noexcept {
    return lhs.pool_ == rhs.pool_;
}

template <typename T1, std::size_t N, typename T2>
bool operator!=(const Allocator<T1, N>& lhs, const Allocator<T2, N>& rhs) noexcept {
    return !(lhs == rhs);
}

}  // namespace custom
