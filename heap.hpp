/*
 * @Author: woodwood
 * @Date: 2023-10-13 17:33:08
 * @LastEditors: woodwood
 * @LastEditTime: 2023-10-16 10:00:12
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

template<typename RandomIterator, typename Distance, typename Comp>
void fixDown(RandomIterator beg, Distance size, Distance startIndex, Comp comp) {
    auto elem = std::move(*(beg + startIndex));
    auto nodeIndex = startIndex;
    auto rightChild = startIndex;

    while(rightChild * 2 + 2 < size) {
        rightChild = 2 * rightChild + 2;
        if(comp(*(beg + rightChild), *(beg + (rightChild - 1)))) {
            --rightChild;
        }
        if(!comp(elem, *(beg + rightChild))) {
            break;
        }

        *(beg + nodeIndex) = std::move(*(beg + rightChild));
        nodeIndex = rightChild;
    }

    // if only have one child
    if(rightChild * 2 + 2 == size) {
        rightChild = 2 * rightChild + 1;
        if(comp(elem, *(beg + rightChild))) {
            *(beg + nodeIndex) = std::move(*(beg + rightChild));
            nodeIndex = rightChild;
        }
    }
    *(beg + nodeIndex) = std::move(elem);
}

template<typename RandomIterator, typename Distance>
void fixDown(RandomIterator beg, Distance size, Distance startIndex) {
    wood_STL::fixDown(beg, size, startIndex, std::less<decltype(*beg)>());
}

template<typename RandomIterator, typename Comp>
void pop_heap(RandomIterator beg, RandomIterator end, Comp comp) {
    if(end - beg < 2)
        return;
    std::swap(*beg, *(--end));
    auto size = end - beg;
    decltype(size) index = 0;
    wood_STL::fixDown(beg, size, index, comp);
}

template<typename RandomIterator>
void pop_heap(RandomIterator beg, RandomIterator end) {
    wood_STL::pop_heap(beg, end, std::less<decltype(*beg)>());
}

template<typename RandomIterator, typename Comp>
void make_heap(RandomIterator beg, RandomIterator end, Comp comp) {
    if(end - beg < 2)
        return;
    const auto size = end - beg;
    const auto lastIndex = size - 1;
    for(auto i = (lastIndex - 1) / 2; i >= 0; --i) {
        wood_STL::fixDown(beg, size, i, comp);
    }
}

template<typename RandomIterator>
void make_heap(RandomIterator beg, RandomIterator end) {
    wood_STL::make_heap(beg, end, std::less<decltype(*beg)>());
}

// make sure the heap is a max_heap
template<typename RandomIterator, typename Comp>
void sort_heap(RandomIterator beg, RandomIterator end, Comp comp) {
    while(end - beg > 1) {
        wood_STL::pop_heap(beg, end--, comp);
    }
}

template<typename RandomIterator, typename Comp>
void heap_sort(RandomIterator beg, RandomIterator end, Comp comp) {
    wood_STL::make_heap(beg, end, comp);
    wood_STL::sort_heap(beg, end, comp);
}

template<typename RandomIterator>
void heap_sort(RandomIterator beg, RandomIterator end) {
    wood_STL::heap_sort(beg, end, std::less<decltype(*beg)>());
}

template<typename RandomIterator, typename BinaryPredicate>
RandomIterator is_heap_until(RandomIterator beg, RandomIterator end, BinaryPredicate predicate) {
    if(end - beg < 2)
        return end;
    auto size = end - beg;
    for(decltype(size) index = 1; index < size; ++index) {
        auto parentIndex = (index - 1) / 2;
        if(predicate(*(beg + parentIndex), *(beg + index))) {
            return beg + index;
        }
    }
    return end;
}

template<typename RandomIterator>
RandomIterator is_heap_until(RandomIterator beg, RandomIterator end) {
    return wood_STL::is_heap_until(beg, end, std::less<decltype(*beg)>{});
}

template<typename RandomIterator, typename BinaryPredicate>
bool is_heap(RandomIterator beg, RandomIterator end, BinaryPredicate predicate) {
    return wood_STL::is_heap_until(beg, end, predicate) == end;
}

template<typename RandomIterator>
bool is_heap(RandomIterator beg, RandomIterator end) {
    return wood_STL::is_heap(beg, end, std::less<decltype(*beg)>{});
}


};  // namespace wood_STL




#endif /* _HEAP_H_ */