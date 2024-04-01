// Default constructor
#include "node.h"
template<typename T>
Node<T>::Node(){}

// Constructor with key, value and level
template<typename T>
Node<T>::Node(int key, T val, Block<T> *down){
    this->kv = KV<T>(key, val);
    this->down = down;
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

// Destructor
template<typename T>
Node<T>::~Node() {
    // Destructor implementation if needed
}
