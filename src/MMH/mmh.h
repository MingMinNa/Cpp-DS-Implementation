#pragma once

#include "../utils.h"
#include <cstdint>
#include <cassert>
#include <functional>
#include <stdexcept>
#include <fstream>
#include <iomanip>

/* Declaration */
namespace ds_imp {

template <typename T> 
class MMH {

    public:
        MMH();
        ~MMH();

        T* get_min();
        T* get_max();
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
        size_t pow_index; 

        void move_up(T* curr_node, size_t &curr_index, bool &in_min_level);
        void move_down(T* curr_node, size_t &curr_index, bool &in_min_level);
};

}

/* Implementation */
namespace ds_imp {

template <typename T> 
MMH<T>::MMH() {
    this->num_nodes = 0;
    this->arr_size  = 4;
    this->arr = new T*[arr_size]();
    this->pow_index = 0;
}

template <typename T> 
MMH<T>::~MMH() {

    for(size_t i = 0; i < arr_size; ++i) {
        if(arr[i] != nullptr)
            delete arr[i];
    }
    delete [] arr;
}

template <typename T> 
T* MMH<T>::get_min() {

    if(empty()) 
        return nullptr;
    return arr[1];
}

template <typename T> 
T* MMH<T>::get_max() {

    if(empty()) 
        return nullptr;
    
    size_t n = (num_nodes >= 3) ? (3) : (num_nodes);
    T* max_node = arr[1];

    for(size_t i = 1; i <= n; ++i) {
        max_node = (*max_node > *arr[i]) ? (max_node) : (arr[i]);
    }
    return max_node;
}

template <typename T> 
void MMH<T>::insert(const T  &ele) {

    if(num_nodes + 1 >= arr_size) {
        arr = double_arr<T*>(arr, arr_size);
        arr_size *= 2;
    }

    num_nodes ++;
    if((1llu << (pow_index + 1)) <= num_nodes) {
        pow_index ++;
    }

    size_t curr_index = num_nodes;
    bool in_min_level = (pow_index + 1) % 2;
    T* curr_node = new T(ele);

    move_up(curr_node, curr_index, in_min_level);
    arr[curr_index] = curr_node;
    return;
}

template <typename T> 
void MMH<T>::insert(T &&ele) {

    if(num_nodes + 1 >= arr_size) {
        arr = double_arr<T*>(arr, arr_size);
        arr_size *= 2;
    }

    num_nodes ++;
    if((1llu << (pow_index + 1)) <= num_nodes) {
        pow_index ++;
    }

    size_t curr_index = num_nodes;
    bool in_min_level = (pow_index + 1) % 2;
    T* curr_node = new T(std::move(ele));

    move_up(curr_node, curr_index, in_min_level);
    arr[curr_index] = curr_node;
    return;
}

template <typename T> 
void MMH<T>::delete_min() {

    if(empty())
        return;
    
    size_t curr_index = 1;
    bool in_min_level = true;
    T* curr_node = arr[num_nodes];
    arr[num_nodes] = nullptr;
    num_nodes --;

    delete arr[curr_index];
    arr[curr_index] = nullptr;

    size_t pow_2 = 1u << pow_index;

    if(num_nodes < pow_2 && pow_index > 0) {
        pow_index --;
    }

    if(curr_index > num_nodes) {
        return;
    }
    
    move_down(curr_node, curr_index, in_min_level);
    move_up(curr_node, curr_index, in_min_level);
    arr[curr_index] = curr_node;
    return;
}

template <typename T> 
void MMH<T>::delete_max() {

    if(empty())
        return;
    else if(num_nodes == 1) {
        delete_min();
        return;
    }

    size_t curr_index = 2;
    bool in_min_level = false;
    
    if(curr_index + 1 <= num_nodes && 
       *arr[curr_index] < *arr[curr_index + 1]) {
        curr_index ++;
    }
    
    T* curr_node = arr[num_nodes];
    arr[num_nodes] = nullptr;
    num_nodes --;

    delete arr[curr_index];
    arr[curr_index] = nullptr;

    size_t pow_2 = 1u << pow_index;

    if(num_nodes < pow_2 && pow_index > 0) {
        pow_index --;
    }

    if(curr_index > num_nodes) {
        return;
    }
    
    move_down(curr_node, curr_index, in_min_level);
    move_up(curr_node, curr_index, in_min_level);
    arr[curr_index] = curr_node;
    return;
}

template <typename T> 
void MMH<T>::show(std::ostream &os) {

    for(size_t i = 1, j = 1, k = 0; i <= num_nodes; ++i, ++k) {
        if(k == j) {
            j *= 2;
            k = 0;
            os << std::endl;
        }
        os << std::setw(4) << *(arr[i]) << " ";
    }
    os << std::endl;
    return;
}

template <typename T> 
inline bool MMH<T>::empty() const {
    return (size() == 0);
}

template <typename T> 
inline size_t MMH<T>::capacity() const {
    return (arr_size - 1);
}

template <typename T> 
inline size_t MMH<T>::size() const {
    return num_nodes;
}

template <typename T> 
void MMH<T>::move_up(T* curr_node, size_t &curr_index, bool &in_min_level) {

    T* parent_node = arr[curr_index >> 1];
    /* Compare with the parent node
        swap with the parent node if the following two conditions happen 
        (1) if the current node in min level but is greater than the parent (which is in max level)
        (2) if the current node in max level but is less than the parent (which is in min level)
    */
    if(curr_index >= 2 && 
      (( in_min_level && *curr_node > *parent_node) ||    // condition 1
       (!in_min_level && *curr_node < *parent_node))) {   // condition 2

        arr[curr_index] = parent_node;
        in_min_level = !in_min_level;
        curr_index >>= 1;
    }

    T* grandparent_node = arr[curr_index >> 2];

    while(curr_index >= 4 && 
         (( in_min_level && *curr_node < *grandparent_node) ||
          (!in_min_level && *curr_node > *grandparent_node))) {
        
        arr[curr_index] = grandparent_node;
        curr_index >>= 2;
        grandparent_node = arr[curr_index >> 2];
    }
    return;
}

template <typename T> 
void MMH<T>::move_down(T* curr_node, size_t &curr_index, bool &in_min_level) {

    if(curr_index * 2 > num_nodes)
        return;

    size_t next_index_arr[] = {
        2 * curr_index, 2 * curr_index + 1, 
        4 * curr_index, 4 * curr_index + 1, 4 * curr_index + 2, 4 * curr_index + 3
    };
    
    size_t next_index = next_index_arr[0];
    T* next_node = arr[next_index];

    for(int i = 0; i < 6; ++i) {
        if(next_index_arr[i] > num_nodes) break;

        if(( in_min_level && *next_node > *arr[next_index_arr[i]])  ||
           (!in_min_level && *next_node < *arr[next_index_arr[i]])) {
            next_index = next_index_arr[i];
            next_node = arr[next_index];
        }
    }

    arr[curr_index] = next_node;
    curr_index = next_index;

    if(next_index == next_index_arr[0] || next_index == next_index_arr[1]) {
        in_min_level = !in_min_level;
    }
    else {
        move_down(curr_node, curr_index, in_min_level);
    }
    return;
}

}