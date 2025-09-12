#pragma once

#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <vector>

/* Declaration */
namespace ds_imp {

template <typename T>
class BloomFilter {

    public:
        BloomFilter(size_t bit_count, std::vector<std::function<size_t(const T&)>> funcs);
        BloomFilter(size_t bit_count, size_t func_count, std::function<size_t(const T&)> funcs[]);
        ~BloomFilter();

        void insert(const T &ele);
        void insert(T &&ele);

        bool query(const T &ele);
        bool query(T &&ele);
        void clear();

        static constexpr uint32_t MAX_FUNC_COUNT = 50;
        static constexpr size_t   MAX_BIT_COUNT  = 1000000;

    private:
        size_t bit_count;
        size_t hash_func_count;
        std::vector<size_t> bit_band;
        std::function<size_t(const T&)> hash_funcs[MAX_FUNC_COUNT];
};

}

/* Implementation */
namespace ds_imp {

template <typename T>
BloomFilter<T>::BloomFilter(size_t bit_count, std::vector<std::function<size_t(const T&)>> funcs) {

    size_t func_count = funcs.size();

    if(func_count <= 0 || func_count > BloomFilter<T>::MAX_FUNC_COUNT) {
        throw std::out_of_range("The function count is out of range");
    }

    if(bit_count <= 0 || bit_count > BloomFilter<T>::MAX_BIT_COUNT) {
        throw std::out_of_range("The bit count is out of range");
    }

    this->hash_func_count = func_count;
    this->bit_count = bit_count;
    this->bit_band = std::move(std::vector((bit_count / (sizeof(size_t) * 8)) + 1, 0)); 

    for(auto i = 0u; i < func_count; ++i) {
        this->hash_funcs[i] = std::move(funcs[i]);
    }
}

template <typename T>
BloomFilter<T>::BloomFilter(size_t bit_count, size_t func_count, std::function<size_t(const T&)> funcs[]) {

    if(func_count <= 0 || func_count > BloomFilter<T>::MAX_FUNC_COUNT) {
        throw std::out_of_range("The function count is out of range");
    }

    if(bit_count <= 0 || bit_count > BloomFilter<T>::MAX_BIT_COUNT) {
        throw std::out_of_range("The bit count is out of range");
    }

    this->hash_func_count = func_count;
    this->bit_count = bit_count;
    this->bit_band = std::move(std::vector<size_t>((bit_count / (sizeof(size_t) * 8)) + 1, 0)); 

    for(auto i = 0u; i < func_count; ++i) {
        this->hash_funcs[i] = std::move(funcs[i]);
    }
}

template <typename T>
BloomFilter<T>::~BloomFilter() = default;

template <typename T>
void BloomFilter<T>::insert(const T &ele) {

    auto size_t_bit = (sizeof(size_t) * 8);

    for(auto i = 0u; i < hash_func_count; ++i) {
        auto index = hash_funcs[i](ele) % bit_count;
        bit_band[index / size_t_bit] |= (1 << (index % size_t_bit)); 
    }
}

template <typename T>
void BloomFilter<T>::insert(T &&ele) {

    auto size_t_bit = (sizeof(size_t) * 8);

    for(auto i = 0u; i < hash_func_count; ++i) {
        auto index = hash_funcs[i](ele) % bit_count;
        bit_band[index / size_t_bit] |= (1 << (index % size_t_bit)); 
    }
}

template <typename T>
bool BloomFilter<T>::query(const T &ele) {

    auto size_t_bit = (sizeof(size_t) * 8);

    for(auto i = 0u; i < hash_func_count; ++i) {
        auto index = hash_funcs[i](ele) % bit_count;
        if(bit_band[index / size_t_bit] & (1 << (index % size_t_bit))== 0) {
            return false;
        }
    }
    return true;
}

template <typename T>
bool BloomFilter<T>::query(T &&ele) {

    auto size_t_bit = (sizeof(size_t) * 8);

    for(auto i = 0u; i < hash_func_count; ++i) {
        auto index = hash_funcs[i](ele) % bit_count;
        if((bit_band[index / size_t_bit] & (1 << (index % size_t_bit))) == 0) {
            return false;
        }
    }
    return true;
}

template <typename T>
void BloomFilter<T>::clear() {
    std::fill(bit_band.begin(), bit_band.end(), 0);
}

}
