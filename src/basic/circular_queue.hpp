#pragma once

#include "../utils.hpp"
#include <cstdint>
#include <cassert>
#include <variant>

/* Declaration */
namespace ds_imp {
    
template <typename T> 
class CircularQueue {

    using Result = std::variant<std::nullptr_t, T>;

    public:
        CircularQueue();
        ~CircularQueue();

        Result front();
        Result back();
        void enqueue(const T  &ele);
        void enqueue(T &&ele);
        void dequeue();
        inline bool empty() const;
        inline size_t capacity() const;
        inline size_t size() const;
    
    private:
        T* arr;
        size_t head;
        size_t tail;
        size_t arr_size;
        size_t num_nodes;

        inline size_t next(size_t index);
};

}

/* Implementation */
namespace ds_imp { 

/* CircularQueue */
template <typename T>
CircularQueue<T>::CircularQueue() {

    this->head = 0;
    this->tail = 0;
    this->num_nodes = 0;
    this->arr_size = 4;
    this->arr = new T[arr_size]();
}

template <typename T>
CircularQueue<T>::~CircularQueue() {
    
    if(arr != nullptr)
        delete arr;
}

template <typename T>
CircularQueue<T>::Result CircularQueue<T>::front() {
    
    if(empty())
        return nullptr;
    return arr[next(head)];
}

template <typename T>
CircularQueue<T>::Result CircularQueue<T>::back() {
    
    if(empty())
        return nullptr;
    return arr[tail];
}

template <typename T>
void CircularQueue<T>::enqueue(const T &ele) {
    
    if(size() == arr_size - 1) {
        arr = double_arr(arr, arr_size);
        if(tail < head) {
            size_t i = 0, j = arr_size;
            for(; i <= tail; ++i, ++j) {
                arr[j] = std::move(arr[i]);
            }
            tail = arr_size;
        }
        arr_size *= 2;
    }

    tail = next(tail);
    arr[tail] = ele;
    num_nodes ++;
}

template <typename T>
void CircularQueue<T>::enqueue(T &&ele) {
    
    if(size() == arr_size - 1) {
        arr = double_arr(arr, arr_size);
        if(tail < head) {
            size_t i = 0, j = arr_size;
            for(; i <= tail; ++i, ++j) {
                arr[j] = std::move(arr[i]);
            }
            tail = arr_size;
        }
        arr_size *= 2;
    }

    tail = next(tail);
    arr[tail] = std::move(ele);
    num_nodes ++;
}

template <typename T>
void CircularQueue<T>::dequeue() {
    
    if(empty())
        return;
    
    assert(head != tail);
    head = next(head);
    num_nodes --;
}

template <typename T>
inline bool CircularQueue<T>::empty() const {
    return (size() == 0);
}

template <typename T>
inline size_t CircularQueue<T>::capacity() const {
    return arr_size - 1;
}

template <typename T>
inline size_t CircularQueue<T>::size() const {
    return num_nodes;
}

template <typename T>
inline size_t CircularQueue<T>::next(size_t index) {
    return (index + 1) % arr_size;
}

}
