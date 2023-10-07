/*
Created by woodwood on 28/9/2023.

Stack: 
    1. based on vector as a lower container instead of a deque
    2. throw an exception when illegal operation happen

*/




#ifndef DATA_STRUCTURE_STACK_H
#define DATA_STRUCTURE_STACK_H

#endif //DATA_STRUCTURE_STACK_H

#include <stdlib.h>
#include <vector>
#include <exception>

template<typename T>
struct Stack
{
private:
    T *data;
    size_t top; // point to the next spare location
    size_t capacity;

public:
    void init(size_t capacity);
    void destroy();
    void push(const T &e);

};
