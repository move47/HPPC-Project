#include "block.h"
#include "node.h"
#include <list>

using namespace std;
#include <iostream>

template<typename T>
Block<T>::Block()
{
    nodes = std::vector<Node<T>*>();
    this->next = NULL;
}

template<typename T>
Block<T>::Block(Node<T> *node, Block<T> *next)
{
    nodes.push_back(node);
    this->next = next;
}

template<typename T>
Block<T>::Block(std::vector<Node<T>*> nodes, Block<T> *next)
{
    this->nodes = nodes;
    this->next = next;
}

template<typename T>
void Block<T>::print()
{  
    for (auto node : nodes)
    {
        std::cout << node->get_key() << " ";
    }
    cout << " | " ;
    
}

template<typename T>
std::vector<Node<T>*> Block<T>::get_nodes()
{
    return nodes;
}

template<typename T>
Block<T>* Block<T>::get_next()
{
    return next;
}

template<typename T>
void Block<T>::set_next(Block<T> *next)
{
    this->next = next;
}

template<typename T>
void Block<T>::lock(){
    this->block_lock.lock();
}

template<typename T>
void Block<T>::unlock(){
    this->block_lock.unlock();
}