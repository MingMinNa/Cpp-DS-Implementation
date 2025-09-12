#pragma once

#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <utility>
#include <variant>
#include <stack>
#include <algorithm>

/* Declaration */
namespace ds_imp {

template <typename T>
struct AVL_Node {
    T element;
    AVL_Node<T> *left;
    AVL_Node<T> *right;
    size_t height;
    int32_t bf;

    /* Constructor */
    AVL_Node(const T &ele = T(), AVL_Node<T>* left = nullptr, AVL_Node<T>* right = nullptr);
    AVL_Node(T &&ele, AVL_Node<T>* left = nullptr, AVL_Node<T>* right = nullptr);

    /* Destructor */
    ~AVL_Node();

    AVL_Node<T>* insert(const T &ele);
    AVL_Node<T>* insert(T &&ele);
    void preorder(std::ostream &os);
    void inorder(std::ostream &os);
    void postorder(std::ostream &os);
    void show(std::ostream &os);

    void update();
    AVL_Node<T>* rotate();
    AVL_Node<T>* left_rotate();
    AVL_Node<T>* right_rotate();
};

template <typename T> 
class AVL_Tree {

    using Pair_Result = std::pair<AVL_Node<T>*, AVL_Node<T>*>;
    using Result = std::variant<std::nullptr_t, T>;

    public:
        AVL_Tree();
        ~AVL_Tree();

        Result get_min() const;
        Result get_max() const;
        Pair_Result search_node(const T &ele);
        void insert_node(const T  &ele);
        void insert_node(T &&ele);
        void delete_node(const T ele);
        void modify_node(const T &ele, const T &new_ele);
        void modify_node(const T &ele, const T &&new_ele);
        void preorder(std::ostream &os);
        void inorder(std::ostream &os);
        void postorder(std::ostream &os);
        void show(std::ostream &os);
        inline bool empty() const;
        inline size_t size() const;
        inline size_t height() const;
    
    private:
        AVL_Node<T> *root;
        AVL_Node<T> *max_ptr;
        AVL_Node<T> *min_ptr;
        size_t num_nodes;

        void update_min_ptr();
        void update_max_ptr();
};

}

/* Implementation */
namespace ds_imp {

/* AVL_Node */
template <typename T>
AVL_Node<T>::AVL_Node(const T &ele, AVL_Node<T>* left, AVL_Node<T>* right) 
    : element(ele), left(left), right(right) {
    update();
}

template <typename T>
AVL_Node<T>::AVL_Node(T &&ele, AVL_Node<T>* left, AVL_Node<T>* right) 
    : element(std::move(ele)), left(left), right(right) {
    update();
}

template <typename T>
AVL_Node<T>::~AVL_Node() {

    if( left != nullptr)  delete left;
    if(right != nullptr)  delete right;
}

template <typename T>
AVL_Node<T>* AVL_Node<T>::insert(const T &ele) {

    if     (ele > element && right == nullptr) right = new AVL_Node<T>(ele);
    else if(ele < element &&  left == nullptr)  left = new AVL_Node<T>(ele);
    else if(ele > element) /* (right != nullptr) */ right = right->insert(ele);
    else /* (ele < element) && (left != nullptr) */  left =  left->insert(ele);

    update();
    
    if(-1 <= bf && bf <= 1) 
        return this;

    return this->rotate();
}

template <typename T>
AVL_Node<T>* AVL_Node<T>::insert(T &&ele) {

    if     (ele > element && right == nullptr) right = new AVL_Node<T>(std::move(ele));
    else if(ele < element &&  left == nullptr)  left = new AVL_Node<T>(std::move(ele));
    else if(ele > element) /* (right != nullptr) */ right = right->insert(std::move(ele));
    else /* (ele < element) && (left != nullptr) */  left =  left->insert(std::move(ele));

    update();
    
    if(-1 <= bf && bf <= 1) 
        return this;

    return this->rotate();
}

template <typename T>
void AVL_Node<T>::preorder(std::ostream &os) {
    
    os << element << ", ";
    if( left != nullptr) left->preorder(os);
    if(right != nullptr) right->preorder(os);
}

template <typename T>
void AVL_Node<T>::inorder(std::ostream &os) {

    if( left != nullptr) left->inorder(os);
    os << element << ", ";
    if(right != nullptr) right->inorder(os);
}

template <typename T>
void AVL_Node<T>::postorder(std::ostream &os) {

    if( left != nullptr) left->postorder(os);
    if(right != nullptr) right->postorder(os);
    os << element << ", ";
}

template <typename T>
void AVL_Node<T>::show(std::ostream &os) {

    os << "Node(" << height << "): "
       << std::setw(4) << element << ", ";

    os << "Left: ";
    if (!left) os << "null";
    else       os << std::setw(4) << left->element;
    os << ", ";

    os << "Right: ";
    if (!right) os << "null";
    else        os << std::setw(4) << right->element;
    os << std::endl;

    if(left)  left->show(os);
    if(right) right->show(os);
}

template <typename T>
void AVL_Node<T>::update() {
    size_t left_h  = ( left == nullptr) ? (0llu) : ( left->height),
           right_h = (right == nullptr) ? (0llu) : (right->height);
    this->height = std::max(left_h, right_h) + 1;
    this->bf = (static_cast<int32_t>(left_h)) - right_h;
}

template <typename T>
AVL_Node<T>* AVL_Node<T>::rotate() {

    if(-1 <= bf && bf <= 1)
        return this;

    if     (bf >= 2 && left->bf >= 1)      { // LL
        return this->right_rotate();
    }
    else if(bf >= 2) /* left->bf < 1 */    { // LR
        left = left->left_rotate();
        return this->right_rotate();
    }
    else if(bf <= -2 && right->bf <= -1)   { // RR
        return this->left_rotate();
    }
    else/* (bf <= -2 && right->bf > -1) */ { // RL
        right = right->right_rotate();
        return this->left_rotate();
    }
}

template <typename T>
AVL_Node<T>* AVL_Node<T>::left_rotate() {

    assert(right != nullptr);

    decltype(this) ret_root = right;
    this->right = ret_root->left;
    ret_root->left = this;
    this->update();
    ret_root->update();
    return ret_root;
}

template <typename T>
AVL_Node<T>* AVL_Node<T>::right_rotate() {

    assert(left != nullptr);

    decltype(this) ret_root = left;
    this->left = ret_root->right;
    ret_root->right = this;
    this->update();
    ret_root->update();
    return ret_root;
}

/* AVL_Tree */
template <typename T> 
AVL_Tree<T>::AVL_Tree() 
    : root(nullptr), 
      max_ptr(nullptr), 
      min_ptr(nullptr), 
      num_nodes(0) {}

template <typename T> 
AVL_Tree<T>::~AVL_Tree() {

    if(root != nullptr)
        delete root;
    max_ptr = min_ptr = nullptr;
    num_nodes = 0;
}

template <typename T> 
AVL_Tree<T>::Result AVL_Tree<T>::get_min() const {

    if(empty())
        return nullptr;
    return min_ptr->element;
}

template <typename T> 
AVL_Tree<T>::Result AVL_Tree<T>::get_max() const {

    if(empty())
        return nullptr;
    return max_ptr->element;
}

template <typename T> 
AVL_Tree<T>::Pair_Result AVL_Tree<T>::search_node(const T &ele) {

    decltype(root) parent = nullptr, curr = root;
    while(curr != nullptr && curr->element != ele) {
        
        parent = curr;
        if(curr->element < ele) curr = curr->right;
        else                    curr = curr->left;
    }

    return {parent, curr};
}

template <typename T> 
void AVL_Tree<T>::insert_node(const T &ele) {

    auto search_result = search_node(ele);
    
    if(search_result.second != nullptr) {
        throw std::runtime_error("The element has been in the AVL_Tree");
    }

    if(empty()) root = new AVL_Node<T>(ele);
    else        root = root->insert(ele);
    num_nodes ++;
    update_max_ptr();
    update_min_ptr();
    return;
}

template <typename T> 
void AVL_Tree<T>::insert_node(T &&ele) {

    auto search_result = search_node(ele);
    
    if(search_result.second != nullptr) {
        throw std::runtime_error("The element has been in the AVL_Tree");
    }

    if(empty()) root = new AVL_Node<T>(std::move(ele));
    else        root = root->insert(std::move(ele));
    num_nodes ++;
    update_max_ptr();
    update_min_ptr();
    return;
}

template <typename T> 
void AVL_Tree<T>::delete_node(const T ele) {
    using NodePtr = AVL_Node<T>*;

    NodePtr curr = root, target = nullptr, trg_parent = nullptr;
    std::stack<NodePtr> ancestors;

    while(curr != nullptr) {
        if(curr->element == ele) {
            target = curr;
            break;
        } 
        else if(curr->element < ele) {
            ancestors.push(curr);
            trg_parent = curr;
            curr = curr->right; 
        } 
        else {
            ancestors.push(curr);
            trg_parent = curr;
            curr = curr->left; 
        }
    }

    if (target == nullptr) return; // Not found

    NodePtr new_subtree = nullptr;

    if (target->left == nullptr && target->right == nullptr) {
        // Case 1: Leaf
        new_subtree = nullptr;
        delete target;
    } 
    else if (target->left == nullptr) {
        // Case 2: Only right child
        new_subtree = target->right;
        target->right = nullptr;
        delete target;
    } 
    else if (target->right == nullptr) {
        // Case 3: Only left child
        new_subtree = target->left;
        target->left = nullptr;
        delete target;
    } 
    else {
        // Case 4: Two children
        NodePtr succ = target->right;
        ancestors.push(target);

        while(succ->left != nullptr) {
            ancestors.push(succ);
            succ = succ->left;
        }

        target->element = std::move(succ->element); 

        NodePtr succ_parent = ancestors.top();
        if(succ_parent->left == succ)  succ_parent->left = succ->right;
        else                           succ_parent->right = succ->right;

        succ->right = succ->left = nullptr;
        delete succ;

        new_subtree = target;
    }

    if(trg_parent == nullptr)           root = new_subtree;
    else if(ele < trg_parent->element)  trg_parent->left = new_subtree;
    else                                trg_parent->right = new_subtree;
    
    while(!ancestors.empty()) {
        NodePtr node = ancestors.top();
        ancestors.pop();

        node->update();
        NodePtr rotated = node->rotate();

        if(!ancestors.empty()) {
            NodePtr parent = ancestors.top();
            if(parent->left == node)  parent->left = rotated;
            else                      parent->right = rotated;
        } else {
            root = rotated;
        }
    }

    num_nodes --;
    update_max_ptr();
    update_min_ptr();
}

template <typename T> 
void AVL_Tree<T>::modify_node(const T &ele, const T &new_ele) {

    delete_node(ele);
    insert_node(new_ele);
}

template <typename T> 
void AVL_Tree<T>::modify_node(const T &ele, const T &&new_ele) {

    delete_node(ele);
    insert_node(std::move(new_ele));
}

template <typename T> 
void AVL_Tree<T>::preorder(std::ostream &os) {

    if(root != nullptr)
        root->preorder(os);
    os << std::endl;
}

template <typename T> 
void AVL_Tree<T>::inorder(std::ostream &os) {

    if(root != nullptr)
        root->inorder(os);
    os << std::endl;
}

template <typename T> 
void AVL_Tree<T>::postorder(std::ostream &os) {

    if(root != nullptr)
        root->postorder(os);
    os << std::endl;
}

template <typename T>
void AVL_Tree<T>::show(std::ostream &os) {

    os << "Size: " << std::setw(4) << size() << ", ";
    os << "Height: " << std::setw(4) << height() << ", ";

    os << "Max: ";
    if(max_ptr) os << std::setw(4) << max_ptr->element << ", ";
    else        os << "null, ";

    os << "Min: ";
    if(min_ptr) os << std::setw(4) << min_ptr->element << ", ";
    else        os << "null, ";
    
    os << std::endl;
    if(root) root->show(os);
}

template <typename T> 
inline bool AVL_Tree<T>::empty() const {
    return (size() == 0);
}

template <typename T> 
inline size_t AVL_Tree<T>::size() const {
    return num_nodes;
}

template <typename T> 
size_t AVL_Tree<T>::height() const {
    return (root == nullptr) ? (0) : (root->height);
}

template <typename T> 
void AVL_Tree<T>::update_min_ptr() {
    min_ptr = root;
    while (min_ptr != nullptr && min_ptr->left != nullptr) {
        min_ptr = min_ptr->left;
    }
}

template <typename T> 
void AVL_Tree<T>::update_max_ptr() {
    max_ptr = root;
    while (max_ptr != nullptr && max_ptr->right != nullptr) {
        max_ptr = max_ptr->right;
    }
}
    
}