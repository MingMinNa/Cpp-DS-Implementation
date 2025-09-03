#pragma once

#include <cstdint>
#include <cstddef>
#include <variant>
#include <cassert>

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

    public:
        Queue();
        ~Queue();

        T* front();
        T* back();
        void enqueue(const T  &ele);
        void enqueue(const T &&ele);
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
    if(this->next != nullptr) {
        delete this->next;
    }
}

/* Queue */
template <typename T>
Queue<T>::Queue() {
    // TODO
}

template <typename T>
Queue<T>::~Queue() {
    // TODO
}

template <typename T>
T* Queue<T>::front() {
    // TODO
}

template <typename T>
T* Queue<T>::back() {
    // TODO
}

template <typename T>
void Queue<T>::enqueue(const T &ele) {
    // TODO
}

template <typename T>
void Queue<T>::enqueue(const T &&ele) {
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
