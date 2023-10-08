/*
Created by woodwood on 08/10/2023.
*/

#ifndef _MEMORY_H_
#define _MEMORY_H_  

#include <memory>


// In C++14 we can use std::make_unique(), but not in C++11
// When we use std::forward, we can save the type
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params) {
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

#endif

/*
This is an example on how to use our "make_unique":

struct MyStruct
{
    int value;

    MyStruct(int val) : value(val) {
        std::cout << "MyStruct constructed with value: " << value << std::endl;
    }

    ~MyStruct() {
        std::cout << "MyStruct destructed with value: " << value << std::endl;
    }
};

int main() {
    auto ptr = make_unique<MyStruct>(10);
    std::cout << "MyStruct value: " << ptr->value << std::endl;
    return 0;
}

*/


