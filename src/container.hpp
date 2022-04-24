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
        List(const List<T, I>& list) { *this = list.clone(); }
        List(T* arr, I len) {
            for (I i = 0; i < len; ++i) this->push_back(arr[i]);
        }
        List<T, I> clone() const {
            List<T, I> list;
            ListNode<T>* ptr;

            for (ptr = this->start; ptr != nullptr; ptr = ptr->next)
                list.push_back(ptr->value);
            
            return list;
        }
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
        T& operator[](I i) const {
            ListNode<T>* ptr;

            for (ptr = this->start; i > 0; --i) ptr = ptr->next;

            return ptr->value;
        }
        T& index_back(I i) const {
            ListNode<T>* ptr;

            for (ptr = this->end; i > 0; --i) ptr = ptr->prev;

            return ptr->value;
        }
        inline I len() const { return this->length; }
};

struct Range {
    u32 start;
    u32 end;
};

#endif
