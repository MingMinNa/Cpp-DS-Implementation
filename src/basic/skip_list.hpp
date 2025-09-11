#pragma once

#include "linked_list.hpp"
#include <stdexcept>
#include <cstdint>
#include <cassert>
#include <fstream>
#include <variant>
#include <random>

/* Declaration */
namespace ds_imp {

template <typename T>
struct SL_Node {

    T element;
    SL_Node<T> *next;
    SL_Node<T> *next_level;

    /* Constructor */
    SL_Node(const T &ele = T(), SL_Node<T>* next = nullptr, SL_Node<T>* next_level = nullptr);
    SL_Node(T &&ele, SL_Node<T>* next = nullptr, SL_Node<T>* next_level = nullptr);

    /* Destructor */
    ~SL_Node();
};

template <typename T>
class SkipList {

    using Result = std::variant<std::nullptr_t, T>;

    public:
        SkipList();
        ~SkipList();

        Result search_node(const T &ele);
        void insert_node(const T ele);
        void delete_node(const T &ele);
        void traversal(std::ostream& os);
        bool empty() const;
        size_t size() const;

        static const int32_t MAX_LEVEL = 20;
    
    private:
        SL_Node<T> *level_lists[MAX_LEVEL];
        size_t num_nodes;
        int32_t level_count;
        std::mt19937 generator;

        void add_level_list();
        bool coin_flip();
};

}

/* Implementation */
namespace ds_imp {

/* SL_Node */
template <typename T>
SL_Node<T>::SL_Node(const T &ele, SL_Node<T>* next, SL_Node<T>* next_level) 
    : element(ele), 
      next(next), 
      next_level(next_level) {}

template <typename T>
SL_Node<T>::SL_Node(T &&ele, SL_Node<T>* next, SL_Node<T>* next_level)
    : element(std::move(ele)), 
      next(next), 
      next_level(next_level) {}

template <typename T>
SL_Node<T>::~SL_Node() = default;

/* SkipList */
template <typename T>
SkipList<T>::SkipList() {

    this->num_nodes = 0;
    this->level_count = 1;
    this->generator = std::mt19937(std::random_device{}());

    level_lists[0] = new SL_Node<T>();
    for(uint32_t i = 1; i < SkipList<T>::MAX_LEVEL; ++i) {
        level_lists[i] = new SL_Node<T>(T(), nullptr, level_lists[i - 1]);
    }
}

template <typename T>
SkipList<T>::~SkipList() {

    for(auto &head : level_lists) {
        SL_Node<T> *curr = head, *prev = nullptr;
        while(curr != nullptr) {
            prev = curr;
            curr = curr->next;
            delete prev;
        }
    }
}

template <typename T>
SkipList<T>::Result SkipList<T>::search_node(const T &ele) {

    SL_Node<T> *curr = (level_lists[level_count - 1])->next;

    while(curr != nullptr) {
        if(curr->element == ele && curr->next_level == nullptr) {
            return curr->element;
        }
        
        if(curr->next == nullptr || curr->next->element > ele)  curr = curr->next_level;
        else                                                    curr = curr->next;
    }
    return nullptr;
}

template <typename T>
void SkipList<T>::insert_node(const T ele) {

    auto curr_level = level_count - 1;
    SL_Node<T> *prev_nodes[SkipList<T>::MAX_LEVEL];
    SL_Node<T> *curr = level_lists[curr_level], *new_node, *next_level_ptr = nullptr;

    while(curr != nullptr) {
        
        if(curr->next == nullptr || curr->next->element >= ele) {
            prev_nodes[curr_level--] = curr;
            curr = curr->next_level;
        }
        else
            curr = curr->next;
    }

    for(int32_t i = 0; i < level_count; ++i) {
        new_node = new SL_Node<T>(ele, prev_nodes[i]->next, next_level_ptr);
        prev_nodes[i]->next = new_node;
        next_level_ptr = new_node;

        if(!coin_flip()) break;
    }
    num_nodes ++;

    if(level_count < SkipList<T>::MAX_LEVEL && 
       num_nodes >= static_cast<size_t>(2 << level_count)) {
        add_level_list();
    }

    return;
}

template <typename T>
void SkipList<T>::delete_node(const T &ele) {

    auto curr_level = level_count - 1;
    SL_Node<T> *curr = level_lists[curr_level], *prev = level_lists[curr_level];

    while(curr != nullptr) {
        
        if(curr->next == nullptr || curr->next->element > ele) {
            prev = curr;
            curr = curr->next_level;
        }
        else if(curr->next->element < ele) {
            prev = curr;
            curr = curr->next;
        }
        else /* curr->next->element == ele */ {
            prev = curr;
            curr = curr->next;
            break;
        }
    }

    if(curr == nullptr) // The element is not in the skip list
        return;

    do {
        while(prev != nullptr && prev->next != curr) {
            prev = prev->next;
        } 

        auto temp = curr;
        prev->next = curr->next;
        curr = curr->next_level;
        prev = prev->next_level;
        temp->next = temp->next_level = nullptr;
        delete temp;

    } while(prev != nullptr && curr != nullptr);

    num_nodes --;
    return;
}

template <typename T>
void SkipList<T>::traversal(std::ostream& os) {

    SL_Node<T> *curr;
    for(auto i = level_count - 1; i >= 0; --i) {
        assert(level_lists[i] != nullptr);
        
        os << "List[" << i << "]: ";
        curr = level_lists[i]->next;
        while (curr != nullptr) {
            os << (curr->element) << ", ";
            curr = curr->next;
        }
        os << std::endl;
    }
    return;
}

template <typename T>
inline bool SkipList<T>::empty() const {
    return (this->num_nodes == 0);
}

template <typename T>
inline size_t SkipList<T>::size() const {
    return this->num_nodes;
}

template <typename T>
void SkipList<T>::add_level_list() {

    if(level_count >= SkipList<T>::MAX_LEVEL) {
        throw std::runtime_error("The level lists is full");
    }

    assert(level_count >= 1);

    auto prev_list_ptr = level_lists[level_count - 1]->next, 
         curr_list_ptr = level_lists[level_count];

    decltype(curr_list_ptr) new_node;

    while (prev_list_ptr != nullptr) {

        if(coin_flip()) {
            new_node = new SL_Node<T>(prev_list_ptr->element, nullptr, prev_list_ptr);
            curr_list_ptr->next = new_node;
            curr_list_ptr = new_node;
        }

        prev_list_ptr = prev_list_ptr->next;
    }

    level_count ++;
    return;
}

template <typename T>
bool SkipList<T>::coin_flip() {

    std::bernoulli_distribution dist(0.5);
    return dist(generator);
}

}
