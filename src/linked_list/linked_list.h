#pragma once

#include <cstdint>
#include <cassert>
#include <fstream>

/* Declaration */
namespace ds_imp {
    
template <typename T>
struct ListNode {
    T element;
    ListNode<T> *next;

    /* Constructor */
    ListNode(const T &ele = T(), ListNode<T>* next = nullptr);
    ListNode(T &&ele, ListNode<T>* next = nullptr);

    /* Destructor */
    ~ListNode();
};

template <typename T> 
class LinkedList {

    public:
        LinkedList();
        ~LinkedList();

        T* search_node(const T &ele);
        void insert_node(const T  &ele);
        void insert_node(T &&ele);
        void delete_node(const T &ele);
        void update_node(const T &ele, const T &new_ele);
        void traversal(std::ostream& os);
        bool empty() const;
        size_t size() const;
    
    private:
        ListNode<T> *head;
        size_t num_nodes;
};

}

/* Implementation */
namespace ds_imp { 

/* ListNode */
template <typename T>
ListNode<T>::ListNode(const T &ele, ListNode<T>* next) : element(ele), next(next) {}

template <typename T>
ListNode<T>::ListNode(T &&ele, ListNode<T>* next) : element(std::move(ele)), next(next) {}

template <typename T>
ListNode<T>::~ListNode() {
    /* // recursive destruction
    if(this->next != nullptr) {
        delete this->next;
    }
    */
}

/* LinkedList */
template <typename T>
LinkedList<T>::LinkedList() {
    this->num_nodes = 0;
    this->head = new ListNode<T>();
}

template <typename T>
LinkedList<T>::~LinkedList() {
    /* // recursive destruction
    if(this->head != nullptr)
        delete this->head;
    return;
    */

    // iterative destruction
    auto curr = this->head, prev = nullptr;

    while(curr != nullptr) {
        prev = curr;
        curr = curr->next;
        delete prev;
    }
}

template <typename T>
T* LinkedList<T>::search_node(const T &ele) {

    assert(this->head != nullptr);

    auto prev = this->head;
    auto curr = prev->next;

    while(curr != nullptr && curr->element < ele) {
        prev = curr;
        curr = curr->next;
    }

    if(curr != nullptr && curr->element == ele) {
        return &(curr->element);
    }
    return nullptr;
}

template <typename T>
void LinkedList<T>::insert_node(const T  &ele) {
    
    assert(this->head != nullptr);

    auto prev = this->head;
    auto curr = prev->next;

    while(curr != nullptr && curr->element < ele) {
        prev = curr;
        curr = curr->next;
    }

    ListNode<T> *new_node = new ListNode(ele, curr);
    prev->next = new_node;
    ++ (this->num_nodes);
    return;
}

template <typename T>
void LinkedList<T>::insert_node(T &&ele) {

    assert(this->head != nullptr);

    auto prev = this->head;
    auto curr = prev->next;

    while(curr != nullptr && curr->element < ele) {
        prev = curr;
        curr = curr->next;
    }

    ListNode<T> *new_node = new ListNode(std::move(ele), curr);
    prev->next = new_node;
    ++ (this->num_nodes);
    return;
}

template <typename T>
void LinkedList<T>::delete_node(const T &ele) {

    assert(this->head != nullptr);

    auto prev = this->head;
    auto curr = prev->next;

    while(curr != nullptr && curr->element < ele) {
        prev = curr;
        curr = curr->next;
    }

    if(curr != nullptr && curr->element == ele) {
        prev->next = curr->next;
        curr->next = nullptr;
        delete curr;
    }
    return;
}

template <typename T>
void LinkedList<T>::update_node(const T &ele, const T &new_ele) {
    
    assert(this->head != nullptr);

    if(search_node(ele) == nullptr) return;

    delete_node(ele);
    insert_node(new_ele);
    return;
}

template <typename T>
void LinkedList<T>::traversal(std::ostream& os) {

    assert(this->head != nullptr);

    auto curr = this->head->next;
    while (curr != nullptr) {
        os << (curr->element) << ", ";
        curr = curr->next;
    }
    os << std::endl;
    return;
}

template <typename T>
inline bool LinkedList<T>::empty() const {
    return (this->num_nodes == 0);
}

template <typename T>
inline size_t LinkedList<T>::size() const {
    return this->num_nodes;
}

}
