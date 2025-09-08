#pragma once

#include "../utils.hpp"
#include <cstdint>
#include <cassert>
#include <functional>
#include <variant>

/* Declaration */
namespace ds_imp {

template <typename T, typename CMP = std::less<T>> 
class Heap {

    using Result = std::variant<std::nullptr_t, T>;

    public:
        Heap(CMP cmp = CMP());
        ~Heap();

        Result top();
        void push(const T  &ele);
        void push(T &&ele);
        void pop();
        inline bool empty()      const;
        inline size_t capacity() const;
        inline size_t size()     const;
    
    private:
        T** arr;
        size_t arr_size;  // capacity()
        size_t num_nodes; // size();
        CMP cmp;
};

}

/* Implementation */
namespace ds_imp {

template <typename T, typename CMP> 
Heap<T,CMP>::Heap(CMP cmp) {
    this->cmp       = cmp;
    this->num_nodes = 0;
    this->arr_size  = 4;
    this->arr = new T*[arr_size]();
}

template <typename T, typename CMP> 
Heap<T,CMP>::~Heap() {

    for(size_t i = 0; i < arr_size; ++i) {
        if(arr[i] != nullptr)
            delete arr[i];
    }
    delete [] arr;
}

template <typename T, typename CMP> 
Heap<T,CMP>::Result Heap<T,CMP>::top() {

    if(empty()) 
        return nullptr;
    return *(arr[1]);
}

template <typename T, typename CMP> 
void Heap<T,CMP>::push(const T  &ele) {

    if(num_nodes + 1 >= arr_size) {
        arr = double_arr<T*>(arr, arr_size);
        arr_size *= 2;
    }

    size_t curr_index = num_nodes + 1;
    T* curr_node = new T(ele);
    T* parent_node = arr[curr_index / 2];

    while(curr_index > 1 && cmp(*parent_node, *curr_node)) {
        arr[curr_index] = parent_node;
        curr_index >>= 1;
        parent_node = arr[curr_index >> 1];
    }

    arr[curr_index] = curr_node;
    num_nodes ++;
    return;
}

template <typename T, typename CMP> 
void Heap<T,CMP>::push(T &&ele) {

    if(num_nodes + 1 >= arr_size) {
        arr = double_arr<T*>(arr, arr_size);
        arr_size *= 2;
    }

    size_t curr_index = num_nodes + 1;
    T* curr_node = new T(std::move(ele));
    T* parent_node = arr[curr_index / 2];

    while(curr_index > 1 && cmp(*parent_node, *curr_node)) {
        arr[curr_index] = parent_node;
        curr_index >>= 1;
        parent_node = arr[curr_index >> 1];
    }
    arr[curr_index] = curr_node;
    num_nodes ++;
    return;
}

template <typename T, typename CMP> 
void Heap<T,CMP>::pop() {

    if(empty())
        return;

    delete arr[1];
    T* curr_node = arr[num_nodes];
    arr[num_nodes] = nullptr;
    num_nodes --;
    
    size_t curr_index = 1, next_index = 0;

    do {
        if(next_index > curr_index) {
            arr[curr_index] = arr[next_index];
            curr_index = next_index;
        }

        next_index = curr_index * 2;
        if(next_index + 1 <= num_nodes && cmp(*arr[next_index], *arr[next_index + 1])) {
            next_index ++;
        } 

    }while(next_index <= num_nodes && cmp(*curr_node, *arr[next_index]));

    if(num_nodes > 0)
        arr[curr_index] = curr_node;
    return;
}

template <typename T, typename CMP> 
inline bool Heap<T,CMP>::empty() const {
    return (size() == 0);
}

template <typename T, typename CMP> 
inline size_t Heap<T,CMP>::capacity() const {
    return (arr_size - 1);
}

template <typename T, typename CMP> 
inline size_t Heap<T,CMP>::size() const {
    return num_nodes;
}

}