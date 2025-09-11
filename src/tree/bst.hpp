#pragma once

#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <utility>
#include <variant>

/* Declaration */
namespace ds_imp {

template <typename T>
struct BST_Node {
    T element;
    BST_Node<T> *left;
    BST_Node<T> *right;

    /* Constructor */
    BST_Node(const T &ele = T(), BST_Node<T>* left = nullptr, BST_Node<T>* right = nullptr);
    BST_Node(T &&ele, BST_Node<T>* left = nullptr, BST_Node<T>* right = nullptr);

    /* Destructor */
    ~BST_Node();

    void preorder(std::ostream &os);
    void inorder(std::ostream &os);
    void postorder(std::ostream &os);
};

template <typename T> 
class BST {

    using Pair_Result = std::pair<BST_Node<T>*, BST_Node<T>*>;
    using Result = std::variant<std::nullptr_t, T>;

    public:
        BST();
        ~BST();

        Result get_min();
        Result get_max();
        Pair_Result search_node(const T &ele);
        void insert_node(const T  &ele);
        void insert_node(T &&ele);
        void delete_node(const T &ele);
        void modify_node(const T &ele, const T &new_ele);
        void modify_node(const T &ele, const T &&new_ele);
        void preorder(std::ostream &os);
        void inorder(std::ostream &os);
        void postorder(std::ostream &os);
        bool empty() const;
        size_t size() const;
    
    private:
        BST_Node<T> *root;
        BST_Node<T> *max_ptr;
        BST_Node<T> *min_ptr;
        size_t num_nodes;
};

}

/* Implementation */
namespace ds_imp {

/* BST_Node */
template <typename T>
BST_Node<T>::BST_Node(const T &ele, BST_Node<T>* left, BST_Node<T>* right) 
    : element(ele), left(left), right(right) {}

template <typename T>
BST_Node<T>::BST_Node(T &&ele, BST_Node<T>* left, BST_Node<T>* right) 
    : element(std::move(ele)), left(left), right(right) {}

template <typename T>
BST_Node<T>::~BST_Node() {

    if( left != nullptr)  delete left;
    if(right != nullptr)  delete right;
}

template <typename T>
void BST_Node<T>::preorder(std::ostream &os) {
    
    os << element << ", ";
    if( left != nullptr) left->preorder(os);
    if(right != nullptr) right->preorder(os);
}

template <typename T>
void BST_Node<T>::inorder(std::ostream &os) {

    if( left != nullptr) left->inorder(os);
    os << element << ", ";
    if(right != nullptr) right->inorder(os);
}

template <typename T>
void BST_Node<T>::postorder(std::ostream &os) {

    if( left != nullptr) left->postorder(os);
    if(right != nullptr) right->postorder(os);
    os << element << ", ";
}

/* BST */
template <typename T> 
BST<T>::BST() 
    : root(nullptr), 
      max_ptr(nullptr), 
      min_ptr(nullptr), 
      num_nodes(0) {}

template <typename T> 
BST<T>::~BST() {

    if(root != nullptr)
        delete root;
    max_ptr = min_ptr = nullptr;
    num_nodes = 0;
}

template <typename T> 
BST<T>::Result BST<T>::get_min() {

    if(empty())
        return nullptr;
    return min_ptr->element;
}

template <typename T> 
BST<T>::Result BST<T>::get_max() {

    if(empty())
        return nullptr;
    return max_ptr->element;
}

template <typename T> 
BST<T>::Pair_Result BST<T>::search_node(const T &ele) {

    decltype(root) parent = nullptr, curr = root;
    while(curr != nullptr && curr->element != ele) {
        
        parent = curr;
        if(curr->element < ele) curr = curr->right;
        else                    curr = curr->left;
    }

    return {parent, curr};
}

template <typename T> 
void BST<T>::insert_node(const T  &ele) {

    auto search_result = search_node(ele);
    
    if(search_result.second != nullptr) {
        throw std::runtime_error("The element has been in the BST");
    }

    auto temp_node = new BST_Node<T>(ele);
    decltype(temp_node) parent = search_result.first;

    // second_result.first will be nullptr when empty() == true
    if(empty()) {
        root = temp_node;
        max_ptr = root;
        min_ptr = root;
    }
    else if(parent->element < ele)  {
        parent->right = temp_node;
        max_ptr = (temp_node->element > max_ptr->element) ? (temp_node) : (max_ptr);
    }
    else {
        parent->left = temp_node;
        min_ptr = (temp_node->element < min_ptr->element) ? (temp_node) : (min_ptr);
    }

    num_nodes ++;
    return;
}

template <typename T> 
void BST<T>::insert_node(T &&ele) {

    auto search_result = search_node(ele);

    if(search_result.second != nullptr) {
        throw std::runtime_error("The element has been in the BST");
    }

    auto temp_node = new BST_Node<T>(std::move(ele));
    decltype(temp_node) parent = search_result.first;

    // second_result.first will be nullptr when empty() == true
    if(empty()) {
        root = temp_node;
        max_ptr = root;
        min_ptr = root;
    }
    else if(parent->element < ele)  {
        parent->right = temp_node;
        max_ptr = (temp_node->element > max_ptr->element) ? (temp_node) : (max_ptr);
    }
    else if(parent->element > ele){
        parent->left = temp_node;
        min_ptr = (temp_node->element < min_ptr->element) ? (temp_node) : (min_ptr);
    }

    num_nodes ++;
    return;
}

template <typename T> 
void BST<T>::delete_node(const T &ele) {

    auto search_result = search_node(ele);

    // The element is not found
    if(search_result.second == nullptr) {
        throw std::runtime_error("The new elemenet is not in the BST");
    }
    
    BST_Node<T> **ptr_addr = nullptr;
    if(search_result.first == nullptr)              ptr_addr = &root;
    else if((search_result.first)->element < ele)   ptr_addr = &((search_result.first)->right);
    else                                            ptr_addr = &((search_result.first)->left);

    auto temp_node = search_result.second;
    if     (temp_node-> left == nullptr)  *ptr_addr = temp_node->right;
    else if(temp_node->right == nullptr)  *ptr_addr = temp_node->left;
    else {
        decltype(temp_node) successor = temp_node->right, prev = nullptr;
        while(successor->left != nullptr) {
            prev = successor;
            successor = successor->left;
        }
        
        // successor don't have left child, but it may have right child.
        if(prev != nullptr) {
            prev->left = successor->right;
            successor->right = temp_node->right;
        }
        
        successor->left = temp_node->left;
        *ptr_addr = successor;
    }

    num_nodes --;
    temp_node->left = temp_node->right = nullptr;
    delete temp_node;

    /* update min_ptr & max_ptr */
    min_ptr = max_ptr = root;
    while (min_ptr != nullptr && min_ptr->left ) min_ptr = min_ptr->left;
    while (max_ptr != nullptr && max_ptr->right) max_ptr = max_ptr->right;

    return;
}

template <typename T> 
void BST<T>::modify_node(const T &ele, const T &new_ele) {

    delete_node(ele);
    insert_node(new_ele);
}

template <typename T> 
void BST<T>::modify_node(const T &ele, const T &&new_ele) {

    delete_node(ele);
    insert_node(std::move(new_ele));
}

template <typename T> 
void BST<T>::preorder(std::ostream &os) {

    if(root != nullptr)
        root->preorder(os);
    os << std::endl;
}

template <typename T> 
void BST<T>::inorder(std::ostream &os) {

    if(root != nullptr)
        root->inorder(os);
    os << std::endl;
}

template <typename T> 
void BST<T>::postorder(std::ostream &os) {

    if(root != nullptr)
        root->postorder(os);
    os << std::endl;
}

template <typename T> 
bool BST<T>::empty() const {
    return size() == 0;
}

template <typename T> 
size_t BST<T>::size() const {
    return num_nodes;
}

}