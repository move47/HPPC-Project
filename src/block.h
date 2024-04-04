#pragma once
#include <list>
#include "node.h"

template<typename T>
class Block
{
public:
    //std::vector<Node *> vector;
    private: 
        
    public:
        std::list<Node<T>*> nodes;
    
        Block<T> *next; // Pointer to the next block at the same level
     
        Block();
        Block(Node<T> *node, Block *next);
        Block(std::list<Node<T>*> nodes, Block *next);
        void print();
        std::list<Node<T>*> get_nodes();
        Block<T>* get_next();
        void set_next(Block *next);
};