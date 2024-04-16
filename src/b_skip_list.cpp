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
#include<thread>
using namespace std;

// Default constructor
template<typename T>
BSkipList<T>::BSkipList(int max_levels){
    Block<T>* prev_block = nullptr;
    this->num_levels = max_levels;
    this->levels = vector<Block<T>*>(max_levels);
    this->mutexVec = vector<std::mutex>(max_levels);

    for ( int i = max_levels - 1; i >= 0; i-- ) {
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

    srand(key);

    int num_flips = 0;
    while (num_flips < this->num_levels && rand() % 2 == 0) {
        num_flips++;
    }

    int max_level = levels.size() - 1;
    mutexVec[max_level].lock();
    Block<T>* greatest_min_block = levels[levels.size() - 1];
    bool first_insert = false;
    Node<T>* parent = nullptr;
    for ( int l = levels.size() - 1; l >= 0; l-- ) {
        if ( l > 0 )
            mutexVec[l-1].lock();
        if ( l > num_flips ) {
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
        }   
        else {
            Block<T>* curr_block = greatest_min_block;

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

            if ( !first_insert ) {
                Node<T>* new_node = new Node<T>(key, value, nullptr);
                curr_block->nodes.insert(curr_block->nodes.begin() + insert_index, new_node);
                parent = new_node;
                first_insert = true;
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
        if ( l < max_level )
            mutexVec[l + 1].unlock();
    }
    mutexVec[0].unlock();
}

// Remove a key-value pair
template<typename T>
void BSkipList<T>::remove(int key){

   std::pair<Block<T>*, pair<int,int>> search_info = this->search(key);
    if (search_info.first == nullptr)
        return;
    
    Block<T>* greatest_min_block = search_info.first;
    int found_index = search_info.second.first;
    int current_level = search_info.second.second;

    Block<T>* curr_block = greatest_min_block;
    // reading the next level stuff, so need to lock it also
    mutexVec[current_level - 1].lock();
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
        delete curr_block;
        curr_block = next_level_self;
    }
}

// Search for a key
template<typename T>
pair<Block<T>*, pair<int,int>> BSkipList<T>::search(int key){
    
    int current_level = levels.size() - 1;
    // lock this level
    mutexVec[current_level].lock();
    Block<T>* greatest_min_block = levels[current_level];
    bool found = false;
    int found_index = -1;
    while ( !found && greatest_min_block != nullptr ) {
        int size = greatest_min_block->nodes.size();
        Block<T>* curr_block = greatest_min_block;
        int index = 0;
        for (int i = 0; i<size; i++ ) {
            if ( key > curr_block->nodes[i]->get_key() ) {
                index = i;
            }
            else {
                break;
            }
        }

        if ( index < size - 1 && key == curr_block->nodes[index + 1]->get_key() ) {
            found = true;
            found_index = index + 1;
            break;
        }
        else {
            // lock the down level first 
            mutexVec[current_level-1].lock();
            greatest_min_block = curr_block->nodes[index]->down;
            
        }
        mutexVec[current_level].unlock();
        current_level--;
    }

    if ( found_index == -1 ) {
        cout << "Key not found" << endl;
        return {nullptr, -1, -1};
    }
    return {greatest_min_block, found_index, current_level}; 
}

// TODO : Implement range query
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
    int count = this->num_levels - 1;
    for (auto it = levels.rbegin(); it != levels.rend(); ++it) {
        cout << "Level -- " << count-- << ": ";
        Block<T>* curr_level = *it;
        while(curr_level != nullptr){
            curr_level->print();
            curr_level = curr_level->get_next();
        }
        std::cout << std::endl;
    }

}

// get the average size
template<typename T>
std::vector<int> BSkipList<T>::getAverageSize(){
    // Get average size implementation
    return vector<int>();
}
template<typename T>
void insertElements(BSkipList<T> &b_skip_list, int start, int end){
    for(int i = start; i < end; i++){
        //b_skip_list.print();
        b_skip_list.insert(i, i);
        //b_skip_list.print();
        //cout<<"inserting "<<i<<endl;
    }
}

int main(int argc, char* argv[]){
    srand(time(0));
    int num_threads = stoi(argv[1]);
    std::cout<< "Number of threads: " << num_threads << std::endl;
    
    BSkipList<int> b_skip_list(500);
    
    vector<thread> threads;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < num_threads; i++){
        threads.push_back(thread(insertElements<int>, ref(b_skip_list), 100000*i, (i+1)*100000));
    }

    for(int i = 0; i < num_threads; i++){
        threads[i].join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    
    cout << "Time taken to insert 3200000 elements: " << elapsed_seconds.count() << "s\n";

    // int a[1000000];
    // for(int i=0;i<1000000;i++){
    //     a[i] = i;
    // }
    // random_shuffle(a, a + 1000000);
    // for(int i = 0; i < 1000000; i++){
    //     b_skip_list.insert(a[i], a[i]);
    // }
    // random_shuffle(a, a + 1000000);
    // for(int i = 0; i < 1000000; i++){
    //     b_skip_list.remove(a[i]);
    // }
    // b_skip_list.print();
}