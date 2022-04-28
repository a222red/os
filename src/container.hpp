#ifndef CONTAINER_HPP
#define CONTAINER HPP

#include "mem.hpp"

template<typename T>
struct ListNode {
    T value;
    ListNode<T>* next;
    ListNode<T>* prev;
};

/// A doubly-linked list.
template<typename T, typename I = u32>
class List {
    protected:
        /// A pointer to the first element in the list.
        /// If this is null, the list is empty.
        ListNode<T>* start;
        /// A pointer to the last element in the list.
        /// If this is null, the list is empty.
        ListNode<T>* end;
        /// The amount of elements in the list.
        /// This value can be calculated at any time,
        /// but storing it is much more efficient.
        I length;
    public:
        /// Constructs an empty list.
        List() : start(nullptr), end(nullptr), length(0) {}
        /// Copies a list, reallocating each element.
        List(const List<T, I>& list) { *this = list.clone(); }
        /// Constructs a list from an array.
        List(T* arr, I len) {
            for (I i = 0; i < len; ++i) this->push_back(arr[i]);
        }
        /// Creates a copy of this list.
        /// This function usually shouldn't be called,
        /// but instead invoked through the
        /// copy constructor.
        List<T, I> clone() const {
            List<T, I> list;
            ListNode<T>* ptr;

            for (ptr = this->start; ptr != nullptr; ptr = ptr->next)
                list.push_back(ptr->value);
            
            return list;
        }
        /// Adds an element to the back of the list.
        void push_back(const T& t) {
            if (this->start == nullptr) {
                this->start = new ListNode<T> {
                    .value = t,
                    .next = nullptr,
                    .prev = nullptr
                };
                this->end = this->start;
            }
            else {
                this->end->next = new ListNode<T> {
                    .value = t,
                    .next = nullptr,
                    .prev = this->end
                };
                this->end = this->end->next;
            }
            ++this->length;
        }
        /// Adds an element to the front of the list.
        void push_front(const T& t) {
            if (this->start == nullptr) {
                this->start = new ListNode<T> {
                    .value = t,
                    .next = nullptr,
                    .prev = nullptr
                };
                this->end = this->start;
            }
            else {
                this->start->prev = new ListNode<T> {
                    .value = t,
                    .next = this->start,
                    .prev = nullptr
                };
                this->start = this->start->prev;
            }
            ++this->length;
        }
        /// Removes an element from the back of the list.
        void pop_back() {
            if (this->end->prev == nullptr) {
                delete this->end;
                this->end = nullptr;
                this->start = nullptr;
            }
            else {
                this->end->prev->next = nullptr;
                auto ptr = this->end->prev;
                delete this->end;
                this->end = ptr;
            }
            --this->length;
        }
        /// Removes an element from the front of the list.
        void pop_front() {
            if (this->end->prev == nullptr) {
                delete this->end;
                this->end = nullptr;
                this->start = nullptr;
            }
            else {
                this->start->next->prev = nullptr;
                auto ptr = this->start->next;
                delete this->start;
                this->start = ptr;
            }
            --this->length;
        }
        /// Gets the `i`th element of the list.
        T& operator[](I i) const {
            ListNode<T>* ptr;

            for (ptr = this->start; i > 0; --i) ptr = ptr->next;

            return ptr->value;
        }
        /// Gets the `i`th element of the list from the back.
        T& index_back(I i) const {
            ListNode<T>* ptr;

            for (ptr = this->end; i > 0; --i) ptr = ptr->prev;

            return ptr->value;
        }
        /// Gets the length of the list.
        /// This function doesn't actually
        /// calculate the length, so it's
        /// free to call.
        inline I len() const { return this->length; }
};

struct Range {
    u32 start;
    u32 end;
};

#endif
