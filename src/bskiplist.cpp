#include <iostream>
#include <vector>
#include <stack>
#include <random>
#include <limits.h>
#include <time.h>
#include <chrono>
#include <numeric>   // For std::accumulate
#include <algorithm> // For std::min_element and std::max_element
using namespace std;
class Block;

class Node
{
public:
    int value;
    Block *down; // Pointer to lower level block contains same value
    Node(int value, Block *down)
    {
        this->value = value;
        this->down = down;
    }
};

class Block
{
public:
    std::vector<Node *> vector;
    Block *next; // Pointer to the next block at the same level
    Block(Node *node, Block *next)
    {
        vector.push_back(node);
        // vector.resize(3); // minimum size of each block
        this->next = next;
    }

    Block(std::vector<Node *> vector, Block *next)
    {
        this->vector = vector;
        // vector.resize(3); // minimum size of each block
        this->next = next;
    }

    void print()
    {
        for (unsigned int i = 0; i < vector.size(); i++)
        {
            std::cout << vector[i]->value;
            if (vector[i]->down)
                std::cout << "(" << vector[i]->down->vector[0]->value << ")";
            std::cout << " ";
        }
        std::cout << "| ";
    }
};

class BSkipList
{
private:
    std::vector<Block *> levels; // Vector of head blocks from each level
    std::stack<Block *> getBlockStack(int value)
    {
        int lvl = levels.size() - 1;
        Block *current = levels[levels.size() - 1]; // starting from first block in higest level
        std::stack<Block *> blocks;                 // store the path
        Block *block = current;                     // keep track the place for value
        Node *prev;
        while (current)
        {
            bool found = false;
            // find a value greater than insert value
            for (unsigned int i = 0; i < current->vector.size(); i++)
            {
                if (value > current->vector[i]->value)
                { // go to next node
                    prev = current->vector[i];
                }
                else
                { // find the place
                    blocks.push((block));
                    current = prev->down;
                    lvl--;
                    block = current;
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                // keep looking in next block
                if (current->next)
                {
                    current = current->next;
                    // last in current block
                    if (value < current->vector[0]->value)
                    {
                        blocks.push(block);
                        current = prev->down;
                    }
                }
                else // last in this level
                    blocks.push(current);
                current = prev->down;
            }
            block = current;
        }
        return blocks;
    }

public:
    int r = 1;
    const float P_FACTOR = 0.25;
    // const int MAX_LEVEL = 32;
    // const float P_FACTOR = 0.25;
    // static std::random_device rd; // obtain a random number from hardware
    // static std::mt19937 gen(rand()); // seed the generator
    // static std::uniform_real_distribution<> distr(0, 1); // define the range
    BSkipList()
    {
        // Block *block = new Block(new Node(INT_MIN, nullptr), nullptr); // negative infinity block
        // levels.push_back(block);

        Node* level1_node1 = new Node(1, nullptr);
        Node* level1_node2 = new Node(10, nullptr);
        std::vector<Node*> level1;
        level1.push_back(level1_node1);
        level1.push_back(level1_node2);
        Block* level1Block = new Block(level1, nullptr);

        Node* level2_node1 = new Node(1, nullptr);
        Node* level2_node2 = new Node(3, nullptr);
        Node* level2_node3 = new Node(5, nullptr);
        std::vector<Node*> level2_1;
        level2_1.push_back(level2_node1);
        level2_1.push_back(level2_node2);
        level2_1.push_back(level2_node3);
        Block* levelBlock2_1 = new Block(level2_1, nullptr);
        level1_node1->down = levelBlock2_1;

        Node* level2_node4 = new Node(10, nullptr);
        Node* level2_node5 = new Node(12, nullptr);
        std::vector<Node*> level2_2;
        level2_2.push_back(level2_node4);
        level2_2.push_back(level2_node5);
        Block* levelBlock2_2 = new Block(level2_2, nullptr);
        level1_node2->down = levelBlock2_2;
        
        levelBlock2_1->next = levelBlock2_2;

        Node* level3_node1 = new Node(1, nullptr);
        Node* level3_node2 = new Node(2, nullptr);
        std::vector<Node*> level3_1;
        level3_1.push_back(level3_node1);
        level3_1.push_back(level3_node2);
        Block* levelBlock3_1 = new Block(level3_1, nullptr);
        level2_node1->down = levelBlock3_1;

        Node* level3_node3 = new Node(3, nullptr);
        Node* level3_node4 = new Node(4, nullptr);
        std::vector<Node*> level3_2;
        level3_2.push_back(level3_node3);
        level3_2.push_back(level3_node4);
        Block* levelBlock3_2 = new Block(level3_2, nullptr);
        level2_node2->down = levelBlock3_2;

        Node* level3_node5 = new Node(5, nullptr);
        Node* level3_node6 = new Node(6, nullptr);
        std::vector<Node*> level3_3;
        level3_3.push_back(level3_node5);
        level3_3.push_back(level3_node6);
        Block* levelBlock3_3 = new Block(level3_3, nullptr);
        level2_node3->down = levelBlock3_3;

        Node* level3_node7 = new Node(10, nullptr);
        Node* level3_node8 = new Node(11, nullptr);
        std::vector<Node*> level3_4;
        level3_4.push_back(level3_node7);
        level3_4.push_back(level3_node8);
        Block* levelBlock3_4 = new Block(level3_4, nullptr);
        level2_node4->down = levelBlock3_4;

        Node* level3_node9 = new Node(12, nullptr);
        Node* level3_node10 = new Node(13, nullptr);
        std::vector<Node*> level3_5;
        level3_5.push_back(level3_node9);
        level3_5.push_back(level3_node10);
        Block* levelBlock3_5 = new Block(level3_5, nullptr);
        level2_node5->down = levelBlock3_5;

        levelBlock3_1->next = levelBlock3_2;
        levelBlock3_2->next = levelBlock3_3;
        levelBlock3_3->next = levelBlock3_4;
        levelBlock3_4->next = levelBlock3_5;
        
        levels.push_back(levelBlock3_1);
        levels.push_back(levelBlock2_1);
        levels.push_back(level1Block);

    //     Node* level1_node1 = new Node(1, nullptr);
    //     Node* level2_node1 = new Node(1, nullptr);
    //     Node* node_2 = new Node(2, nullptr);
    //     Node* node_3 = new Node(3, nullptr);
    //     Node* node_4 = new Node(4, nullptr);
    //     Node* node_5 = new Node(5, nullptr);
    //     Node* node_6 = new Node(6, nullptr);
    //     Node* node_7 = new Node(7, nullptr);
    //     Node* node_8 = new Node(8, nullptr);
    //     Node* node_9 = new Node(9, nullptr);
    //     Node* node_10 = new Node(10, nullptr);
    //     Node* node_11 = new Node(11, nullptr);
    //     Node* node_12 = new Node(12, nullptr);
    //     Node* node_13 = new Node(13, nullptr);

    //     std::vector<Node*> level1;
    //     level1.push_back(node_1);
    //     level1.push_back(node_10);
    //     Block* level1Block = new Block(level1, nullptr);

    //     // Level 2
    //     std::vector<Node*> level2_1;
    //     level2_1.push_back(node_1);
    //     level2_1.push_back(node_3);
    //     level2_1.push_back(node_5);
    //     Block* levelBlock2_1 = new Block(level2_1, nullptr);

    //     node_1->down = levelBlock2_1;
    //     node

    //     std::vector<Node*> level2_2;
    //     level2_1.push_back(node_10);
    //     level2_1.push_back(node_12);
    //     Block* levelBlock2_2 = new Block(level2_2, nullptr);
    //     levelBlock2_1->next = levelBlock2_2;

    //    // Level 3
    //     std::vector<Node*> level3_1;
    //     level3_1.push_back(node_1);
    //     level3_1.push_back(node_2);
    //     Block* levelBlock3_1 = new Block(level3_1, nullptr);

    //     std::vector<Node*> level3_2;
    //     level3_2.push_back(node_3);
    //     level3_2.push_back(node_4);
    //     Block* levelBlock3_2 = new Block(level3_2, nullptr);

    //     std::vector<Node*> level3_3;
    //     level3_3.push_back(node_5);
    //     level3_3.push_back(node_6);
    //     Block* levelBlock3_3 = new Block(level3_3, nullptr);

    //     std::vector<Node*> level3_4;
    //     level3_4.push_back(node_10);
    //     level3_4.push_back(node_11);
    //     Block* levelBlock3_4 = new Block(level3_4, nullptr);

    //     std::vector<Node*> level3_5;
    //     level3_5.push_back(node_12);
    //     level3_5.push_back(node_13);
    //     Block* levelBlock3_5 = new Block(level3_5, nullptr);

    //     levelBlock3_1->next = levelBlock3_2;
    //     levelBlock3_2->next = levelBlock3_3;
    //     levelBlock3_3->next = levelBlock3_4;
    //     levelBlock3_4->next = levelBlock3_5;
    }

    ~BSkipList()
    {
        // Destructor to free memory
        // ... (cleanup logic here)
    }

    void insert(int value)
    {
        srand(time(NULL)); // initialize random seed
        std::stack<Block *> blocks = getBlockStack(value);
        Block *lower = nullptr;
        // building block from botton
        while (!blocks.empty())
        {
            bool inserted = false;
            Block *block = blocks.top();
            blocks.pop();
            for (unsigned int i = 0; i < block->vector.size(); i++)
            {
                if (block->vector[i]->value > value)
                { // in the middle of the vector
                    // (static_cast<float>(rand()) / RAND_MAX) < P_FACTOR)
                    if (rand() % 2 != 0)
                    { // tail
                        r = r + rand();
                        block->vector.insert(block->vector.begin() + i, new Node(value, lower));
                        return;
                    }
                    else
                    { // head
                        r++;
                        // split and shrink block
                        std::vector<Node *> right;
                        right.push_back(new Node(value, lower));
                        for (unsigned int j = i; j < block->vector.size(); j++)
                            right.push_back(block->vector[j]);
                        block->vector.resize(i);
                        Block *rightBlock = new Block(right, block->next);
                        block->next = rightBlock;
                        // new level
                        if (blocks.empty())
                        {
                            Block *up = new Block(new Node(INT_MIN, block), nullptr);
                            up->vector.push_back(new Node(value, block->next));
                            levels.push_back(up);
                        }
                        inserted = true;
                        lower = block->next;
                        break;
                    }
                }
            }
            if (!inserted)
            {
                // at the end of the vector
                if (rand() % 2 != 0)
                { // tail
                    r = r + 1;
                    block->vector.push_back(new Node(value, lower));
                    return;
                }
                else
                { // head
                    r = r + rand();
                    Block *newBlock = new Block(new Node(value, lower), block->next);
                    block->next = newBlock;
                    // new level
                    if (blocks.empty())
                    {
                        Block *up = new Block(new Node(INT_MIN, block), nullptr);
                        up->vector.push_back(new Node(value, newBlock));
                        levels.push_back(up);
                    }
                    lower = newBlock;
                }
            }
        }
    }

    void remove(int value)
    {
        std::stack<Block *> blocks = getBlockStack(value);
        Block *current;
        Block *block;
        vector<Block *> update;
        Block *curr = nullptr;
        bool flag = false;
        for (int i = levels.size() - 1; i >= 0; i--)
        {
            Block *pre = nullptr;
            curr = levels[i];
            while (curr)
            {
                for (int j = 0; j < curr->vector.size(); j++)
                {
                    if (curr->vector[j]->value == value)
                    {
                        if (pre)
                        {
                            flag = true;
                            update.push_back(pre);
                            // cout << pre->vector[0]->value << "pre" << endl;
                        }
                        break;
                    }
                }
                if (flag)
                {
                    flag = false;
                    break;
                }

                pre = curr;
                curr = curr->next;
            }
        }

        // for (int i = 0; i < update.size(); i++)
        // {
        //     cout << update[i]->vector[0]->value << "update" << endl;
        //     if (update[i]->next)
        //     {
        //         cout << update[i]->next->vector[0]->value << "update next" << endl;
        //         if(update[i]->next->vector.size() > 1){
        //             cout << update[i]->next->vector[1]->value << "test" << endl;
        //         }
        //     }
        // }

        // Print the blocks in the stack
        Block *block1;
        while (!blocks.empty())
        {
            block1 = blocks.top();
            blocks.pop();
            cout << block1->vector[0]->value << " ";
        }
        blocks = getBlockStack(value);

        // Print the update vector in full content
        for (int i = 0; i < update.size(); i++)
        {
            cout << update[i]->vector[0]->value << " ";
        }

        int x = 0;
        while (!blocks.empty())
        {
            block = blocks.top();
            blocks.pop();

            for (unsigned int i = 0; i < block->vector.size(); i++)
            {
                if (block->vector[i]->value == value)
                {
                    Block *downBlock = block->vector[i]->down;
                    block->vector.erase(block->vector.begin() + i);

                    while (downBlock != nullptr)
                    {
                        current = downBlock->vector[0]->down;
                        downBlock->vector.erase(downBlock->vector.begin());
                        if (!downBlock->vector.empty())
                        {
                            update[x]->vector.insert(update[x]->vector.end(), downBlock->vector.begin(), downBlock->vector.end());
                            update[x]->next = update[x]->next->next;
                            x++;
                            // Print the update[x] block full content
                            // for (int i = 0; i < update[x]->vector.size(); i++)
                            // {
                            //     cout << update[x]->vector[i]->value << " ";
                            // }                            
                        }
                        else
                        {
                            update[x]->next = update[x]->next->next;
                            x++;
                        }

                        downBlock = current;
                    }
                }
            }
        }
    }

    // void remove(int value)
    // {
    //     std::stack<Block *> blocks = getBlockStack(value);
    //     Block *current;
    //     Block *block;
    //     vector<Block *> update;

    //     // Traverse the levels to find the blocks containing the value
    //     for (int i = levels.size() - 1; i >= 0; i--)
    //     {
    //         Block *pre = nullptr;
    //         current = levels[i];
    //         while (current)
    //         {
    //             for (int j = 0; j < current->vector.size(); j++)
    //             {
    //                 if (current->vector[j]->value == value)
    //                 {
    //                     if (pre)
    //                     {
    //                         update.push_back(pre);
    //                     }
    //                     break;
    //                 }
    //             }
    //             pre = current;
    //             current = current->next;
    //         }
    //     }

    //     // Iterate over the blocks in the stack
    //     while (!blocks.empty())
    //     {
    //         block = blocks.top();
    //         blocks.pop();

    //         // Iterate over the nodes in the block
    //         for (unsigned int i = 0; i < block->vector.size(); i++)
    //         {
    //             // If the node contains the value to remove
    //             if (block->vector[i]->value == value)
    //             {
    //                 // Erase the node from the block
    //                 block->vector.erase(block->vector.begin() + i);
    //             }
    //         }
    //     }

    //     // Iterate over the update vector and adjust the structure
    //     for (int i = 0; i < update.size(); i++)
    //     {
    //         // Iterate over the blocks in the update vector
    //         block = update[i];

    //         // Find the block containing the value
    //         current = block->next;
    //         while (current)
    //         {
    //             // Check if the current block contains the value
    //             for (unsigned int j = 0; j < current->vector.size(); j++)
    //             {
    //                 if (current->vector[j]->value == value)
    //                 {
    //                     // Erase the node from the current block
    //                     current->vector.erase(current->vector.begin() + j);

    //                     // If the current block becomes empty, remove it
    //                     if (current->vector.empty())
    //                     {
    //                         block->next = current->next;
    //                         delete current;
    //                         current = block->next;
    //                     }
    //                     break;
    //                 }
    //             }
    //             if (current)
    //             {
    //                 current = current->next;
    //             }
    //         }
    //     }
    // }

    void print_list()
    {
        Block *curr;
        for (int i = levels.size() - 1; i >= 0; i--)
        {
            Block *pre = nullptr;
            curr = levels[i];
            while (curr)
            {
                for (int j = 0; j < curr->vector.size(); j++)
                {

                    cout << curr->vector[j]->value << " ";
                    if (curr->vector[j]->down)
                    {
                        cout << "(" << curr->vector[j]->down->vector[0]->value << ")";
                    }
                }
                curr = curr->next;
                cout << "|";
            }
            cout << " " << endl;
        }
    }

    std::vector<int> getAverageSize()
    {
        Block *curr;
        vector<int> sizes;
        for (int i = levels.size() - 1; i >= 0; i--)
        {
            Block *pre = nullptr;
            curr = levels[i];
            while (curr)
            {
                sizes.push_back(curr->vector.size());
                curr = curr->next;
            }
        }
        return sizes;
    }

    bool search(int key)
    {
        std::vector<Node *>::iterator it;
        Node *node;
        Node *prev_node;
        Block *block = levels[levels.size() - 1];

        while (block)
        {
            for (it = block->vector.begin(); it != block->vector.end(); ++it)
            {
                node = *it;
                if (node->value < key)
                {
                    prev_node = node;
                    if (node == *std::prev(block->vector.end()))
                    {
                        block = block->next;
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                else if (node->value == key)
                {
                    return true;
                }
                else if (key < node->value)
                {
                    block = prev_node->down;
                    break;
                }
                // else if (i == 0) {return false;}
            }
        }
        // }
        return false;
    }

    std::vector<bool> range_query(int start_key, int end_key)
    {
        std::vector<bool> output;
        for (int key = start_key; key < end_key; key++)
        {
            int value = search(key);
            if (value != -1)
            {
                output.push_back(value);
            }
        }
        return output;
    }

    int getHeight(){
        return levels.size();
    }
};

int main()
{
    BSkipList list;
    for (int i = 0; i < 20; ++i)
    {
        list.insert(i);
        // if(i%5000==0)
        // list.print_list();
    }
    list.print_list();
    auto start = std::chrono::high_resolution_clock::now();
    int a[20];
    for (int i=0;i<20;++i){
        a[i] = i;
    }
    std::random_shuffle(std::begin(a), std::end(a));
    for (int i = 0; i < 1; ++i)
    {
        cout << a[i] << endl;
        list.remove(a[i]);
        // if(i%5000==0)
        // {
        //     list.print_list();
        // }
    }

    list.print_list();

    for (int i = 1; i < 2; ++i)
    {
        cout << a[i] << endl;
        list.insert(a[i]);
        // if(i%5000==0)
        // {
        //     list.print_list();
        // }
    }

    // Stop timing
    auto end = std::chrono::high_resolution_clock::now();
    // Calculate duration
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken to add " << 1000000 << " elements: "
              << duration.count() << " seconds." << std::endl;

    // vector<int> vec = list.getAverageSize();

    // double average = std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
    // int maxElement = *std::max_element(vec.begin(), vec.end());
    // int minElement = *std::min_element(vec.begin(), vec.end());

    // std::cout << "Average: " << average << std::endl;
    // std::cout << "Max: " << maxElement << std::endl;
    // std::cout << "Min: " << minElement << std::endl;
    // std::cout << "Height: " << list.getHeight() << std::endl;
    return 0;
}
