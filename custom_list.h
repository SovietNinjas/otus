#pragma once

#include <memory>

namespace custom {
template <typename T, typename Alloc = std::allocator<T>>
class List {
    struct ListNode {
        ListNode* next;
        T value;
    };

   public:
    using allocator_type = Alloc;

    List() : head_(nullptr), tail_(nullptr) {}

    List(const allocator_type& a) : head_(nullptr), tail_(nullptr), allocator_(a) {}

    void pushFront(const T& value);

    T& front() const { return head_->value; }

    T& back() { return tail_->value; }

    class Iterator {
       public:
        Iterator(const ListNode* ptr) noexcept : current_(ptr) {}

        Iterator& operator=(ListNode* ptr) {
            current_ = ptr;
            return *this;
        }

        Iterator& operator++() {
            if (current_) current_ = current_->next;
            return *this;
        }

        bool operator==(const Iterator& it) { return current_ == it.current_; }

        bool operator!=(const Iterator& it) { return current_ != it.current_; }

        T operator*() const { return current_->value; }

       private:
        const ListNode* current_ = nullptr;
    };

    Iterator begin() { return Iterator(head_); }

    Iterator begin() const { return Iterator(head_); }

    Iterator end() { return Iterator(nullptr); }

    Iterator end() const { return Iterator(nullptr); }

   private:
    ListNode* head_;
    ListNode* tail_;
    Alloc allocator_;
};

template <typename T, typename Alloc>
void List<T, Alloc>::pushFront(const T& value) {
    typename Alloc::template rebind<ListNode>::other alloc;
    ListNode* n = alloc.allocate(1);

    n->value = value;
    n->next = nullptr;

    if (head_ == nullptr) {
        head_ = n;
        tail_ = n;
    } else {
        tail_->next = n;
        tail_ = tail_->next;
    }
}

}  // namespace custom
