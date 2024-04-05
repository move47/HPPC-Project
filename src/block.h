#pragma once
#include <list>
#include "node.h"

template<typename T>
class Block
{
public:
    private: 
        
    public:
        std::vector<Node<T>*> nodes;
    
        Block<T> *next; // Pointer to the next block at the same level
     
        Block();
        Block(Node<T> *node, Block *next);
        Block(std::vector<Node<T>*> nodes, Block *next);
        void print();
        std::vector<Node<T>*> get_nodes();
        Block<T>* get_next();
        void set_next(Block *next);
};