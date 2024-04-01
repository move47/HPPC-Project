// Define a generic class using a template
// the value can take any data type
#pragma once
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