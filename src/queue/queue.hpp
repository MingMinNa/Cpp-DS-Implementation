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
        QueueNode<T> *tail;
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

    this->num_nodes = 0;
    this->head = new QueueNode<T>();
    this->tail = head;
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
    
    if(empty())
        return nullptr;
    return head->next->element;
}

template <typename T>
Queue<T>::Result Queue<T>::back() {
    
    if(empty())
        return nullptr;
    return tail->element;
}

template <typename T>
void Queue<T>::enqueue(const T &ele) {
    
    auto new_node = new QueueNode<T>(ele);
    tail->next = new_node;
    tail = new_node;
    num_nodes ++;
}

template <typename T>
void Queue<T>::enqueue(T &&ele) {
    
    auto new_node = new QueueNode<T>(std::move(ele));
    tail->next = new_node;
    tail = new_node;
    num_nodes ++;
}

template <typename T>
void Queue<T>::dequeue() {
    
    if(empty())
        return;
    auto temp_node = head->next;
    head->next = temp_node->next;
    num_nodes --;

    if(head->next == nullptr)
        tail = head;
    
    temp_node->next = nullptr;
    delete temp_node;
}

template <typename T>
inline bool Queue<T>::empty() const {
    return (size() == 0);
}

template <typename T>
inline size_t Queue<T>::size() const {
    return num_nodes;
}

}
