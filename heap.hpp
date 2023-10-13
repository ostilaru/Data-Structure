/*
 * @Author: woodwood
 * @Date: 2023-10-13 17:33:08
 * @LastEditors: woodwood
 * @LastEditTime: 2023-10-13 17:57:33
 * @FilePath: \Data-Structure\heap.hpp
 * @FileName: heap.hpp
 * @Description: Description
 */

#ifndef _HEAP_H_
#define _HEAP_H_

namespace wood_STL {

template<typename RandomIterator, typename Comp>
void push_heap(RandomIterator beg, RandomIterator end, Comp comp) {
    if(end - beg < 2) {
        return;
    }

    auto elem = std::move(*(end - 1));
    auto elemIndex = end - beg - 1;
    const decltype(elemIndex) rootIndex = 0;
    auto parentIndex = (elemIndex - 1) / 2;

    while(elemIndex > rootIndex && comp(*(beg + parentIndex), elem)) {
        *(beg + elemIndex) = std::move(*(beg + parentIndex));
        elemIndex = parentIndex;
        parentIndex = (elemIndex - 1) / 2;
    }
    *(beg + elemIndex) = std::move(elem);
}

template<typename RandomIterator>
void push_heap(RandomIterator beg, RandomIterator end) {
    wood_STL::push_heap(beg, end, std::less<decltype(*beg)>());
}


};



#endif /* _HEAP_H_ */