#pragma once

#include <cstdint>
#include <cstddef>
#include <variant>
#include <cassert>

/* Declaration */
namespace ds_imp {
    
template <typename T>
struct StackNode {
    T element;
    StackNode<T> *next;

    /* Constructor */
    StackNode(const T &ele = T(), StackNode<T>* next = nullptr);
    StackNode(T &&ele, StackNode<T>* next = nullptr);

    /* Destructor */
    ~StackNode();
};

template <typename T> 
class Stack {

    public:
        Stack();
        ~Stack();

        T* top();
        void push(const T  &ele);
        void push(const T &&ele);
        void pop();
        bool empty() const;
        size_t size() const;
    
    private:
        StackNode<T> *head;
        size_t num_nodes;
};

}

/* Implementation */
namespace ds_imp { 

/* StackNode */
template <typename T>
StackNode<T>::StackNode(const T &ele, StackNode<T>* next) : element(ele), next(next) {}

template <typename T>
StackNode<T>::StackNode(T &&ele, StackNode<T>* next) : element(std::move(ele)), next(next) {}

template <typename T>
StackNode<T>::~StackNode() {
    if(this->next != nullptr) {
        delete this->next;
    }
}

/* Stack */
template <typename T>
Stack<T>::Stack() {
    this->num_nodes = 0;
    this->head = new StackNode<T>();
}

template <typename T>
Stack<T>::~Stack() {
    if(this->head != nullptr)
        delete this->head;
    return;
}

template <typename T>
T* Stack<T>::top() {

    if(this->head == nullptr || this->head->next == nullptr)
        return nullptr;
    return &(this->head->next->element);
}

template <typename T>
void Stack<T>::push(const T &ele) {

    assert(this->head != nullptr);

    StackNode<T> *new_node = new StackNode<T>(ele, this->head->next);
    this->head->next = new_node;
    ++ (this->num_nodes);
    return;
}

template <typename T>
void Stack<T>::push(const T &&ele) {

    assert(this->head != nullptr);

    StackNode<T> *new_node = StackNode(std::move(ele), this->head->next);
    this->head->next = new_node;
    ++ (this->num_nodes);
    return;
}

template <typename T>
void Stack<T>::pop() {

    if(this->empty())
        return;

    assert(this->head != nullptr);

    StackNode<T> *temp_node = this->head->next;
    this->head->next = temp_node->next;
    -- (this->num_nodes);

    temp_node->next = nullptr;
    delete temp_node;
    return;
}

template <typename T>
inline bool Stack<T>::empty() const {
    return (this->num_nodes == 0);
}

template <typename T>
inline size_t Stack<T>::size() const {
    return this->num_nodes;
}

}
