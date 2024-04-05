#include "b_skip_list.h"
#include "block.cpp"
#include "node.cpp"
#include "kv.cpp"
// #include "kv.h"
#include<list>
#include<climits>
#include<algorithm>
#include<vector>

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
    greatest_min_block->print();
    bool first_insert = false;
    Node<T>* parent = nullptr;

    for ( int l = levels.size() - 1; l >= 0; l-- ) {
        if ( l > num_flips ) {
            // Keep track of the greatest min block node
            // Iterate over vector of the block greatesr_min_block
            // This is not required

            Block<T>* curr_block = greatest_min_block;

            for ( int i = 0; i < curr_block->nodes.size(); i++ ) {
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

            for ( int i = 0; i < curr_block->nodes.size(); i++ ) {
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
    int a[50];
    for(int i=0;i<50;i++){
        a[i] = i;
    }
    random_shuffle(a, a + 50);
    for(int i = 0; i < 50; i++){
        b_skip_list.print();
        b_skip_list.insert(a[i], a[i]);
        b_skip_list.print();
        printf("Number %d inserted\n", a[i]);
    }
}