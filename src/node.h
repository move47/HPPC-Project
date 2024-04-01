#include<bits/stdc++.h>
#include <atomic>
#include "KV.h"

template<typename T>
class Node{
    public:
        // Stores the key and value for the Node
        KV<T> kv;

        // Stores the reference of the next node until the top level for the node
        vector<Node*> next;

        // Lock to lock the node when modifing it
        mutex node_lock;

        // Atomic variable to be marked if this Node is being deleted
        atomic<bool> marked = {false};

        // Atomic variable to indicate the Node is completely linked to predecessors and successors
        atomic<bool> fully_linked = {false};

        // The Maximum level until which the node is available
        int top_level; 

        Node();
        Node(int key, T value, int level);
        ~Node();
        int get_key();
        T get_value();
        void lock();
        void unlock();
};