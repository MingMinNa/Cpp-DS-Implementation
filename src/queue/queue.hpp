#pragma once

#include <cstdint>
#include <cassert>
#include <variant>

/* Declaration */
namespace ds_imp {
    
template <typename T>
struct QueueNode {
    T element;
    QueueNode<T> *next;

    /* Constructor */
    QueueNode(const T &ele = T(), QueueNode<T>* next = nullptr);
    QueueNode(T &&ele, QueueNode<T>* next = nullptr);

    /* Destructor */
    ~QueueNode();
};

template <typename T> 
class Queue {

    using Result = std::variant<std::nullptr_t, T>;

    public:
        Queue();
        ~Queue();

        Result front();
        Result back();
        void enqueue(const T  &ele);
        void enqueue(T &&ele);
        void dequeue();
        bool empty() const;
        size_t size() const;
    
    private:
        QueueNode<T> *head;
        size_t num_nodes;
};

}

/* Implementation */
namespace ds_imp { 

/* QueueNode */
template <typename T>
QueueNode<T>::QueueNode(const T &ele, QueueNode<T>* next) : element(ele), next(next) {}

template <typename T>
QueueNode<T>::QueueNode(T &&ele, QueueNode<T>* next) : element(std::move(ele)), next(next) {}

template <typename T>
QueueNode<T>::~QueueNode() {
    /* // recursive destruction
    if(this->next != nullptr) {
        delete this->next;
    }
    */
}

/* Queue */
template <typename T>
Queue<T>::Queue() {
    // TODO
}

template <typename T>
Queue<T>::~Queue() {
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
Queue<T>::Result Queue<T>::front() {
    // TODO
}

template <typename T>
Queue<T>::Result Queue<T>::back() {
    // TODO
}

template <typename T>
void Queue<T>::enqueue(const T &ele) {
    // TODO
}

template <typename T>
void Queue<T>::enqueue(T &&ele) {
    // TODO
}

template <typename T>
void Queue<T>::dequeue() {
    // TODO
}

template <typename T>
inline bool Queue<T>::empty() const {
    // TODO
}

template <typename T>
inline size_t Queue<T>::size() const {
    // TODO
}

}
