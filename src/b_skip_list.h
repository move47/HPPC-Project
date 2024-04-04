#pragma once
#include <list>
#include "block.h"
#include <vector>

template<typename T>
class BSkipList
{
private:
    // Stores the head block at each level
    std::list<Block<T> *> levels;
    // returns the path to the key
    std::list<Block<T> *> getBlocksInPath(int key);

public:
    // Stores the probability of a node being promoted to the next level
    int r = 1;
    const float P_FACTOR = 0.25;
    BSkipList();
    ~BSkipList();
    void insert(int key, T value);
    void remove(int key);
    bool search(int key);
    std::vector<bool> range_query(int start_key, int end_key);
    
    // helper functions
    void print();
    std::vector<int> getAverageSize();
    int getLevels();
};