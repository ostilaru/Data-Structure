/*
 * @Author: woodwood
 * @Date: 2023-10-08 09:28:10
 * @LastEditors: woodwood
 * @LastEditTime: 2023-10-08 16:11:33
 * @FilePath: \Data-Structure\algorithm.hpp
 * @FileName: FileName
 * @Description: some functions to operate and manage our iterators
 */

#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <functional>
#include <iostream>
#include <algorithm>
#include <random>
#include <iterator>

namespace wood_STL {

    template<typename InputIterator>
    inline typename std::iterator_traits<InputIterator>::difference_type distance( InputIterator first, InputIterator last ) {
        return wood_STL::distance( first, last, typename std::iterator_traits<InputIterator>::iterator_category() );
    }

    template<typename RandomAccessIterator>
    inline typename std::iterator_traits<RandomAccessIterator>::difference_type distance( RandomAccessIterator first, RandomAccessIterator last, std::random_access_iterator_tag ) {
        return last - first;
    }

    template<typename InputIterator>
    inline typename std::iterator_traits<InputIterator>::difference_type distance( InputIterator first, InputIterator last, std::input_iterator_tag ) {
        typename std::iterator_traits<InputIterator>::difference_type n = 0;
        while(first != last) {
            ++first;
            ++n;
        }
        return n;
    }

    template<typename InputIterator, typename OutputIterator>
    inline OutputIterator copy( InputIterator beg, InputIterator end, OutputIterator desBeg ) {
        while(beg != end) {
            *desBeg++ = *beg++;
        }
        return desBeg;
    }

    template<typename InputIterator, typename T>
    InputIterator find( InputIterator beg, InputIterator end, const T& value ) {
        for(; beg != end; ++beg) {
            if(*beg == value) {
                return beg;
            }
        }
        return beg;
    }

    template<typename InputIterator, typename UnaryPredicate>
    InputIterator find_if( InputIterator beg, InputIterator end, UnaryPredicate pred ) {
        for(; beg != end && !predicate(*beg); ++beg){
            if(pred(*beg)) {
                break;
            }
        }
        return beg;
    }

    template<typename InputIterator, typename UnaryPredicate>
    InputIterator find_if_not( InputIterator beg, InputIterator end, UnaryPredicate pred ) {
        for(; beg != end && !pred(*beg); ++beg){
            if(!pred(*beg)) {
                break;
            }
        }
        return beg;
    }

    template<typename ForwardIterator, typename Comp>
    ForwardIterator min_element(ForwardIterator beg, ForwardIterator end, Comp comp) {
        if(beg == end) {
            return beg;
        }
        auto min = beg;
        ++beg;
        while(++beg != end) {
            if(comp(*beg, *min)) {
                min = beg;
            }
        }
        return min;
    }

    template<typename ForwardIterator>
    ForwardIterator min_element(ForwardIterator beg, ForwardIterator end) {
        return min_element(beg, end, std::less<*beg>());
    }

    template<typename ForwardIterator, typename Comp>
    ForwardIterator max_element(ForwardIterator beg, ForwardIterator end, Comp comp) {
        if(beg == end) {
            return beg;
        }
        auto max = beg;
        while(++beg != end) {
            if(comp(*max, *beg)) {
                max = beg;
            }
        }
        return max;
    }

    template<typename ForwardIterator>
    ForwardIterator max_element(ForwardIterator beg, ForwardIterator end) {
        return max_element(beg, end, std::less<decltype(*beg)>());
    }

    template<typename ForwardIterator, typename Comp>
    std::pair<ForwardIterator, ForwardIterator> minmax_element(ForwardIterator beg, ForwardIterator end, Comp comp) {
        if(beg == end) {
            return {beg, beg};
        }
        auto min = beg, max = beg;
        while(++beg != end) {
            if(comp(*beg, *min)) {
                min = beg;
            }
            if(comp(*max, *beg)) {
                max = beg;
            }
        }
        return {min, max};
    }

    template<typename ForwardIterator>
    std::pair<ForwardIterator, ForwardIterator> minmax_element(ForwardIterator beg, ForwardIterator end) {
        return minmax_element(beg, end, std::less<decltype(*beg)>());
    }

    template<typename InputIterator1, typename InputIterator2>
    bool equal(InputIterator1 beg1, InputIterator1 end1, InputIterator2 beg2) {
        while(beg1 != end1) {
            if(*beg1++ != *beg2++) {
                return false;
            }
        }
        return true;
    }

    template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    bool equal(InputIterator1 beg1, InputIterator1 end1, InputIterator2 beg2, BinaryPredicate predicate) {
        while(beg1 != end1) {
            if(!predicate(*beg1++, *beg2++)) {
                return false;
            }
        }
        return true;
    }

    /*
        there is an example on how to use BinaryPredicate:
        bool isEven(int a, int b) {
            return a % 2 == b % 2;
        }

        std::vector<int> numbers1 = {2, 4, 6, 8, 10};
        std::vector<int> numbers2 = {2, 4, 6, 8, 10};
        bool result = equal(numbers1.begin(), numbers1.end(), numbers2.begin(), isEven);
    */

    template<typename InputIterator, typename T>
    auto count(InputIterator beg, InputIterator end, const T& value) -> decltype(beg - beg) {
        auto num = beg - beg;
        while(beg != end) {
            if(*beg++ == value) {
                ++num;
            }
        }
        return num;
    }

    template<typename InputIterator, typename UnaryPredicate>
    auto count_if(InputIterator beg, InputIterator end, UnaryPredicate predicate) -> decltype(beg - beg) {
        auto num = beg - beg;
        while(beg != end) {
            if(predicate(*beg++)) {
                ++num;
            }
        }
        return num;
    }

    template<typename ForwardIterator1, typename ForwardIterator2>
    bool is_permutation(ForwardIterator1 beg1, ForwardIterator1 end1, ForwardIterator2 beg2) {
        for(; beg1 != end1; ++beg1, ++beg2) { 
            if(*beg1 != *beg2) {
                break;
            }
        }
        if(beg1 == end1){
            return true;
        }
        auto end2 = beg2;
        std::advance(end2, std::distance(beg1, end1));
        for(auto scan = beg1; scan != end1; ++scan){
            if(scan != wood_STL::find(beg2, end2, *scan)){
                continue;
            }
            auto matches = wood_STL::count(beg2, end2, *scan);
            if(matches == 0 || wood_STL::count(scan, end1, *scan) != matches) {
                return false;
            }
        }
        return true;
    }

    template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    bool is_permutation(ForwardIterator1 beg1, ForwardIterator1 end1, ForwardIterator2 beg2, BinaryPredicate predicate) {
        for( ; beg1 != end1; ++beg1, ++beg2) { 
            if(!bool(predicate(*beg1, *beg2))) {
                break;
            }
        }
        if(beg1 == end1){
            return true;
        }
        auto end2 = beg2;
        std::advance(end2, std::distance(beg1, end1));
        for(auto scan = beg1; scan != end1; ++scan) {
            using std::placeholders::_1;
            if(scan != wood_STL::find_if(beg1, scan, std::bind(predicate, _1, *scan))){
                continue;
            }
            auto matches = wood_STL::count_if(beg2, end2, std::bind(predicate, _1, *scan));
            if(matches == 0 || wood_STL::count_if(scan, end1, std::bind(predicate, _1, *scan)) != matches) {
                return false;
            }
        }
        return true;
    }

    
    /**
     * @description: find the first dismatched element
     * @return {a pair of the two dismatched elemtns}
     */    
    template<typename InputIterator1, typename InputIterator2>
    std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 beg1, InputIterator1 end1, InputIterator2 beg2) {
        for( ; beg1 != end1; ++beg1, ++beg2) { 
            if(*beg1 != *beg2) {
                break;
            }
        }
        return {beg1, beg2};
    }
    
    /**
     * @description: find the first dismatched element in a BinaryPredicate way
     * @return {std::pair<InputIterator1, InputIterator2>} 
     */    
    template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 beg1, InputIterator1 end1, InputIterator2 beg2, BinaryPredicate predicate) {
        for( ; beg1 != end1; ++beg1, ++beg2) {
            if(!predicate(*beg1, *beg2)) {
                break;
            }
        }
        return {beg1, beg2};
    }

    /**
     * @description: remove the specific elem in your designed range 
     * @return {ForwardIterator} return the newEnd pos of the new range
     */    
    template<typename ForwardIterator, typename T>
    ForwardIterator remove(ForwardIterator beg, ForwardIterator end, const T& elem) {
        auto pos = wood_STL::find(beg, end, elem);
        if(pos == end) {
            return end;
        }
        auto newEnd = pos;
        for(++pos; pos != end; ++pos) {
            if(*pos != elem){
                *newEnd++ = std::move(*pos);
            }
        }
        return newEnd;
    }

    /**
     * @description: remove the specific elem in your designed range in a BinaryPredicate way
     * @return {ForwardIterator} return the newEnd pos of the new range
     */  
    template<typename ForwardIterator, typename UnaryPredicate>
    ForwardIterator remove_if(ForwardIterator beg, ForwardIterator end, UnaryPredicate predicate) {
        auto pos = wood_STL::find_if(beg, end, predicate);
        if(pos == end) {
            return end;
        }
        auto newEnd = pos;
        for(++pos; pos != end; ++pos) {
            if(!predicate(*pos)) {
                // * std::move() can transfer an object as an rvalue reference
                // * lvalue: identifiable, named and persistend, can appear on both sides
                // * rvalue: temporary, unnamed and about to be destroyed, only right sides
                // * it informs the complier it no longer need the origin object
                *newEnd++ = std::move(*pos);
            }
        }
        return newEnd;
    }

    /**
     * @description: copy the origin range Iterator exclude from the specific elem
     * @return {OutputIterator} a new Iterator which doesn't include the designed elem
     */    
    template<typename InputIterator, typename OutputIterator, typename T>
    OutputIterator remove_copy(InputIterator srcBeg, InputIterator srcEnd, OutputIterator desBeg, const T& elem) {
        while(srcBeg != srcEnd){
            auto copy = *srcBeg++;
            if(copy != elem) {
                *desBeg++ = copy;
            }
        }
        return desBeg;
    }

    /**
     * @description: copy the origin range Iterator exclude from the specific elem in predicate way
     * @return {OutputIterator} a new Iterator which doesn't include the designed elem
     */   
    template <typename InputIterator, typename OutputIterator, typename UnaryPredicate>
    OutputIterator remove_copy_if( InputIterator srcBeg, InputIterator srcEnd, OutputIterator desBeg, UnaryPredicate predicate ) 
    {
        while( srcBeg != srcEnd ) 
        {
            auto copy = *srcBeg++;
            if( !predicate( copy ) ) 
            {
                *desBeg++ = copy;
            } 
        }
        return desBeg;
    }

    /**
     * @description: find the first elem that satisfy the BinaryPredicate
     * @return {ForwardIterator} 
     */    
    template<typename ForwardIterator, typename BinaryPredicate>
    ForwardIterator adjacent_find(ForwardIterator beg, ForwardIterator end, BinaryPredicate prediacate) {
        if(beg == end) {
            return end;
        }
        auto previous = beg;
        while(++beg != end) {
            if(prediacate(*previous, *beg)) {
                return previous;
            }
            ++previous;
        }
        return end;
    }

    /**
     * @description: overload adjacent_find use std::equal_to
     * @return {*}
     */    
    template<typename ForwardIterator>
    ForwardIterator adjacent_find(ForwardIterator beg, ForwardIterator end) {
        return adjacent_find(beg, end, std::equal_to<decltype(*beg)>{});
    }

    /**
     * @description: remove adjacent duplicates from a range of elems based on a given predicate
     * @return {ForwardIterator} a new Iterator which only includes unique elem 
     */    
    template<typename ForwardIterator, typename BinaryPredicate>
    ForwardIterator unique(ForwardIterator beg, ForwardIterator end, BinaryPredicate predicate) {
        beg = wood_STL::adjacent_find(beg, end, predicate);
        if(beg == end) {
            return end;
        }
        auto newEnd = beg++;
        while(++beg != end) {
            if(!predicate(*newEnd, *beg)) {
                *(++newEnd) = std::move(*beg);
            }
        }
        return ++newEnd;
    }

    /**
     * @description: overload unique use std::equal_to
     * @return {ForwardIterator}
     */    
    template <typename ForwardIterator>
    ForwardIterator unique( ForwardIterator beg, ForwardIterator end ) {
        return mystl::unique( beg, end, std::equal_to<decltype(*beg)>() );
    }

    /**
     * @description: make an unique copy the origin Iterator
     * @return {OutputIterator} return a new copied Iterator
     */    
    template <typename InputIterator, typename OutputIterator, typename BinaryPredicate>
    OutputIterator unique_copy(InputIterator srcBeg, InputIterator srcEnd, OutputIterator desBeg, BinaryPredicate predicate) {
        if(srcBeg == srcEnd) {
            return desBeg;
        }
        auto lastOutputElem = *srcBeg++;
        *desBeg++ = lastOutputElem;
        while(srcBeg != srcEnd) {
            auto currentElem = *srcBeg++;
            if(!predicate(lastOutputElem, currentElem)) {
                *desBeg++ = currentElem;
                lastOutputElem = std::move(currentElem);
            }
        }
        return desBeg;
    }

    /**
     * @description: overload unique_copy use std::equal_to
     * @return {OutputIterator}
     */    
    template <typename InputIterator, typename OutputIterator>
    OutputIterator unique_copy( InputIterator srcBeg, InputIterator srcEnd, OutputIterator desBeg ) {
        return mystl::unique_copy( srcBeg, srcEnd, desBeg, std::equal_to<decltype(*srcBeg)>{} );
    }

    


};  // namespace wood_STL


#endif /* _ALGORITHM_H_ */