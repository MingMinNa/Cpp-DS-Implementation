#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <utility>
#include <fstream>
#include <variant>
#include <random>

/* Declaration */ 
/* Merge-split Treap */
namespace ds_imp {

template <typename T>
struct MS_TreapNode;

template <typename T>
using Split_Result = std::pair<MS_TreapNode<T>*, MS_TreapNode<T>*>;

template <typename T>
MS_TreapNode<T>* merge(MS_TreapNode<T>* node_left, MS_TreapNode<T>* node_right);

template <typename T>
Split_Result<T> split(MS_TreapNode<T>* node_x, const T ele, bool in_first);
    
template <typename T>
struct MS_TreapNode {

    T element;
    uint32_t priority;
    MS_TreapNode<T>* left;
    MS_TreapNode<T>* right;

    /* Constructor */
    MS_TreapNode(uint32_t priority, const T &ele = T(), MS_TreapNode<T>* left = nullptr, MS_TreapNode<T>* right = nullptr);
    MS_TreapNode(uint32_t priority, T &&ele, MS_TreapNode<T>* left = nullptr, MS_TreapNode<T>* right = nullptr);

    /* Destructor */
    ~MS_TreapNode();

    void inorder(std::ostream &os);

    friend MS_TreapNode<T>* merge<T>(MS_TreapNode<T>* node_left, MS_TreapNode<T>* node_right);
    friend Split_Result<T> split<T>(MS_TreapNode<T>* node_x, const T ele, bool in_first);
};

template <typename T>
class MS_Treap {

    public:
        MS_Treap();
        ~MS_Treap();

        void insert_node(const T  &ele);
        void insert_node(T &&ele);
        void erase_node(const T ele);
        bool find_node(const T ele);
        void inorder(std::ostream &os);
        inline bool empty() const;
        inline size_t size() const;

    private:
        MS_TreapNode<T> *root;
        size_t num_nodes;
        std::mt19937 generator;
};

}

/* Implementation */
namespace ds_imp {

/* MS_TreapNode */
template <typename T>
MS_TreapNode<T>::MS_TreapNode(uint32_t priority, const T &ele, MS_TreapNode<T>* left, MS_TreapNode<T>* right)
    : element(ele), priority(priority), left(left), right(right) {}

template <typename T>
MS_TreapNode<T>::MS_TreapNode(uint32_t priority, T &&ele, MS_TreapNode<T>* left, MS_TreapNode<T>* right)
    : element(std::move(ele)), priority(priority), left(left), right(right) {}

template <typename T>
MS_TreapNode<T>::~MS_TreapNode() {
    
    if( left != nullptr)  delete left;
    if(right != nullptr)  delete right;
}

template <typename T>
void MS_TreapNode<T>::inorder(std::ostream &os) {

    if( left != nullptr) left->inorder(os);
    os << element << ", ";
    if(right != nullptr) right->inorder(os);
}

template <typename T>
MS_TreapNode<T>* merge(MS_TreapNode<T>* node_left, MS_TreapNode<T>* node_right) {

    if(node_left == nullptr) return node_right;
    if(node_right == nullptr) return node_left;

    if(node_left->priority < node_right->priority) {
        node_left->right = merge<T>(node_left->right, node_right);
        return node_left;
    }
    else {
        node_right->left = merge<T>(node_left, node_right->left);
        return node_right;
    }
}

template <typename T>
Split_Result<T> split(MS_TreapNode<T>* node_x, const T ele, bool in_first) {

    if(node_x == nullptr) return {nullptr, nullptr};

    if (in_first && ele < node_x->element) {
        auto res = split<T>(node_x->left, ele, in_first);
        node_x->left = res.second;
        return {res.first, node_x};
    }
    else if (in_first) /* ele >= node_x->element */ {
        auto res = split<T>(node_x->right, ele, in_first);
        node_x->right = res.first;
        return {node_x, res.second};
    }
    else if /* !in_first */ (ele <= node_x->element) {
        auto res = split<T>(node_x->left, ele, in_first);
        node_x->left = res.second;
        return {res.first, node_x};
    }
    else /* !in_first && ele > node_x->element */ {
        auto res = split<T>(node_x->right, ele, in_first);
        node_x->right = res.first;
        return {node_x, res.second};
    }
}

/* MS_Treap */
template <typename T>
MS_Treap<T>::MS_Treap()
    : root(nullptr),  
      num_nodes(0) {
    
    this->generator = std::mt19937(std::random_device{}());
    
}

template <typename T>
MS_Treap<T>::~MS_Treap() {

    if(root != nullptr)
        delete root;
    root = nullptr;
    num_nodes = 0;
}

template <typename T>
void MS_Treap<T>::insert_node(const T  &ele) {

    std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);

    auto res = split<T>(root, ele);
    MS_TreapNode<T>* new_node = new MS_TreapNode<T>(dist(generator), ele);
    return merge<T>(merge<T>(res.first, new_node), res.second);
}

template <typename T>
void MS_Treap<T>::insert_node(T &&ele) {

    std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);

    auto res = split<T>(root, ele, true);
    MS_TreapNode<T>* new_node = new MS_TreapNode<T>(dist(generator), std::move(ele));
    root = merge<T>(merge<T>(res.first, new_node), res.second);
}

template <typename T>
void MS_Treap<T>::erase_node(const T ele) {

    auto node_first = split<T>(root, ele, true);
    auto node_second = split<T>(node_first.first, ele, false);
    root = merge<T>(node_second.first, node_first.second);
}

template <typename T>
bool MS_Treap<T>::find_node(const T ele) {

    decltype(root) curr = root;
    
    while(curr != nullptr && curr->element != ele) {
        if     (curr->element < ele)   curr = curr->right;
        else if(curr->element > ele)   curr = curr->left;  
    }

    return (curr) ? (true) : (false);
}

template <typename T>
void MS_Treap<T>::inorder(std::ostream &os) {

    if(root != nullptr)
        root->inorder(os);
    os << std::endl;
}

template <typename T>
inline bool MS_Treap<T>::empty() const {
    return (size() == 0);
}

template <typename T>
inline size_t MS_Treap<T>::size() const {
    return num_nodes;
}

}
