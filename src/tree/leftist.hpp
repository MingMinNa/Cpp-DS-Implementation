#pragma once

#include <cstdint>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <variant>

namespace ds_imp {

template <typename T>
struct LeftistNode;

template <typename T>
LeftistNode<T>* meld_root(LeftistNode<T>* root_x, LeftistNode<T>* root_y);

template <typename T>
struct LeftistNode {
    T element;
    LeftistNode<T> *left;
    LeftistNode<T> *right;
    size_t shortest;
    size_t num_nodes;

    /* Constructor */
    LeftistNode(
        const T &ele = T(), 
        LeftistNode<T>* left = nullptr, 
        LeftistNode<T>* right = nullptr, 
        size_t shortest = 0, 
        size_t num_nodes = 1
    );
    LeftistNode(
        T &&ele, 
        LeftistNode<T>* left = nullptr, 
        LeftistNode<T>* right = nullptr, 
        size_t shortest = 0, 
        size_t num_nodes = 1
    );

    /* Destructor */
    ~LeftistNode();

    friend LeftistNode<T>* meld_root<T>(LeftistNode<T> *root_x, LeftistNode<T> *root_y);
};

template <typename T>
class LeftistTree {

    using Result = std::variant<std::nullptr_t, T>;

    public:

        LeftistTree();
        ~LeftistTree();

        Result get_min();
        void meld(LeftistTree<T> &other);
        void insert_node(const T  &ele);
        void insert_node(T &&ele);
        void delete_min();
        inline bool empty() const;
        inline size_t size() const;

    private:
        LeftistNode<T> *root;
        size_t num_nodes;
};

}

namespace ds_imp {

/* LeftistNode */
template <typename T>
LeftistNode<T>::LeftistNode(
    const T &ele, 
    LeftistNode<T>* left, 
    LeftistNode<T>* right, 
    size_t shortest, 
    size_t num_nodes
) : element(ele), left(left), right(right), shortest(shortest), num_nodes(num_nodes) {}

template <typename T>
LeftistNode<T>::LeftistNode(
    T &&ele, 
    LeftistNode<T>* left, 
    LeftistNode<T>* right, 
    size_t shortest, 
    size_t num_nodes
) : element(std::move(ele)), left(left), right(right), shortest(shortest), num_nodes(num_nodes) {}

template <typename T>
LeftistNode<T>::~LeftistNode() {

    if( left != nullptr) delete left;
    if(right != nullptr) delete right;
}

template <typename T>
LeftistNode<T>* meld_root(LeftistNode<T> *root_x, LeftistNode<T> *root_y) {

    if     (root_x == nullptr)  return root_y;
    else if(root_y == nullptr)  return root_x;

    decltype(root_x) roots[] = {
        nullptr, nullptr
    };

    if(root_x->element > root_y->element) {
        roots[0] = root_y; roots[1] = root_x;
    }
    else {
        roots[0] = root_x; roots[1] = root_y;
    }

    roots[0]->num_nodes += roots[1]->num_nodes;
    roots[0]->right = meld_root<T>(roots[0]->right, roots[1]);

    size_t left_shortest  = ( roots[0]->left == nullptr) ? (0) : (roots[0]->left->shortest);
    size_t right_shortest = (roots[0]->right == nullptr) ? (0) : (roots[0]->right->shortest);

    if(left_shortest < right_shortest) {
        decltype(root_x) t_root = roots[0]->left;
        roots[0]->left = roots[0]->right;
        roots[0]->right = t_root;

        size_t t_shortest = left_shortest;
        left_shortest = right_shortest;
        right_shortest = t_shortest;
    }

    roots[0]->shortest = right_shortest + 1;
    return roots[0];
}

/* LeftistTree */
template <typename T>
LeftistTree<T>::LeftistTree() {

    this->root = nullptr;
    this->num_nodes = 0;
}

template <typename T>
LeftistTree<T>::~LeftistTree() {
    if(root != nullptr)
        delete root;
}

template <typename T>
LeftistTree<T>::Result LeftistTree<T>::get_min() {

    if(empty())
        return nullptr;
    return root->element;
}

template <typename T>
void LeftistTree<T>::meld(LeftistTree<T> &other) {

    this->root = meld_root<T>(this->root, other->root);
    num_nodes = (this->root != nullptr) ? (this->root->num_nodes) : (0);
    other->root = nullptr;
    other->num_nodes = 0;
}

template <typename T>
void LeftistTree<T>::insert_node(const T  &ele) {

    auto new_node = new LeftistNode<T>(ele);
    root = meld_root<T>(root, new_node);
    num_nodes = root->num_nodes;
}

template <typename T>
void LeftistTree<T>::insert_node(T &&ele) {

    auto new_node = new LeftistNode<T>(std::move(ele));
    root = meld_root<T>(root, new_node);
    num_nodes = root->num_nodes;
}

template <typename T>
void LeftistTree<T>::delete_min() {

    if(empty())
        return;
    
    auto temp_node = root;
    root = meld_root<T>(temp_node->left, temp_node->right);
    num_nodes = (root != nullptr) ? (root->num_nodes) : (0);

    temp_node->left = temp_node->right = nullptr;
    delete temp_node;
}

template <typename T>
inline bool LeftistTree<T>::empty() const {
    return (size() == 0);
}

template <typename T>
inline size_t LeftistTree<T>::size() const {
    return num_nodes;
}

}