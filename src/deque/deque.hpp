#pragma once

#include <cstdint>
#include <cassert>
#include <fstream>

/* Declaration */
namespace ds_imp {
    
template <typename T>
struct DequeNode {
    T element;
    DequeNode<T> *prev;
    DequeNode<T> *next;

    /* Constructor */
    DequeNode(const T &ele = T(), DequeNode<T>* prev = nullptr, DequeNode<T>* next = nullptr);
    DequeNode(T &&ele, DequeNode<T>* prev = nullptr, DequeNode<T>* next = nullptr);

    /* Destructor */
    ~DequeNode();
};

template <typename T> 
class Deque {

    public:
        Deque();
        ~Deque();

        T* front();
        T* back();
        void push_front(const T  &ele);
        void push_front(T &&ele);
        void push_back(const T  &ele);
        void push_back(T &&ele);
        void pop_front();
        void pop_back();
        void show(std::ostream &os);
        bool empty() const;
        size_t size() const;
    
    private:
        DequeNode<T> *head;
        DequeNode<T> *tail;
        size_t num_nodes;
};

}

/* Implementation */
namespace ds_imp { 

/* DequeNode */
template <typename T>
DequeNode<T>::DequeNode(const T &ele, DequeNode<T>* prev, DequeNode<T>* next) 
    : element(ele), prev(prev), next(next) {}

template <typename T>
DequeNode<T>::DequeNode(T &&ele, DequeNode<T>* prev, DequeNode<T>* next) 
    : element(std::move(ele)), prev(prev), next(next) {}

template <typename T>
DequeNode<T>::~DequeNode() {
    /* // recursive destruction
    if(this->next != nullptr) {
        delete this->next;
    }
    */
}

/* Deque */
template <typename T>
Deque<T>::Deque() {

    this->num_nodes = 0;
    this->head = new DequeNode<T>();
    this->tail = new DequeNode<T>();
    this->head->next = this->tail;
    this->tail->prev = this->head;
}

template <typename T>
Deque<T>::~Deque() {
    /* // recursive destruction
    if(this->head != nullptr)
        delete this->head;
    return;
    */

    // iterative destruction
    decltype(this->head) curr = this->head, prev = nullptr;

    while(curr != nullptr) {
        prev = curr;
        curr = curr->next;
        delete prev;
    }
}

template <typename T>
T* Deque<T>::front() {

    if(empty())
        return nullptr;
    return &(head->next->element);
}

template <typename T>
T* Deque<T>::back() {
    
    if(empty())
        return nullptr;
    return &(tail->prev->element);
}

template <typename T>
void Deque<T>::push_front(const T &ele) {

    DequeNode<T> *new_node = new DequeNode<T>(
        ele, head, head->next);
    head->next->prev = new_node;
    head->next = new_node;
    num_nodes ++;
}

template <typename T>
void Deque<T>::push_front(T &&ele) {
    
    DequeNode<T> *new_node = new DequeNode<T>(
        std::move(ele), head, head->next);
    head->next->prev = new_node;
    head->next = new_node;
    num_nodes ++;
}

template <typename T>
void Deque<T>::push_back(const T &ele) {
    
    DequeNode<T> *new_node = new DequeNode<T>(
        ele, tail->prev, tail);
    tail->prev->next = new_node;
    tail->prev = new_node;
    num_nodes ++;
}

template <typename T>
void Deque<T>::push_back(T &&ele) {
    
    DequeNode<T> *new_node = new DequeNode<T>(
        std::move(ele), tail->prev, tail);
    tail->prev->next = new_node;
    tail->prev = new_node;
    num_nodes ++;
}

template <typename T>
void Deque<T>::pop_front() {

    if(empty())
        return;

    auto temp_node = head->next;
    head->next = temp_node->next;
    temp_node->next->prev = head;
    delete temp_node;
    num_nodes --;
}

template <typename T>
void Deque<T>::pop_back() {
    
    if(empty())
        return;

    auto temp_node = tail->prev;
    tail->prev = temp_node->prev;
    temp_node->prev->next = tail;
    delete temp_node;
    num_nodes --;
}

template <typename T>
void Deque<T>::show(std::ostream &os) {
    
    auto curr = head->next;
    while(curr != tail) {
        os << curr->element << ", ";
        curr = curr->next;
    }
    os << std::endl;
}

template <typename T>
inline bool Deque<T>::empty() const {
    return (size() == 0);
}

template <typename T>
inline size_t Deque<T>::size() const {
    return num_nodes;
}

}
