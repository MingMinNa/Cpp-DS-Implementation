#pragma once

#include <cstdint>
#include <cassert>
#include <stdexcept>
#include <variant>
#include <unordered_map>
#include <set>
#include <fstream>
#include <iomanip>

/* Declaration */
namespace ds_imp {

template <typename T>
struct FibNode;

template <typename T>
FibNode<T>* merge_root(FibNode<T>* node_x, FibNode<T>* node_y);

template <typename T>
FibNode<T>* combine_link(FibNode<T>* node_x, FibNode<T>* node_y);

template <typename T>
struct FibNode {
    bool has_show;
    bool marked;
    T element;
    size_t degree;
    size_t num_nodes;
    FibNode<T> *left_sib;
    FibNode<T> *right_sib;
    FibNode<T> *parent;
    FibNode<T> *children;

    /* Constructor */
    FibNode(
        const T &ele = T(), 
        FibNode<T>* parent = nullptr,
        FibNode<T>* child  = nullptr,
        FibNode<T>* left   = nullptr, 
        FibNode<T>* right  = nullptr, 
        size_t degree = 0,
        size_t num_nodes = 1
    );
    FibNode(
        T &&ele, 
        FibNode<T>* parent = nullptr,
        FibNode<T>* child  = nullptr,
        FibNode<T>* left   = nullptr, 
        FibNode<T>* right  = nullptr, 
        size_t degree = 0,
        size_t num_nodes = 1
    );

    /* Destructor */
    ~FibNode();

    void cut_sibling();
    FibNode<T>* find_min_sibling();
    void show(std::ostream &os);

    friend FibNode<T>* merge_root<T>(FibNode<T>* node_x, FibNode<T>* node_y);
    friend FibNode<T>* combine_link<T>(FibNode<T>* node_x, FibNode<T>* node_y);
};

template <typename T> 
class FibHeap {

    using Result = std::variant<std::nullptr_t, T>;

    public:
        FibHeap();
        ~FibHeap();

        Result get_min();
        void insert_node(const T  &ele);
        void insert_node(T &&ele);
        void delete_min();
        void decrease_node(const T &ele, const T &new_ele);
        void decrease_node(const T &ele, const T &&new_ele);
        void show(std::ostream &os);
        inline bool empty() const;
        inline size_t size() const;

        static const uint8_t MAX_DEGREE = 20;
        static const size_t MAX_BUCKET = 10;
    
    private:
        FibNode<T> *root;
        std::unordered_map<T, std::set<FibNode<T>*>> hash_table;
        size_t num_nodes;

        inline size_t map_index(const T &ele) const;
        inline size_t map_index(T &&ele) const;
};

}

/* Implementation */
namespace ds_imp {

/* FibNode */
template <typename T>
FibNode<T>::FibNode(
    const T &ele      , 
    FibNode<T>* parent,
    FibNode<T>* child ,
    FibNode<T>* left  , 
    FibNode<T>* right , 
    size_t degree,
    size_t num_nodes
) : element(ele)  , degree(degree) , num_nodes(num_nodes), 
    parent(parent), children(child) {
    marked    = false;
    has_show  = false;
    left_sib  = ( left == nullptr) ? (this) : (left);
    right_sib = (right == nullptr) ? (this) : (right);
}

template <typename T>
FibNode<T>::FibNode(
    T &&ele           , 
    FibNode<T>* parent,
    FibNode<T>* child ,
    FibNode<T>* left  , 
    FibNode<T>* right , 
    size_t degree,
    size_t num_nodes
) : element(std::move(ele)), degree(degree) , num_nodes(num_nodes), 
    parent(parent)         , children(child) {
    marked    = false;
    has_show  = false;
    left_sib  = ( left == nullptr) ? (this) : (left);
    right_sib = (right == nullptr) ? (this) : (right);
}

template <typename T>
FibNode<T>::~FibNode() {

    if(left_sib != nullptr)
        left_sib->right_sib = nullptr;
    if(right_sib != nullptr) delete right_sib;
    if(children != nullptr)  delete children;
}

template <typename T>
void FibNode<T>::cut_sibling() {

    left_sib->right_sib = right_sib;
    right_sib->left_sib = left_sib;
    left_sib = right_sib = this;
    return;
}

template <typename T>
FibNode<T>* FibNode<T>::find_min_sibling() {

    decltype(this) curr = this, min_ptr = this;
    do {
        curr = curr->left_sib;
        min_ptr = (curr->element < min_ptr->element) ? (curr) : (min_ptr);
    } while(curr != this);

    return min_ptr;
}

template <typename T>
void FibNode<T>::show(std::ostream &os) {

    if(has_show)
        return;

    os << "Parent: ";
    if (!parent) os << "null";
    else         os << std::left << std::setw(4) << parent->element;
    os << ", ";

    os << "Node("   << degree << "): "
       << std::left << std::setw(4) << element << ", ";

    os << "Left: ";
    if (!left_sib) os << "null";
    else           os << std::left << std::setw(4) << left_sib->element;
    os << ", ";

    os << "Right: ";
    if (!right_sib) os << "null";
    else            os << std::left << std::setw(4) << right_sib->element;
    os << ", ";

    os << "Children: ";
    if (!children)  os << "null";
    else            os << std::left << std::setw(4) << children->element;
    os << ", ";

    os << "Marked: " << marked << std::endl;

    has_show = true;
    if (children)  children->show(os);
    if (right_sib) right_sib->show(os);
    has_show = false;

    return;
}

template <typename T>
FibNode<T>* merge_root(FibNode<T>* node_x, FibNode<T>* node_y) {

    using NodePtr = decltype(node_x);
    const auto MAX_DEGREE = FibHeap<T>::MAX_DEGREE;

    NodePtr new_root = combine_link<T>(node_x, node_y);

    if(new_root == nullptr)
        return new_root;
    
    NodePtr degree_ptr[MAX_DEGREE] = {nullptr};

    for(auto &ptr: degree_ptr) 
        ptr = nullptr;
    
    NodePtr curr = new_root, next_ptr;
    uint32_t degree = 0;

    do {

        degree = curr->degree;
        next_ptr = curr->left_sib;

        while(degree < MAX_DEGREE && degree_ptr[degree] != nullptr) {

            if(curr->element < degree_ptr[degree]->element) {
                degree_ptr[degree]->parent = curr;
                (curr->degree)++;
                degree_ptr[degree]->cut_sibling();
                curr->num_nodes += degree_ptr[degree]->num_nodes;
                curr->children = combine_link(curr->children, degree_ptr[degree]);
            }
            else {
                curr->parent = degree_ptr[degree];
                (degree_ptr[degree]->degree)++;
                curr->cut_sibling();
                degree_ptr[degree]->num_nodes += curr->num_nodes;
                degree_ptr[degree]->children = combine_link(curr, degree_ptr[degree]->children);
                curr = degree_ptr[degree];
            }
            degree_ptr[degree] = nullptr;
            degree ++;
        }

        degree_ptr[degree] = curr;
        curr = next_ptr;
    } while(curr != new_root);

    return new_root;
}

template <typename T>
FibNode<T>* combine_link(FibNode<T>* node_x, FibNode<T>* node_y) {

    if     (node_x == nullptr) return node_y;
    else if(node_y == nullptr) return node_x;

    decltype(node_x) root_ptr = (node_x->element < node_y->element) ? (node_x) : (node_y);
    decltype(root_ptr) left_x = node_x->left_sib, left_y = node_y->left_sib; 
    
    node_x->left_sib  = left_y;
    left_x->right_sib = node_y;
    node_y->left_sib  = left_x;
    left_y->right_sib = node_x;

    return root_ptr;
}

/* FibHeap */
template <typename T>
FibHeap<T>::FibHeap() {

    this->num_nodes = 0;
    this->root = nullptr;
}

template <typename T>
FibHeap<T>::~FibHeap() {

    if(root != nullptr)
        delete root;
}

template <typename T>
FibHeap<T>::Result FibHeap<T>::get_min() {

    if(empty())
        return nullptr;
    return root->element;
}

template <typename T>
void FibHeap<T>::insert_node(const T  &ele) {

    FibNode<T> *new_node = new FibNode<T>(ele);
    hash_table[map_index(ele)].insert(new_node);
    num_nodes++;
    
    root = combine_link(root, new_node);
    return;
}

template <typename T>
void FibHeap<T>::insert_node(T &&ele) {

    FibNode<T> *new_node = new FibNode<T>(std::move(ele));
    hash_table[map_index(ele)].insert(new_node);
    num_nodes++;
    
    root = combine_link(root, new_node);
    return;
}

template <typename T>
void FibHeap<T>::delete_min() {

    if(empty())
        return;

    auto ele = root->element;
    if(num_nodes == 1) {
        hash_table[map_index(ele)].clear();
        delete root;
        num_nodes = 0;
        root = nullptr;
        return;
    }

    num_nodes--;
    decltype(root) temp_node = root;
    decltype(root) children  = temp_node->children;
    decltype(root)  sibling  = temp_node->left_sib;
    temp_node->children = nullptr;
    temp_node->parent = nullptr;

    decltype(root) ptr = children;
    
    if(ptr != nullptr) {
        do {
            ptr->marked = false;
            ptr->parent = nullptr;
            ptr = ptr->left_sib;
        } while(ptr != children);
    }

    if(sibling == temp_node) {
        root = children;
    }
    else {
        temp_node->cut_sibling();
        sibling = sibling->find_min_sibling();
        root = merge_root(sibling, children);
    }

    hash_table[map_index(ele)].erase(temp_node);
    delete temp_node;
    return;
}

template <typename T>
void FibHeap<T>::decrease_node(const T &ele, const T &new_ele) {

    if(new_ele >= ele || empty())
        return;
    
    if(hash_table.count(map_index(ele)) == 0 ||  
       hash_table[map_index(ele)].empty())
        return;

    decltype(root) curr = nullptr;

    for(const auto &ptr: hash_table[map_index(ele)]) {
        if(ptr->element == ele) {
            curr = ptr;
            break;
        }
    }

    if(curr == nullptr) {
        throw std::runtime_error("The element is not in the binomial heap");
    }

    hash_table[map_index(ele)].erase(curr);
    curr->element = new_ele;
    hash_table[map_index(curr->element)].insert(curr);

    auto parent = curr->parent;
    auto sibling = curr->left_sib;

    if(parent != nullptr) {

        if(parent->element <= curr->element) 
            return;

        curr->marked = false;
        curr->parent = nullptr;
        (parent->degree) --;
        curr->cut_sibling();
        parent->children = (sibling == curr) ? (nullptr) : (sibling->find_min_sibling());
        root = combine_link(root, curr);
    }

    while(parent != nullptr && parent->parent != nullptr && parent->marked) {

        decltype(parent) grandparent = parent->parent;
        (grandparent->degree) --;
        sibling = parent->left_sib;
        
        parent->cut_sibling();
        grandparent->children = (sibling == parent) ? (nullptr) : (sibling->find_min_sibling());
        parent->marked = false;
        parent->parent = nullptr;
        root = combine_link(root, parent);
        
        parent = grandparent;
    }

    if(parent && parent->parent) {
        parent->marked = true;
    }

    if(curr->element < root->element) {
        root = curr;
    }

    return;
}

template <typename T>
void FibHeap<T>::decrease_node(const T &ele, const T &&new_ele) {

    if(new_ele >= ele || empty())
        return;
    
    if(hash_table.count(map_index(ele)) == 0 ||  
       hash_table[map_index(ele)].empty())
        return;

    decltype(root) curr = nullptr;

    for(const auto &ptr: hash_table[map_index(ele)]) {
        if(ptr->element == ele) {
            curr = ptr;
            break;
        }
    }

    if(curr == nullptr) {
        throw std::runtime_error("The element is not in the binomial heap");
    }

    hash_table[map_index(ele)].erase(curr);
    curr->element = std::move(new_ele);
    hash_table[map_index(curr->element)].insert(curr);

    auto parent = curr->parent;
    auto sibling = curr->left_sib;

    if(parent != nullptr) {

        if(parent->element <= curr->element) 
            return;

        curr->marked = false;
        curr->parent = nullptr;
        (parent->degree) --;
        curr->cut_sibling();
        parent->children = (sibling == curr) ? (nullptr) : (sibling->find_min_sibling());
        root = combine_link(root, curr);
    }

    while(parent != nullptr && parent->parent != nullptr && parent->marked) {

        decltype(parent) grandparent = parent->parent;
        (grandparent->degree) --;
        sibling = parent->left_sib;
        
        parent->cut_sibling();
        grandparent->children = (sibling == parent) ? (nullptr) : (sibling->find_min_sibling());
        parent->marked = false;
        parent->parent = nullptr;
        root = combine_link(root, parent);
        
        parent = grandparent;
    }

    if(parent && parent->parent) {
        parent->marked = true;
    }

    if(curr->element < root->element) {
        root = curr;
    }

    return;
}

template <typename T>
void FibHeap<T>::show(std::ostream &os) {

    os << "The number of nodes: " << num_nodes << std::endl;
    root->show(os);

    for(const auto &p: hash_table) {
        os << "hash_table[" << p.first << "]: ";
        for(const auto &ptr: p.second) {
            os << std::left << std::setw(4) << ptr->element << ", ";
        }
        os << std::endl;
    }
}

template <typename T>
inline bool FibHeap<T>::empty() const {
    return (size() == 0);
}

template <typename T>
inline size_t FibHeap<T>::size() const {
    return num_nodes;
}

template <typename T>
inline size_t FibHeap<T>::map_index(const T &ele) const {
    return std::hash<T>()(ele) % FibHeap<T>::MAX_BUCKET;
}

template <typename T>
inline size_t FibHeap<T>::map_index(T &&ele) const {
    return std::hash<T>()(ele) % FibHeap<T>::MAX_BUCKET;
}

}