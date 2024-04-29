#pragma once
#include <vector>
#include "node.h"
#include <mutex>

template<typename T>
class Block
{
public:
    private: 
        
    public:
        std::vector<Node<T>*> nodes;
    
        Block<T> *next; // Pointer to the next block at the same level
        std::mutex block_lock;

        Block();
        Block(Node<T> *node, Block *next);
        Block(std::vector<Node<T>*> nodes, Block *next);
        void print();
        std::vector<Node<T>*> get_nodes();
        Block<T>* get_next();
        void lock();
        void unlock();
        void set_next(Block *next);
};