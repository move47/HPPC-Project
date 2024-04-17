#!/bin/bash

g++ b_skip_list.cpp -o b_skip_list -Wall -g -std=c++11 -pthread
echo "Running strong scaling"
for i in {0..5}
do
    echo "Iteration $i"
    power_of_two=$((2**i))
    echo "2^$i = $power_of_two"
    ./b_skip_list $power_of_two s
done
echo "Running weak scaling"
for i in {0..5}
do
    echo "Iteration $i"
    power_of_two=$((2**i))
    echo "2^$i = $power_of_two"
    ./b_skip_list $power_of_two w
done