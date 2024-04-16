#!/bin/bash

for i in {1..5}
do
    echo "Iteration $i"
    power_of_two=$((2**i))
    echo "2^$i = $power_of_two"
    g++ b_skip_list.cpp -o b_skip_list -Wall -g -std=c++11 -pthread
    ./b_skip_list $power_of_two
done