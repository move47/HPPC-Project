#include "node.h"


// Default constructor
template<typename T>
Node<T>::Node(){}

// Constructor with key, value and level
template<typename T>
Node<T>::Node(int key, T val, int level){
    kv = KV<T>(key, val);
    next.resize(level + 1);
    for (size_t i = 0; i < next.size(); i++){
        next[i] = NULL;
    }
    top_level = level;
}
// Getter for key
template<typename T>
int Node<T>::get_key() {
    return kv.get_key();
}

// Getter for value
template<typename T>
T Node<T>::get_value() {
    return kv.get_value();
}

// Lock the node
template<typename T>
void Node<T>::lock() {
    node_lock.lock();
}

// Unlock the node
template<typename T>
void Node<T>::unlock() {
    node_lock.unlock();
}