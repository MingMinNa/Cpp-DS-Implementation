#pragma once

template <typename T>
T* double_arr(T* arr, size_t old_size) {
    
    size_t new_size = 2 * old_size;
    T* new_arr = new T[new_size]();

    for(size_t i = 0; i < old_size; ++i) {
        new_arr[i] = std::move(arr[i]);
    }

    delete [] arr;
    return new_arr;
}