#include "b_skip_list.h"
#include "block.cpp"
#include "node.cpp"
#include "kv.cpp"
// #include "kv.h"
#include<list>
using namespace std;

// Default constructor
template<typename T>
BSkipList<T>::BSkipList(){
    this->levels = std::list<Block<T>*>();
    Block<T> *block = new Block<T>(new Node<T>(INT_MIN, 0, nullptr), nullptr); // negative infinity block
    this->levels.push_back(block);
}

// defualt destrcutor
template<typename T>
BSkipList<T>::~BSkipList(){
    // Destructor implementation if needed
}

// Insert a key-value pair
template<typename T>
void BSkipList<T>::insert(int key, T value){
    // TODO Insert implementation
}

// Remove a key-value pair
template<typename T>
void BSkipList<T>::remove(int key){
    std::list<Block<T> *> path = getBlocksInPath(key);
    std::vector<Block<T> *> prev_blocks_for_removed_nodes;

    for ( auto start_block = this->levels.rbegin(); start_block != this->levels.rend(); start_block++ ) {
        Block<T> * prev_block = nullptr;
        Block<T> * current_block = start_block;
        bool found = false;

        while ( current_block != nullptr && !found ) {
            for ( auto node : current_block->get_nodes() ) {
                if ( node->get_key() == key ) {
                    if ( prev_block != nullptr ) {
                        prev_block->set_next( current_block->get_next() );
                        prev_blocks_for_removed_nodes.push_back( prev_block );
                        flag = true;
                        break;
                    }
                }
            }
            prev_block = current_block;
            current_block = current_block->get_next();
        }
    }

    int prev = 0;
    for ( !path.empty() ) {
        Block<T> * current_block = path.front();
        path.pop_front();
        
    }
    
}

// Search for a key
template<typename T>
bool BSkipList<T>::search(int key){
    // TODO Search implementation
    return false;
}

// range query
template<typename T>
std::vector<bool> BSkipList<T>::range_query(int key1, int key2){
    // Range query implementation
    return vector<bool>();
}

// print
template<typename T>
void BSkipList<T>::print(){
    // print levels info
    for (auto level : levels){
        level->print();
    }

}

// get the average size
template<typename T>
std::vector<int> BSkipList<T>::getAverageSize(){
    // Get average size implementation
    return vector<int>();
}

int main(){
    BSkipList<int> b_skip_list;
    b_skip_list.print();
    return 0;
}