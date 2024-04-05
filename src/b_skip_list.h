#pragma once
#include "block.h"
#include <vector>
#include <utility>

template<typename T>
class BSkipList
{
private:
    // Stores the head block at each level
    std::vector<Block<T> *> levels;
    // Stores the number of levels
    int num_levels = 0;

public:
    // Stores the probability of a node being promoted to the next level
    int r = 1;
    const float P_FACTOR = 0.25;
    BSkipList(int max_levels);
    ~BSkipList();
    void insert(int key, T value);
    void remove(int key);
    std::pair<Block<T>*, int> search(int key);
    std::vector<Block<T>*> range_query(int start_key, int end_key);
    
    // helper functions
    void print();
    std::vector<int> getAverageSize();
    void setLevels(int max_levels);
    int getLevels();
};