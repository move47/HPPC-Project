#include<bits/stdc++.h>
using namespace std;

// Define a generic class using a template
template <typename T>
class KV{
    private:
        int key;
        T value;
    public:
        KV();
        KV(int key, T value);
        ~KV();
        int get_key();
        T get_value();
};