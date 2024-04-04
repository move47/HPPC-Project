#pragma once
#include "kv.h"
// #include "block.h"

template<typename T>
class Block;

template<typename T>
class Node{
    private:
        
    public: 
        // Stores the key and value for the Node
        KV<T> kv;

        // Store the reference to the original block wheren this node is head of the block
        Block<T> *down;
          
        Node();
        Node(int key, T value, Block<T> *down);
        ~Node();
        int get_key();
        T get_value();
        Block<T>* get_down();
};

