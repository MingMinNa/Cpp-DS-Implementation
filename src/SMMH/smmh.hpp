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
class SMMH {

    using Result = std::variant<std::nullptr_t, T>;

    public:
        SMMH();
        ~SMMH();

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

        void move_up(T* curr_node, size_t &curr_index);
        void move_down(T* curr_node, size_t &curr_index, bool in_min_level);
        void check_swap(T* curr_node, size_t &curr_index);
};

}

/* Implementation */
namespace ds_imp {

template <typename T> 
SMMH<T>::SMMH() {
    this->num_nodes = 0;
    this->arr_size  = 4;
    this->arr = new T*[arr_size]();
}

template <typename T> 
SMMH<T>::~SMMH() {

    for(size_t i = 0; i < arr_size; ++i) {
        if(arr[i] != nullptr)
            delete arr[i];
    }
    delete [] arr;
}

template <typename T> 
SMMH<T>::Result SMMH<T>::get_min() {

    if(empty()) 
        return nullptr;
    return *(arr[2]);
}

template <typename T> 
SMMH<T>::Result SMMH<T>::get_max() {

    if(empty()) 
        return nullptr;
    else if(num_nodes == 1)
        return *(arr[2]);
    return *(arr[3]);
}

template <typename T> 
void SMMH<T>::insert(const T  &ele) {

    if(num_nodes + 2 >= arr_size) {
        arr = double_arr<T*>(arr, arr_size);
        arr_size *= 2;
    }

    num_nodes ++;
    size_t curr_index = num_nodes + 1;
    T* curr_node = new T(ele);

    check_swap(curr_node, curr_index);
    move_up(curr_node, curr_index);
    arr[curr_index] = curr_node;
    return;
}

template <typename T> 
void SMMH<T>::insert(T &&ele) {

    if(num_nodes + 2 >= arr_size) {
        arr = double_arr<T*>(arr, arr_size);
        arr_size *= 2;
    }

    num_nodes ++;
    size_t curr_index = num_nodes + 1;
    T* curr_node = new T(std::move(ele));

    check_swap(curr_node, curr_index);
    move_up(curr_node, curr_index);
    arr[curr_index] = curr_node;
    return;
}

template <typename T> 
void SMMH<T>::delete_min() {

    if(empty())
        return;

    num_nodes --;
    delete arr[2];
    arr[2] = nullptr;
    
    if(empty())
        return;
    
    size_t curr_index = 2;
    T* curr_node = arr[num_nodes + 2];
    arr[num_nodes + 2] = nullptr;
    
    move_down(curr_node, curr_index, true);
    check_swap(curr_node, curr_index);
    move_up(curr_node, curr_index);
    arr[curr_index] = curr_node;
    return;
}

template <typename T> 
void SMMH<T>::delete_max() {

    if(num_nodes < 2) {
        delete_min();
        return;
    }

    num_nodes --;
    delete arr[3];
    arr[3] = nullptr;

    if(num_nodes == 1)
        return;
    
    size_t curr_index = 3;
    T* curr_node = arr[num_nodes + 2];
    arr[num_nodes + 2] = nullptr;
    
    move_down(curr_node, curr_index, false);
    check_swap(curr_node, curr_index);
    move_up(curr_node, curr_index);
    arr[curr_index] = curr_node;
    return;
}

template <typename T> 
void SMMH<T>::show(std::ostream &os) {

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
inline bool SMMH<T>::empty() const {
    return (size() == 0);
}

template <typename T> 
inline size_t SMMH<T>::capacity() const {
    return (arr_size - 2);
}

template <typename T> 
inline size_t SMMH<T>::size() const {
    return num_nodes;
}

template <typename T> 
void SMMH<T>::move_up(T* curr_node, size_t &curr_index) {

    while(curr_index >= 4) {

        int64_t parent_index = curr_index / 2;
        int64_t min_index = parent_index + ((parent_index & 1) ? (-1) : (0)),
                max_index = parent_index + ((parent_index & 1) ? ( 0) : (1));

        if(*curr_node < *arr[min_index]) {
            arr[curr_index] = arr[min_index];
            curr_index = min_index;
        }
        else if(*curr_node > *arr[max_index]) {
            arr[curr_index] = arr[max_index];
            curr_index = max_index;
        }
        else 
            break;
    }
    return;
}

template <typename T> 
void SMMH<T>::move_down(T* curr_node, size_t &curr_index, bool in_min_level) {

    if(2 * (curr_index / 2 * 2) > num_nodes + 1) 
        return;

    size_t next_index_arr[] = {
        2 * (curr_index / 2 * 2), 
        2 * (curr_index / 2 * 2) + 1, 
        2 * (curr_index / 2 * 2) + 2, 
        2 * (curr_index / 2 * 2) + 3  
    };

    size_t next_index = next_index_arr[0];
    T* next_node = arr[next_index];

    for(int i = 0; i < 4; ++i) {
        if(next_index_arr[i] > num_nodes + 1) break;

        if(( in_min_level && *next_node > *arr[next_index_arr[i]]) ||
           (!in_min_level && *next_node < *arr[next_index_arr[i]])) {
            
            next_node = arr[next_index_arr[i]];
            next_index = next_index_arr[i];
        }
    }

    if(( in_min_level && *curr_node > *next_node) ||
       (!in_min_level && *curr_node < *next_node)) {

        arr[curr_index] = next_node;
        curr_index = next_index;
        move_down(curr_node, curr_index, in_min_level);
    }
    return;
}

template <typename T>
void SMMH<T>::check_swap(T* curr_node, size_t &curr_index) {

    if(curr_index % 2 == 1 && *curr_node < *arr[curr_index - 1]) {
        arr[curr_index] = arr[curr_index - 1];
        curr_index --;
    }
    else if(curr_index % 2 == 0 && 
            curr_index <= num_nodes && 
            *curr_node > *arr[curr_index + 1]) {
        
        arr[curr_index] = arr[curr_index + 1];
        curr_index ++;
    }
}

}