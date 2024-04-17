# CS 6230 High Performance Parallel Computing Project

## Project Members:

- Atith
- Himanshu
- Sairoop

## How to run it:

```c++
cd src/

g++ b_skip_list.cpp -o b_skip_list -Wall -g -std=c++11

./b_skip_list <thread_num> [weak/strong scaling],

e.g. for running two threads: ./b_skip_list 2
```

## For running over multiple threads

```c++
cd src/

./run_multiple.sh
```
