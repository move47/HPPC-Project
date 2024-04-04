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
    this->levels = std::list<Block<T>*>();
    Block<T> *block = new Block<T>(new Node<T>(INT_MIN, 0, nullptr), nullptr); // negative infinity block
    this->levels.push_back(block);
}

// defualt destrcutor
template<typename T>
BSkipList<T>::~BSkipList(){
    // Destructor implementation if needed
}

template<typename T>
std::list<Block<T> *> BSkipList<T>::getBlocksInPath(int key)
{
    std::list<Block<T> *> path;
    Block<T> *curr_block = levels.back();
    // printf("Current block key: %d\n", curr_block->get_nodes().front()->get_key());
    // Block<T> *track_block = curr_block;
    Node<T>* prev_node = nullptr;

    while(curr_block != nullptr){
        bool block_present = false;
        // printf("Size of nodes: %d\n", curr_block->get_nodes().size());
        for(auto node : curr_block->get_nodes()){
            if(key > node->get_key()){
                prev_node = node;
            }
            else {
                path.push_front(curr_block);
                curr_block = node->down;
                block_present = true;
                break;
            }
        }
        if(!block_present){
            if(curr_block->get_next() != nullptr){
                // printf("Next block key: %d\n", curr_block->get_next()->get_nodes().front()->get_key());
                if(key < curr_block->get_next()->get_nodes().front()->get_key()){
                    path.push_front(curr_block);
                    curr_block = prev_node->down;
                }
            }
            else{
                // printf("Last block key: %d\n", curr_block->get_nodes().back()->get_key());
                path.push_front(curr_block);
            }
            curr_block = prev_node->down;
        }
    }

    return path;
    
}

// Insert a key-value pair
template<typename T>
void BSkipList<T>::insert(int key, T value){
    // TODO Insert implementation

    std::list<Block<T> *> path = getBlocksInPath(key);
    Block<T> *last_block = nullptr;
    bool insert_complete = false;
    // Block *level = nullptr;

    // printf("Path size: %d\n", path.size());
    while(path.size() > 0){
        Block<T> *curr_block = path.front();
        path.pop_front();

        for(auto node : curr_block->get_nodes()){
            // printf("Node key: %d\n", node->get_key());
            if(node->get_key() > key){
                if(rand() %2 == 0){
                    auto it = std::find(curr_block->nodes.begin(), curr_block->nodes.end(), node);
                    curr_block->nodes.insert(it, new Node<T>(key, value, last_block));
                    return;
                }
                else {
                    std::list<Node<T>*> new_nodes;
                    
                    auto it = std::next(std::find(curr_block->nodes.begin(), curr_block->nodes.end(), node), 1);
                    new_nodes.splice(new_nodes.begin(), curr_block->nodes, it, curr_block->nodes.end());
                    new_nodes.push_back(new Node<T>(key, value, last_block));

                    Block<T> *new_block = new Block<T>(new_nodes, curr_block->get_next());
                    curr_block->set_next(new_block);
                    // int size_first = std::distance(curr_block->nodes.begin(), it) + 1;

                    if(path.empty()){
                        Block<T> *new_level_block = new Block<T>(new Node<T>(INT_MIN, 0, nullptr), nullptr);
                        new_level_block -> nodes.push_back(new Node<T>(key, value, new_block));
                        levels.push_back(new_level_block);
                    }
                    
                    last_block = new_block;
                    insert_complete = true;
                    break;


                    // curr_node-?nodes.erase(it + 1, curr_block->nodes.end());

                }
            }
        }
        if(!insert_complete){
            if(rand() %2 == 0){
                curr_block->nodes.push_back(new Node<T>(key, value, last_block));
                return;
            }
            else{
                Block<T> *new_block = new Block<T>(new Node<T>(key, value, last_block), curr_block->get_next());
                curr_block -> set_next(new_block);

                if(path.empty()){
                    Block<T> *new_level_block = new Block<T>(new Node<T>(INT_MIN, 0, nullptr), nullptr);
                    new_level_block->nodes.push_back(new Node<T>(key, value, new_block));
                    levels.push_back(new_level_block);
                }

                last_block = new_block;
            }
        }
    }


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

    for(int i = 0; i < 10; i++){
        b_skip_list.insert(i, i);
        b_skip_list.print();
        printf("Number %d inserted\n", i);
    }
    
    return 0;
}