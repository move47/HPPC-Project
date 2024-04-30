#include "b_skip_list.h"
#include "block.cpp"
#include "node.cpp"
#include "kv.cpp"
// #include "kv.h"
#include <chrono>
#include<list>
#include<climits>
#include<algorithm>
#include<thread>
#include<vector>
#include<utility>


using namespace std;
int insertions = 0;
int deletions = 0;
// int a[3200000];
// int b[3200000];
// int removed_elements = 0;
// Default constructor
template<typename T>
BSkipList<T>::BSkipList(int max_levels){
    Block<T>* prev_block = nullptr;
    this->num_levels = max_levels;
    this->levels = vector<Block<T>*>(max_levels);

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
    int locks = 0;
    srand(key);

    int num_flips = 0;
    while (num_flips < this->num_levels && rand() % 2 == 0) {
        num_flips++;
    }

    Block<T>* greatest_min_block = levels[levels.size() - 1];
    if(num_flips == int(levels.size() - 1))
        greatest_min_block -> w_lock();
    else
        greatest_min_block -> r_lock();
    // locks++;
    bool first_insert = false;
    Node<T>* parent = nullptr;
    Block<T>* parent_block = nullptr;
    Block<T>* curr_block = nullptr;
    Block<T>* uncle_block = nullptr;

    for ( int l = levels.size() - 1; l >= 0; l-- ) {
        
        if ( l > num_flips ) {
            curr_block = greatest_min_block;

            int size = curr_block->nodes.size();
            for ( int i = 0; i < size; i++ ) {
                if ( key > curr_block->nodes[i]->get_key() ) {
                    greatest_min_block = curr_block->nodes[i]->down;
                }
                else {
                    break;
                }
            }
            if(parent_block != nullptr){
                parent_block -> r_unlock();
                // if(parent_block -> block_lock.try_lock()){
                //     // printf("Mutes was not locked \n");
                //     parent_block -> unlock();
                // }
                // else{
                //     parent_block -> unlock();
                //     locks--;
                // }
            }
            parent_block = curr_block;

            if(greatest_min_block != nullptr){
                if(l - 1 == num_flips){
                    greatest_min_block -> w_lock();
                    // locks++;
                }
                else{
                    greatest_min_block -> r_lock();
                    // locks++;
                }
                // greatest_min_block->lock();
                // locks++;
            }
            
            // printf("Current number of locks at level %d is %d\n", l, locks);
        }
        else {
            curr_block = greatest_min_block;

            int insert_index = curr_block->nodes.size();

            int size = curr_block->nodes.size();
            for ( int i = 0; i < size; i++ ) {
                if ( key < curr_block->nodes[i]->get_key() ) {
                    insert_index = i;
                    break;
                }
            }

            // Because of INT_MIN, insert_index will always be greater than or equal to 1

            if ( !first_insert ) {
                Node<T>* new_node = new Node<T>(key, value, nullptr);
                curr_block->nodes.insert(curr_block->nodes.begin() + insert_index, new_node);
                insertions++;
                parent = new_node;
                parent_block -> r_unlock();
                // if(parent_block -> block_lock.try_lock()){
                //     // printf("Mutes was not locked \n");
                //     parent_block -> unlock();
                // }
                // else{
                //     parent_block -> unlock();
                //     locks--;
                // }
                // locks--;
                parent_block = curr_block;
                first_insert = true;
            }
            else {
                Node<T>* new_node = new Node<T>(key, value, nullptr);
                curr_block->nodes.insert(curr_block->nodes.begin() + insert_index, new_node);
                vector<Node<T>*> right_block_nodes = vector<Node<T>*>(curr_block->nodes.begin() + insert_index, curr_block->nodes.end());
                curr_block->nodes.resize(insert_index);
                Block<T>* new_block_right = new Block<T>(right_block_nodes, curr_block->next);
                new_block_right->w_lock();
                locks++;
                curr_block->next = new_block_right;
                parent->down = new_block_right;
                parent_block -> w_unlock();
                // if(parent_block -> block_lock.try_lock()){
                //     // printf("Mutes was not locked \n");
                //     parent_block -> unlock();
                // }
                // else{
                //     parent_block -> unlock();
                //     locks--;
                // }

                if(uncle_block != nullptr){
                    uncle_block -> w_unlock();
                    // if(uncle_block -> block_lock.try_lock()){
                    //     // printf("Mutes was not locked \n");
                    //     uncle_block -> unlock();
                    // }
                    // else{
                    //     uncle_block -> unlock();
                    //     locks--;
                    // }
                }
                // locks--;
                parent_block = curr_block;
                uncle_block = new_block_right;
                parent = new_node;
            }
            if(curr_block->nodes[insert_index - 1]->down != nullptr){
                curr_block->nodes[insert_index - 1]->down->w_lock();
                locks++;
            }
            greatest_min_block = curr_block->nodes[insert_index - 1]->down;
        }
        // printf("Current number of locks are %d at level %d for insert %d\n", locks, l, key);
        // printf("Current number of locks %d\n", locks);
    }
    if(curr_block != nullptr){
        curr_block->w_unlock();
        locks--;
    }
    if(uncle_block != nullptr){
        uncle_block->w_unlock();
        locks--;
    }
    // printf("Current number of locks are %d for insert %d\n", locks, key);
}

// Remove a key-value pair
template<typename T>
void BSkipList<T>::remove(int key){

    pair<Block<T>*, int> search_info = this->search_erase(key);
    if ( search_info.first == nullptr )
        return;
    
    Block<T>* greatest_min_block = search_info.first;
    int found_index = search_info.second;

    Block<T>* curr_block = greatest_min_block;
    if(curr_block->nodes[found_index - 1]->down != nullptr){
        curr_block->nodes[found_index - 1]->down->w_lock();
    }
    if(curr_block->nodes[found_index]->down != nullptr){
        curr_block->nodes[found_index]->down->w_lock();
        // if(curr_block->nodes[found_index]->down->next != nullptr){
        //     curr_block->nodes[found_index]->down->next->r_lock();
        // }
    }
    
    Block<T>* next_level_prev = curr_block->nodes[found_index - 1]->down;
    Block<T>* next_level_self = curr_block->nodes[found_index]->down;

    curr_block->nodes.erase(curr_block->nodes.begin() + found_index);
    deletions++;
    curr_block -> w_unlock();
    curr_block = next_level_self;

    while ( curr_block != nullptr ) {
        if(curr_block->nodes[0]->down != nullptr){
            curr_block->nodes[0]->down->w_lock();
            // if(curr_block->nodes[0]->down-> next != nullptr){
            //     curr_block->nodes[0]->down->next->r_lock();
            // }
        }
        
        next_level_self = curr_block->nodes[0]->down;
        Block<T>* curr_level_prev = next_level_prev;
        if(curr_level_prev->nodes.back()->down != nullptr){
            curr_level_prev->nodes.back()->down->w_lock();
        }
        next_level_prev = curr_level_prev->nodes.back()->down;
        curr_block->nodes.erase(curr_block->nodes.begin());
        curr_level_prev->nodes.insert(curr_level_prev->nodes.end(), curr_block->nodes.begin(), curr_block->nodes.end());
        curr_level_prev->next = curr_level_prev->next->next;
        // if(curr_level_prev->next != nullptr){
        //     curr_level_prev->next->r_unlock();
        // }
        delete curr_block;
        curr_level_prev->w_unlock();
        curr_block = next_level_self;
    }

    // removed_elements++;
    // printf("Key %d removed\n", key);
}

// Search for a key
template<typename T>
pair<Block<T>*, int> BSkipList<T>::search(int key){
    levels[levels.size() - 1]->r_lock();
    Block<T>* greatest_min_block = levels[levels.size() - 1];
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

        if ( index < size - 1 && key == curr_block->nodes[index + 1]->get_key() ) {
            found = true;
            found_index = index + 1;
            break;
        }
        else {
            if(curr_block->nodes[index]->down != nullptr){
                curr_block->nodes[index]->down->r_lock();
            }
            greatest_min_block = curr_block->nodes[index]->down;
            curr_block->r_unlock();
        }
    }

    if ( found_index == -1 ) {
        // cout << "Key not found" << endl;
        return {nullptr, -1};
    }
    return {greatest_min_block, found_index}; 
}

// Search for erasing a key
template<typename T>
pair<Block<T>*, int> BSkipList<T>::search_erase(int key){
    levels[levels.size() - 1]->w_lock();
    Block<T>* greatest_min_block = levels[levels.size() - 1];
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

        if ( index < size - 1 && key == curr_block->nodes[index + 1]->get_key() ) {
            found = true;
            found_index = index + 1;
            break;
        }
        else {
            if(curr_block->nodes[index]->down != nullptr){
                curr_block->nodes[index]->down->w_lock();
            }
            greatest_min_block = curr_block->nodes[index]->down;
            curr_block->w_unlock();
        }
    }

    if ( found_index == -1 ) {
        // cout << "Key not found" << endl;
        return {nullptr, -1};
    }
    return {greatest_min_block, found_index}; 
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
            pair<Block<int>*, int> search_info = b_skip_list.search(load[i].second);
            if (search_info.first != nullptr)
            {
                search_info.first->r_unlock();
            }   
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
    cout << "Number of elements inserted " << insertions << endl;   
    cout << "Number of elements deleted " << deletions << endl;
    cout << "Number of elements left " << insertions - deletions << endl;
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


    // take the extra character argument and store into a variable
    char c = ' ';
    if(argc > 2)
        c = argv[2][0];
    else
        c = ' ';

    std::cout<< "Number of threads: " << num_threads << std::endl;
    if(c == 's')
    strong_scaling(num_threads);
    else if(c == 'w')
    weak_scaling(num_threads);
    else
    cout << "No scaling argument provided";
    
    return 0;
}