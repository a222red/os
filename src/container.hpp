#ifndef CONTAINER_HPP
#define CONTAINER HPP

#include "mem.hpp"

template<typename T>
struct ListNode {
    T value;
    ListNode<T>* next;
    ListNode<T>* prev;
};

template<typename T, typename I = u32>
class List {
    protected:
        ListNode<T>* start;
        ListNode<T>* end;
        I length;
    public:
        List() : start(nullptr), end(nullptr), length(0) {}
        List(T* arr, I len) {
            for (I i = 0; i < len; ++i) this->push_back(arr[i]);
        }
        void push_back(const T& t) {
            if (start == nullptr) {
                start = new ListNode<T> {
                    .value = t,
                    .next = nullptr,
                    .prev = nullptr
                };
                end = start;
            }
            else {
                end->next = new ListNode<T> {
                    .value = t,
                    .next = nullptr,
                    .prev = end
                };
                end = end->next;
            }
            ++this->length;
        }
        void push_front(const T& t) {
            if (start == nullptr) {
                start = new ListNode<T> {
                    .value = t,
                    .next = nullptr,
                    .prev = nullptr
                };
                end = start;
            }
            else {
                start->prev = new ListNode<T> {
                    .value = t,
                    .next = start,
                    .prev = nullptr
                };
                start = start->prev;
            }
            ++this->length;
        }
        void pop_back() {
            if (end->prev == nullptr) {
                delete end;
                end = nullptr;
                start = nullptr;
            }
            else {
                end->prev->next = nullptr;
                auto ptr = end->prev;
                delete end;
                end = ptr;
            }
            --this->length;
        }
        void pop_front() {
            if (end->prev == nullptr) {
                delete end;
                end = nullptr;
                start = nullptr;
            }
            else {
                start->next->prev = nullptr;
                auto ptr = start->next;
                delete start;
                start = ptr;
            }
            --this->length;
        }
        T& operator[](I i) const {
            ListNode<T>* ptr;

            for (ptr = start; i > 0; --i) ptr = ptr->next;

            return ptr->value;
        }
        T& index_back(I i) const {
            ListNode<T>* ptr;

            for (ptr = end; i > 0; --i) ptr = ptr->prev;

            return ptr->value;
        }
        I len() const { return this->length; }
};

struct Range {
    u32 start;
    u32 end;
};

#endif
