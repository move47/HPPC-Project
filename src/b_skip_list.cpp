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

    // cout << "Removing key: " << key << endl;

    std::pair<Block<T>*, pair<int,int>> search_info = this->searchHelper(key);
    if (search_info.first == nullptr)
    {
        // cout << "Key not found" << endl;
        return;
    }
    

    Block<T>* greatest_min_block = search_info.first;
    int found_index = search_info.second.first;
    int current_level = search_info.second.second;

    // cout << "Key found at level" << current_level << endl;
    Block<T>* curr_block = greatest_min_block;
    // reading the next level stuff, so need to lock it also
    if ( current_level > 0 )
        mutexVec[current_level - 1].lock();
    Block<T>* next_level_prev = curr_block->nodes[found_index - 1]->down;
    Block<T>* next_level_self = curr_block->nodes[found_index]->down;

    curr_block->nodes.erase(curr_block->nodes.begin() + found_index);

    curr_block = next_level_self;
    bool first = false;
    while ( curr_block != nullptr ) {
        if(!first) {
            first=true;
        }
        else{
            if ( current_level > 0 )
                mutexVec[current_level - 1].lock();
        }
        next_level_self = curr_block->nodes[0]->down;
        Block<T>* curr_level_prev = next_level_prev;
        next_level_prev = curr_level_prev->nodes.back()->down;
        curr_block->nodes.erase(curr_block->nodes.begin());
        curr_level_prev->nodes.insert(curr_level_prev->nodes.end(), curr_block->nodes.begin(), curr_block->nodes.end());
        curr_level_prev->next = curr_level_prev->next->next;
        delete curr_block;
        curr_block = next_level_self;
        mutexVec[current_level].unlock();
        current_level--;
    }
    mutexVec[current_level].unlock();
}

// Search for a key
template<typename T>
pair<Block<T>*, pair<int,int>> BSkipList<T>::searchHelper(int key){
    
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
            if ( current_level > 0 )
                mutexVec[current_level-1].lock();
            greatest_min_block = curr_block->nodes[index]->down;
            
        }
        mutexVec[current_level].unlock();
        current_level--;
    }

    if ( found_index == -1 ) {
        // cout << "Key not found" << endl;
        if(current_level > 0)
            mutexVec[current_level].unlock();
        return {nullptr, {-1, -1}};
    }
    return {greatest_min_block, {found_index, current_level}}; 
}

template<typename T>
T BSkipList<T>::search(int key){
    std::pair<Block<T>*, pair<int,int>> search_info = this->searchHelper(key);
    if (search_info.first == nullptr)
    {
        // cout << "Key not found" << endl;
        return T();
    }
    mutexVec[search_info.second.second].unlock();
    return search_info.first->nodes[search_info.second.first]->get_value();
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

vector<pair<int,int>> load;
template<typename T>
void process_operation(BSkipList<T> &b_skip_list, int start, int end){
    for(int i = start; i < end; i++){
        if(load[i].first==0)
        {
            b_skip_list.insert(load[i].second, load[i].second);
        }
        else if(load[i].first==1)
        {
            b_skip_list.search(load[i].second);
        }
        else
        {
            b_skip_list.remove(load[i].second);
        
        }
    }
}

void generateRandomPairs(vector<pair<int,int>>&random_pairs, int n){
    for(int i = 0; i < n; i++){
        random_pairs.push_back({0, i});
        random_pairs.push_back({1, i});
        random_pairs.push_back({2, i});
    }
    // shuffle the pairs
    random_shuffle(random_pairs.begin(), random_pairs.end());
}

void strong_scaling(int num_threads){
    // keep the load constant

    BSkipList<int> b_skip_list(50);
    
    vector<thread> threads;
    
    
    int num_operations = 3200000;
    
    // clear load
    load.clear();
    generateRandomPairs(load, num_operations);

    int num_operations_per_thread = int(load.size()/num_threads);

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < num_threads; i++){
        threads.push_back(thread(process_operation<int>, ref(b_skip_list), i*num_operations_per_thread, (i+1)*num_operations_per_thread));
    }

    for(int i = 0; i < (int)threads.size(); i++){
        threads[i].join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    cout << "Time taken for strong scaling " << elapsed_seconds.count() << "s\n";
}

void weak_scaling(int num_threads)
{
    // keep increasing the load along with threads
    BSkipList<int> b_skip_list(50);
    vector<thread> threads;

    int num_operations = num_threads*100000;

    // clear load
    load.clear();
    generateRandomPairs(load, num_operations);

    int num_operations_per_thread = int(load.size()/num_threads);

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < num_threads; i++){
        threads.push_back(thread(process_operation<int>, ref(b_skip_list), i*num_operations_per_thread, (i+1)*num_operations_per_thread));
    }

    for(int i = 0; i < (int)threads.size(); i++){
        threads[i].join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    cout << "Time taken for weak scaling " << elapsed_seconds.count() << "s\n";
}

int main(int argc, char* argv[]){
    srand(time(0));
    int num_threads = stoi(argv[1]);

    std::cout<< "Number of threads: " << num_threads << std::endl;
    
    strong_scaling(num_threads);
    
    weak_scaling(num_threads);
    return 0;

}