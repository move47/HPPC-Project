#pragma once
#include <vector>
#include "node.h"
#include "rwlock.h"
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
        RWLock rwLock;

        Block();
        Block(Node<T> *node, Block *next);
        Block(std::vector<Node<T>*> nodes, Block *next);
        void print();
        std::vector<Node<T>*> get_nodes();
        Block<T>* get_next();
        void lock();
        void unlock();
        void r_lock();
        void r_unlock();
        void w_lock();
        void w_unlock();
        void set_next(Block *next);
};