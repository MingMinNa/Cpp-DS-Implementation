#pragma once

#include "../utils.hpp"
#include <cstdint>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <variant>

/* Declaration */
namespace ds_imp {

template <typename T> 
class Deap {

    using Result = std::variant<std::nullptr_t, T>;

    public:
        Deap();
        ~Deap();

        Result get_min();
        Result get_max();
        void insert(const T  &ele);
        void insert(T &&ele);
        void delete_min();
        void delete_max();
        void show(std::ostream &os);
        inline bool empty()      const;
        inline size_t capacity() const;
        inline size_t size()     const;
    
    private:
        T** arr;
        size_t arr_size;  // capacity()
        size_t num_nodes; // size();

        void move_up(T* curr_node, size_t &curr_index, bool in_min_heap);
        void move_down(size_t &curr_index, bool in_min_heap);
        void check_swap(T* curr_node, size_t &curr_index, bool &in_min_heap);
};

}

/* Implementation */
namespace ds_imp {

template <typename T> 
Deap<T>::Deap() {

    this->num_nodes = 0;
    this->arr_size  = 4;
    this->arr = new T*[arr_size]();
}

template <typename T> 
Deap<T>::~Deap() {

    for(size_t i = 0; i < arr_size; ++i) {
        if(arr[i] != nullptr)
            delete arr[i];
    }
    delete [] arr;
}

template <typename T> 
Deap<T>::Result Deap<T>::get_min() {

    if(empty()) 
        return nullptr;
    return *(arr[2]);
}

template <typename T> 
Deap<T>::Result Deap<T>::get_max() {

    if(empty()) 
        return nullptr;
    else if(num_nodes == 1)
        return *(arr[2]);
    return *(arr[3]);
}

template <typename T> 
void Deap<T>::insert(const T  &ele) {

    if(num_nodes + 2 >= arr_size) { 
        arr = double_arr<T*>(arr, arr_size);
        arr_size *= 2;
    } 

    num_nodes ++;
    size_t curr_index = num_nodes + 1;
    T* curr_node = new T(ele);
    bool in_min_heap; 

    check_swap(curr_node, curr_index, in_min_heap);
    move_up(curr_node, curr_index, in_min_heap);
    arr[curr_index] = curr_node;
    return;
}

template <typename T> 
void Deap<T>::insert(T &&ele) {

    if(num_nodes + 2 >= arr_size) { 
        arr = double_arr<T*>(arr, arr_size);
        arr_size *= 2;
    } 

    num_nodes ++;
    size_t curr_index = num_nodes + 1;
    T* curr_node = new T(std::move(ele));
    bool in_min_heap; 

    check_swap(curr_node, curr_index, in_min_heap);
    move_up(curr_node, curr_index, in_min_heap);
    arr[curr_index] = curr_node;
    return;
}

template <typename T> 
void Deap<T>::delete_min() {

    if(empty())
        return;

    bool in_min_heap = true;
    size_t curr_index = 2;
    T* curr_node = arr[num_nodes + 1];
    arr[num_nodes + 1] = nullptr;

    num_nodes --;
    delete arr[curr_index];
    arr[curr_index] = nullptr;
    
    move_down(curr_index, in_min_heap);
    check_swap(curr_node, curr_index, in_min_heap);
    move_up(curr_node, curr_index, in_min_heap);
    arr[curr_index] = curr_node;
    return;
}

template <typename T> 
void Deap<T>::delete_max() {

    if(num_nodes <= 1) {
        delete_min();
        return;
    }

    bool in_min_heap = false;
    size_t curr_index = 3;
    T* curr_node = arr[num_nodes + 1];
    arr[num_nodes + 1] = nullptr;

    num_nodes --;
    delete arr[curr_index];
    arr[curr_index] = nullptr;
    
    move_down(curr_index, in_min_heap);
    check_swap(curr_node, curr_index, in_min_heap);
    move_up(curr_node, curr_index, in_min_heap);
    arr[curr_index] = curr_node;
    return;
}

template <typename T> 
void Deap<T>::show(std::ostream &os) {

    os << "null" << std::endl;
    for(size_t i = 2, j = 2, k = 0; i <= num_nodes + 1; ++i, ++k) {
        if(k == j) {
            j *= 2;
            k = 0;
            os << std::endl;
        }
        if(arr[i] != nullptr)   os << std::setw(4) << *(arr[i]) << " ";
        else                    os << "null ";
    }
    os << std::endl;
    return;
}

template <typename T> 
inline bool Deap<T>::empty() const {
    return (size() == 0);
}

template <typename T> 
inline size_t Deap<T>::capacity() const {
    return (arr_size - 2);
}

template <typename T> 
inline size_t Deap<T>::size() const {
    return num_nodes;
}

template <typename T> 
void Deap<T>::move_up(T* curr_node, size_t &curr_index, bool in_min_heap) {

    size_t parent_index = curr_index >> 1;

    while((curr_index >= 4) && 
          (( in_min_heap && *curr_node < *arr[parent_index]) ||
           (!in_min_heap && *curr_node > *arr[parent_index]))) {
        
        arr[curr_index] = arr[parent_index];
        curr_index = parent_index;
        parent_index = curr_index >> 1;
    }
    
    return;
}

template <typename T> 
void Deap<T>::move_down(size_t &curr_index, bool in_min_heap) {

    size_t next_index = 0;

    do {
        if(next_index > curr_index) {
            arr[curr_index] = arr[next_index];
            curr_index = next_index;
        }

        next_index = curr_index * 2;

        if((next_index <= num_nodes) && 
           (( in_min_heap && *arr[next_index] > *arr[next_index + 1]) || 
            (!in_min_heap && *arr[next_index] < *arr[next_index + 1]))) {
            next_index ++;
        }
    } while(curr_index * 2 <= num_nodes + 1);
    
    return;
}

template <typename T>
void Deap<T>::check_swap(T* curr_node, size_t &curr_index, bool &in_min_heap) {

    size_t curr_level = 0;

    for(; (1llu << (curr_level + 1)) <= curr_index; curr_level ++);

    in_min_heap = (
        curr_index >= ((1llu << (curr_level - 1)) * 3)
    ) ? (false) : (true);
    
    size_t co_index = curr_index;
    if(in_min_heap)  co_index += (1 << (curr_level - 1));
    else             co_index -= (1 << (curr_level - 1));
    
    // the corresponding node doesn't exist, use the corresponding parent index.
    if(co_index > num_nodes + 1) {
        co_index /= 2;
    }
    else if(co_index * 2 <= num_nodes + 1) {
        co_index *= 2;

        if((co_index + 1 <= num_nodes + 1) && 
           (( in_min_heap && *arr[co_index] > *arr[co_index + 1]) || 
            (!in_min_heap && *arr[co_index] < *arr[co_index + 1]))) {
            co_index ++;
        }
    }

    if((co_index >= 2) && (co_index <= num_nodes + 1) && 
       (( in_min_heap && *curr_node > *arr[co_index]) ||
        (!in_min_heap && *curr_node < *arr[co_index]))) {
        in_min_heap = !in_min_heap;
        arr[curr_index] = arr[co_index];
        curr_index = co_index;
    }

    return;
}

}