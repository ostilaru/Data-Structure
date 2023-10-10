/*
 * @Author: woodwood
 * @Date: 2023-10-10 13:26:19
 * @LastEditors: woodwood
 * @LastEditTime: 2023-10-10 13:57:24
 * @FilePath: \Data-Structure\vector_test.cpp
 * @FileName: test.cpp
 * @Description: a test file
 */

#include <iostream>
#include "vector.hpp"
#include <string>

int main() {
    wood_STL::vector<int> myVector;

    //* 1. test for push_back
    myVector.push_back(5);
    myVector.push_back(4);
    myVector.push_back(3);
    myVector.push_back(2);
    myVector.push_back(1);
    myVector.print();
    std::cout << std::endl;

    //* 2. test for overload operator '='
    wood_STL::vector<int> myVector2;
    myVector2 = myVector;
    myVector2.print();
    std::cout << std::endl;

    //* 3. test for clear
    myVector2.clear();
    myVector2.print();
    std::cout << std::endl;

    //* 4. test for assign
    int arr[] = {1, 2, 3};
    myVector2.assign(arr, arr + 3);
    myVector2.print();
    std::cout << std::endl;

    myVector2.clear();
    myVector2.assign({1, 2, 3, 4});
    myVector2.print();
    std::cout << std::endl;

    myVector2.clear();
    myVector2.assign(10, 0);
    myVector2.print();
    std::cout << std::endl;

    // 5. 

}
