#include "kv.h"
// Constructor
template<typename T>
KV<T>::KV() {
    key = 0;
    value = T();
}

// Constructor with parameters
template<typename T>
KV<T>::KV(int key, T value) {
    this->key = key;
    this->value = value;
}

// Destructor
template<typename T>
KV<T>::~KV() {
    // Destructor implementation if needed
}

// Getter for key
template<typename T>
int KV<T>::get_key() {
    return key;
}

// Getter for value
template<typename T>
T KV<T>::get_value() {
    return value;
}