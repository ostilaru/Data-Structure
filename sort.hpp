/*
 * @Author: woodwood
 * @Date: 2023-10-16 10:02:48
 * @LastEditors: woodwood
 * @LastEditTime: 2023-10-19 16:32:55
 * @FilePath: \Data-Structure\sort.hpp
 * @FileName: sort.hpp
 * @Description: Description
 */

#ifndef _SORT_H_
#define _SORT_H_

#include "iterator.hpp"
#include <functional>

namespace wood_STL {

/**
   Selection sort base on array
 **/
template <typename T>
void selection_sort(T arr[], int size) {
    using std::swap;
    if(size < 2)
        return;
    for(int i = 0; i < size; i++) {
        int min = i;
        for(int j = i + 1; j < size; j++) {
            if(arr[j] < arr[min]) {
                min = j;    
            }
        }
    }
    if(min != i) {
        swap(arr[i], arr[min]);
    }
}

/**
   Selection sort in STL style using the user defined comparator
   the iterator require at least be forward iterator because we must traverse the container more than once
 **/
template<typename ForwardIterator, typename Comp, typename = wood_STL::RequireForwardIterator<ForwardIterator>>
void selection_sort(ForwardIterator first, ForwardIterator last, Comp comp) {
    using std::swap;
    if(first == last)
        return;
    for(; first != last; first++) {
        auto min = first;
        auto cur = first;
        for(++cur; cur != last; ++cur) {
            if(comp(*cur, *min)) {
                min = cur;
            }
        }
        if(min != first) {
            swap(*min, *first);
        }
    }
}

/**
   Selection sort in STL style using the default comparator
   the iterator require at least be forward iterator because we must traverse the container more than once
 **/
template<typename ForwardIterator, typename = wood_STL::RequireForwardIterator<ForwardIterator>>
void selection_sort(ForwardIterator first, ForwardIterator last) {
    using value_type = typename std::iterator_traits<ForwardIterator>::value_type;
    selection_sort(first, last, std::less<value_type>());
}

/**
   Bubble sort base on array
 **/
template<typename T>
void bubble_sort(T arr[], int size) {
    using std::swap;
    if(size < 2)
        return;
    for(int i = 0; i < size; i++) {
        for(int j = 1; j < size - i; j++) {
            if(arr[j] < arr[j - 1]) {
                swap(arr[j], arr[j - 1]);
            }
        }
    }
}

/**
   Bubble sort in STL style using the user defined comparator
   the iterator require at least be forward iterator because we must traverse the container more than once
 **/
template<typename ForwardIterator, typename Comp, typename = wood_STL::RequireForwardIterator<ForwardIterator>>
void bubble_sort(ForwardIterator first, ForwardIterator last, Comp comp) {
    using std::swap;
    if(first == last)
        return;
    while(last != first) {
        auto curr = first;
        auto next = first;
        for(++next; next != last; ++next) {
            if(comp(*next, *curr)) {
                swap(*next, *curr);
            }
            curr = next;
        }
        last = curr;
    }
}

/**
   Bubble sort in STL style using the default comparator
   the iterator require at least be forward iterator because we must traverse the container more than once
 **/
template<typename ForwardIterator, typename = wood_STL::RequireForwardIterator<ForwardIterator>>
void bubble_sort(ForwardIterator first, ForwardIterator last) {
    using value_type = typename wood_STL::iterator_traits<ForwardIterator>::value_type;
    bubble_sort(first, last, std::less<value_type>());
}

/**
   Insertion sort base on array
 **/
template<typename T>
void insertion_sort(T arr[], int size) {
    if(size < 2)
        return;
    for(int i = 1; i < size; ++i) {
        T value = std::move(arr[i]);
        int j = i;
        for(; j > 0 && value < arr[j - 1]; --j) {
            arr[j] = std::move(arr[j - 1]);
        }
        arr[j] = std::move(value);
    }
}

template <typename BidrectionalIterator, typename Comp, typename = wood_STL::RequireBidirectionalIterator<BidrectionalIterator>>




};  // namespace wood_STL


#endif /* _SORT_H_ */