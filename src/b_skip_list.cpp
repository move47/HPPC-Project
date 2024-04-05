#include "b_skip_list.h"
#include "block.cpp"
#include "node.cpp"
#include "kv.cpp"
// #include "kv.h"
#include<list>
#include<climits>
#include<algorithm>

using namespace std;

// Default constructor
template<typename T>
BSkipList<T>::BSkipList(){
    this->levels = std::vector<Block<T>*>();
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
  // TODO Implementation


}

// Remove a key-value pair
template<typename T>
void BSkipList<T>::remove(int key){
    // TODO Remove implementation
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
    for (auto it = levels.rbegin(); it != levels.rend(); ++it) {
        Block<T>* curr_level = *it;
        while(curr_level != nullptr){
            curr_level->print();
            curr_level = curr_level->get_next();
        }
        std::cout << std::endl;
        // level->print();
        
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
    // for(int i = 0; i < 10; i++){
    //     b_skip_list.insert(i, i);
    //     b_skip_list.print();
    //     printf("Number %d inserted\n", i);
    // }
    
    return 0;
}