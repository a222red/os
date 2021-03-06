#ifndef CONTAINER_HPP
#define CONTAINER HPP

#include "mem.hpp"

/// Unique smart pointer type.
/// Allocation and deallocation are handled automatically,
/// helping prevent memory leaks.
template<typename T>
class Box {
    protected:
        T* ptr;
    public:
        inline Box(T t) : ptr(new T { t }) {}
        inline Box(const Box<T>& box) : ptr(new T { *box }) {}
        inline void operator =(const Box<T>& box) = delete;
        inline ~Box() { delete this->ptr; }
        inline T& operator *() { return *(this->ptr); }
        inline const T& operator *() const { return *(this->ptr); }
        inline T* operator ->() { return this->ptr; }
        inline const T* operator ->() const { return this->ptr; }
};

/// Lazy-loading container type.
/// This type is best used to store expensive-to-construct objects
/// that may or may not be needed.
/// The contained value is not heap-allocated,
/// so objects of this type should be passed by reference.
/// For a heap-allocated version, `Lazy<Box<T>>` is recommended over
/// `Box<Lazy<T>>`, because it lazy-loads the heap allocation.
template<typename T>
class Lazy {
    protected:
        mutable T t;
        T (*f)();
        mutable bool done;
    public:
        typedef T (*FnType)();
        inline Lazy(T (*fn)()) : f(fn), done(false) {}
        inline Lazy(const Lazy<T>& lazy) {
            this->f = lazy.get_fn();
            this->done = lazy.loaded();
            if (lazy.loaded()) this->t = *lazy;
        }
        inline void operator =(T (*fn)()) {
            this->done = false;
            this->f = fn;
        }
        inline void operator =(const Lazy<T>& lazy) {
            this->f = lazy.get_fn();
            this->done = lazy.loaded();
            if (lazy.loaded()) this->t = *lazy;
        }
        inline T& operator *() {
            if (!done) {
                this->done = true;
                this->t = this->fn();
            }

            return this->t;
        }
        inline const T& operator *() const {
            if (!done) {
                this->done = true;
                this->t = this->f();
            }

            return this->t;
        }
        inline FnType get_fn() const { return this->f; }
        inline bool loaded() const { return this->done; }
};

/// A single linked list node.
/// This type is part of the implementation of `List`,
/// and shouldn't be used on its own.
template<typename T>
struct __ListNode {
    T value;
    __ListNode<T>* next;
    __ListNode<T>* prev;
};

template<typename I = u32>
struct Range {
    I start;
    I end;
};

/// A doubly-linked list.
template<typename T, typename I = u32>
class List {
    protected:
        /// A pointer to the first element in the list.
        /// If this is null, the list is empty.
        __ListNode<T>* start;
        /// A pointer to the last element in the list.
        /// If this is null, the list is empty.
        __ListNode<T>* end;
        /// The amount of elements in the list.
        /// This value can be calculated at any time,
        /// but storing it is much more efficient.
        I length;
        __ListNode<T>* ith_front(I i) const {
            __ListNode<T>* ptr;

            for (ptr = this->start; i > 0; --i) ptr = ptr->next;

            return ptr;
        }
        __ListNode<T>* ith_back(I i) const {
            __ListNode<T>* ptr;

            for (ptr = this->end; i > 0; --i) ptr = ptr->prev;

            return ptr;
        }
        inline __ListNode<T>* ith_node(I i) const {
            if (i > this->length / 2)
                return ith_back(this->length - i - 1);
            else return ith_front(i);
        }
    public:
        /// Constructs an empty list.
        inline List() : start(nullptr), end(nullptr), length(0) {}
        /// Copies a list, reallocating each element.
        List(const List<T, I>& list) :
            start(nullptr),
            end(nullptr),
            length(0)
        {
            list.for_each(
                [](T t, void* s) { ((List*)s)->push_back(t); },
                this
            );
        }
        /// Constructs a list from an array.
        inline List(T* arr, I len) {
            for (I i = 0; i < len; ++i) this->push_back(arr[i]);
        }
        /// Copies another list, destroying this one.
        inline void operator =(const List& list) {
            this->~List();
            
            this->start = nullptr;
            this->end = nullptr;
            this->length = 0;

            list.for_each(
                [](T t, void* s) { ((List*)s)->push_back(t); },
                this
            );
        }
        ~List() {
            for (
                auto ptr = this->start;
                ptr->next != nullptr;
                ptr = ptr->next
            )
                delete ptr;
        }
        /// Iterates through each list element,
        /// calling `fn(elem, data)` for each element.
        /// This method is more efficient than iterating with
        /// a `for` loop.
        void for_each(
            void (*fn)(T, void*),
            void* data = nullptr
        ) const {
            __ListNode<T>* ptr;

            for (ptr = this->start; ptr != nullptr; ptr = ptr->next)
                fn(ptr->value, data);
        }
        /// Creates a new list of `fn(_, data)`
        /// applied to each element.
        List<T, I> map(T (*fn)(T, void*), void* data) const {
            List<T, I> list;
            __ListNode<T>* ptr;

            for (ptr = this->start; ptr != nullptr; ptr = ptr->next)
                list.push_back(fn(ptr->value, data));

            return list;
        }
        /// Creates a new list of `fn` applied to each element.
        List<T, I> map(T (*fn)(T, void*)) const {
            List<T, I> list;
            __ListNode<T>* ptr;

            for (ptr = this->start; ptr != nullptr; ptr = ptr->next)
                list.push_back(fn(ptr->value));

            return list;
        }
        T foldl(T (*fn)(T, T), T t) const {
            T res = t;
            __ListNode<T> ptr;

            for (ptr = this->start; ptr != nullptr; ptr = ptr->next)
                t = fn(t, ptr->value);

            return t;
        }
        /// Adds an element to the back of the list.
        void push_back(const T& t) {
            if (this->start == nullptr) {
                this->start = new __ListNode<T> {
                    .value = t,
                    .next = nullptr,
                    .prev = nullptr
                };
                this->end = this->start;
            }
            else {
                this->end->next = new __ListNode<T> {
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
                this->start = new __ListNode<T> {
                    .value = t,
                    .next = nullptr,
                    .prev = nullptr
                };
                this->end = this->start;
            }
            else {
                this->start->prev = new __ListNode<T> {
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
        /// Removes the `i`th element of the list
        void remove(I i) {
            auto node = this->ith_node(i);
            
            node->prev->next = node->next;
            node->next->prev = node->prev;
            
            delete node;
        }
        /// Gets the `i`th element of the list.
        const T& operator[](I i) const {
            return this->ith_node(i)->value;
        }
        /// Gets the `i`th element of the list.
        T& operator [](I i) { return this->ith_node(i)->value; }
        /// Construct a list from a slice of this one.
        List<T> operator [](Range<I> r) const {
            List list;
            __ListNode<T>* ptr = this->ith_node(r.start);
            I i = r.start;

            while (i < r.end) {
                list.push_back(ptr->value);
                ptr = ptr->next;
                ++i;
            }

            return list;
        }
        /// Gets the `i`th element of the list from the front.
        const T& index_front(I i) const {
            return this->ith_front(i)->value;
        }
        /// Gets the `i`th element of the list from the front.
        T& index_front(I i) { return this->ith_front(i)->value; }
        /// Gets the `i`th element of the list from the back.
        const T& index_back(I i) const {
            return this->ith_back(i)->value;
        }
        /// Gets the `i`th element of the list from the back.
        T& index_back(I i) { return this->ith_back(i)->value; }
        /// Gets the length of the list.
        /// This function doesn't actually
        /// calculate the length, so it's
        /// free to call.
        inline I len() const { return this->length; }
};

#endif
