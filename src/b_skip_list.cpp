#include "b_skip_list.h"
#include "block.cpp"
#include "node.cpp"
#include "kv.cpp"
// #include "kv.h"
#include<list>
#include<climits>
#include<algorithm>
#include<vector>
#include<utility>

using namespace std;

// Default constructor
template<typename T>
BSkipList<T>::BSkipList(){
    Block<T>* prev_block = nullptr;
    this->levels = vector<Block<T>*>(10);

    for ( int i = 9; i >= 0; i-- ) {
        Block<T> *block = new Block<T>(new Node<T>(INT_MIN, 0, nullptr), nullptr); // negative infinity block
        this->levels[i] = block;
        if ( prev_block != nullptr ) {
            prev_block->nodes[0]->down = block;
        }
        prev_block = block;
    }
}

// defualt destrcutor
template<typename T>
BSkipList<T>::~BSkipList(){
    // Destructor implementation if needed
}

// Insert a key-value pair
template<typename T>
void BSkipList<T>::insert(int key, T value){

    int num_flips = 0;
    while (num_flips < 10 && rand() % 2 == 0) {
        num_flips++;
    }

    // cout << "Number of flips: " << num_flips << endl;

    Block<T>* greatest_min_block = levels[levels.size() - 1];
    // greatest_min_block->print();
    bool first_insert = false;
    Node<T>* parent = nullptr;

    for ( int l = levels.size() - 1; l >= 0; l-- ) {
        if ( l > num_flips ) {
            // Keep track of the greatest min block node
            // Iterate over vector of the block greatesr_min_block
            // This is not required

            Block<T>* curr_block = greatest_min_block;

            int size = curr_block->nodes.size();
            for ( int i = 0; i < size; i++ ) {
                if ( key > curr_block->nodes[i]->get_key() ) {
                    greatest_min_block = curr_block->nodes[i]->down;
                }
                else {
                    break;
                }
            }
            // greatest_min_block->print();
            // cout << "Inserting " << key << " at level " << l << endl;
        }
        else if ( l <= num_flips ) {
            // Find the place of insertion
            // cout << "Line 72\n";
            Block<T>* curr_block = greatest_min_block;

            // cout << curr_block->nodes.size() << endl;

            int insert_index = curr_block->nodes.size();

            int size = curr_block->nodes.size();
            for ( int i = 0; i < size; i++ ) {
                if ( key < curr_block->nodes[i]->get_key() ) {
                    insert_index = i;
                    break;
                }
            }

            // Because of INT_MIN, insert_index will always be greater than or equal to 1
            greatest_min_block = curr_block->nodes[insert_index - 1]->down;

            // cout << "Line number 89" << endl;
            // this->print();

            if ( !first_insert ) {
                // Create a new block
                Node<T>* new_node = new Node<T>(key, value, nullptr);
                curr_block->nodes.insert(curr_block->nodes.begin() + insert_index, new_node);
                parent = new_node;
                first_insert = true;
                // cout << "First insert" << endl;
                // cout << "Inserting " << key << " at level " << l << endl;
                // // this->print();
            }
            else {
                Node<T>* new_node = new Node<T>(key, value, nullptr);
                curr_block->nodes.insert(curr_block->nodes.begin() + insert_index, new_node);
                vector<Node<T>*> right_block_nodes = vector<Node<T>*>(curr_block->nodes.begin() + insert_index, curr_block->nodes.end());
                curr_block->nodes.resize(insert_index);
                Block<T>* new_block_right = new Block<T>(right_block_nodes, curr_block->next);
                curr_block->next = new_block_right;
                parent->down = new_block_right;
                parent = new_node;
            }
        }
    }
}

// Remove a key-value pair
template<typename T>
void BSkipList<T>::remove(int key){

    // Block<T>* greatest_min_block = levels[levels.size() - 1];
    // // greatest_min_block->print();
    // bool found = false;
    // int found_index = -1;

    // while ( !found && greatest_min_block != nullptr ) {
    //     int size = greatest_min_block->nodes.size();
    //     Block<T>* curr_block = greatest_min_block;
    //     int index = 0;
    //     for ( int i = 0; i<size; i++ ) {
    //         if ( key > curr_block->nodes[i]->get_key() ) {
    //             index = i;
    //         }
    //         else {
    //             break;
    //         }
    //     }

    //     // curr_block->print();
    //     if ( index < size - 1 && key == curr_block->nodes[index + 1]->get_key() ) {
    //         found = true;
    //         found_index = index + 1;
    //         break;
    //     }
    //     else {
    //         greatest_min_block = curr_block->nodes[index]->down;
    //     }
    // }

    // if ( found_index == -1 ) {
    //     cout << "Key not found" << endl;
    //     return;
    // }
    // else {
    //     // greatest_min_block->print();
    // }

    pair<Block<T>*, int> search_info = this->search(key);
    if ( search_info.first == nullptr )
        return;
    
    Block<T>* greatest_min_block = search_info.first;
    int found_index = search_info.second;

    // Block<T>* next_level_prev = nullptr;
    Block<T>* curr_block = greatest_min_block;
    Block<T>* next_level_prev = curr_block->nodes[found_index - 1]->down;
    Block<T>* next_level_self = curr_block->nodes[found_index]->down;

    curr_block->nodes.erase(curr_block->nodes.begin() + found_index);

    curr_block = next_level_self;

    while ( curr_block != nullptr ) {
        next_level_self = curr_block->nodes[0]->down;
        Block<T>* curr_level_prev = next_level_prev;
        next_level_prev = curr_level_prev->nodes.back()->down;
        curr_block->nodes.erase(curr_block->nodes.begin());
        curr_level_prev->nodes.insert(curr_level_prev->nodes.end(), curr_block->nodes.begin(), curr_block->nodes.end());
        curr_level_prev->next = curr_level_prev->next->next;
        // Delete current block
        delete curr_block;
        curr_block = next_level_self;
        // curr_level_prev = next_level_prev;
    }
}

// Search for a key
template<typename T>
pair<Block<T>*, int> BSkipList<T>::search(int key){
    
    Block<T>* greatest_min_block = levels[levels.size() - 1];
    // greatest_min_block->print();
    bool found = false;
    int found_index = -1;

    while ( !found && greatest_min_block != nullptr ) {
        int size = greatest_min_block->nodes.size();
        Block<T>* curr_block = greatest_min_block;
        int index = 0;
        for ( int i = 0; i<size; i++ ) {
            if ( key > curr_block->nodes[i]->get_key() ) {
                index = i;
            }
            else {
                break;
            }
        }

        // curr_block->print();
        if ( index < size - 1 && key == curr_block->nodes[index + 1]->get_key() ) {
            found = true;
            found_index = index + 1;
            break;
        }
        else {
            greatest_min_block = curr_block->nodes[index]->down;
        }
    }

    if ( found_index == -1 ) {
        cout << "Key not found" << endl;
        return {nullptr, -1};
    }
    return {greatest_min_block, found_index}; 
}

// range query
template<typename T>
std::vector<Block<T>*> BSkipList<T>::range_query(int key1, int key2){
    // Range query implementation
    return this->levels;
}

// print
template<typename T>
void BSkipList<T>::print(){
    // print levels info
    int count = 9;
    for (auto it = levels.rbegin(); it != levels.rend(); ++it) {
        cout << "Level -- " << count-- << ": ";
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
    // b_skip_list.print();
    // b_skip_list.insert(1, 1);
    // b_skip_list.insert(2, 2);
    // return 0;
    int a[1000000];
    for(int i=0;i<1000000;i++){
        a[i] = i;
    }
    random_shuffle(a, a + 1000000);
    for(int i = 0; i < 1000000; i++){
        // b_skip_list.print();
        b_skip_list.insert(a[i], a[i]);
        // b_skip_list.print();
        // printf("Number %d inserted\n", a[i]);
    }
    // vector<Block<int>*> ls = b_skip_list.range_query(0, 0);
    // ls[0]->next->print();
    // cout <<endl;
    // ls[4]->print();
    // cout <<endl;
    // b_skip_list.print();
    // b_skip_list.remove(7);
    // b_skip_list.print();
    // b_skip_list.remove(60);
    // b_skip_list.print();
    random_shuffle(a, a + 1000000);
    for(int i = 0; i < 1000000; i++){
        // b_skip_list.print();
        b_skip_list.remove(a[i]);
        // printf("Number %d inserted\n", a[i]);
    }
    b_skip_list.print();
}