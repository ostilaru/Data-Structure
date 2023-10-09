/*
 * @Author: woodwood
 * @Date: 2023-10-08 09:28:10
 * @LastEditors: woodwood
 * @LastEditTime: 2023-10-09 13:45:56
 * @FilePath: \Data-Structure\algorithm.hpp
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
        return wood_STL::unique( beg, end, std::equal_to<decltype(*beg)>() );
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
    
    /**
     * @description: Iterator2 is a substring, to match It2 and return the first pos in It1
     * @example: Iter1: abcabcabc   Iter2: abc  ==> return (pos = 0)
     * @return {ForwardIterator1} return the first pos in It1
     */    
    template<typename ForwardIterator1, typename ForwardIterator2>
    ForwardIterator1 search(ForwardIterator1 beg, ForwardIterator1 end, ForwardIterator2 searchBeg, ForwardIterator2 searchEnd) {
        if(searchBeg == searchEnd)
            return beg;
        auto start = beg;
        while((start = wood_STL::find(start, end, *searchBeg)) != end) {
            auto i = start;
            auto j = searchBeg;
            while(++j != searchEnd && ++i != end && *i == *j)
                ;
            if(j == searchEnd) {
                return start;
            }
            ++start;
        }
        return end;
    }

    /**
     * @description: Iterator2 is a substring, to match It2 in BinaryPredicate way
     * @return {ForwardIterator1} return the first pos int It1
     */   
    template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
    ForwardIterator1 search(ForwardIterator1 beg, ForwardIterator1 end, ForwardIterator2 searchBeg, ForwardIterator2 searchEnd, BinaryPredicate predicate) {
        using std::placeholders::_1;
        if(searchBeg == searchEnd)
            return beg;
        auto start = beg;
        while((start = wood_STL::find(start, end, std::bind(predicate, _1, *searchBeg))) != end) {
            auto i = start;
            auto j = searchBeg;
            while(++j != searchEnd && ++i != end && predicate(*i, *j))
                ;
            if(j == searchEnd) {
                return start;
            }
            ++start;
        }
        return end;
    }

    /**
     * @description: find the first occurence appear on both string
     * @return {InputIterator} return the fisrt pos of the same word
     */    
    template<typename InputIterator, typename ForwardIterator>
    InputIterator find_first_of(InputIterator beg, InputIterator end, ForwardIterator searchBeg, ForwardIterator searchEnd) {
        for(; beg != end; ++beg) {
            for(auto iter = searchBeg; iter != searchEnd; ++iter) {
                if(*iter == *beg){
                    return beg;
                }
            }
        }
        return end;
    }

    /**
     * @description: find the first occurence appear on both string in a BinaryPredicate way
     * @return {InputIterator} return the fisrt pos of the same word
     */  
    template <typename InputIterator, typename ForwardIterator, typename BinaryPredicate>
    InputIterator find_first_of( InputIterator beg, InputIterator end, ForwardIterator searchBeg, ForwardIterator searchEnd,
                                BinaryPredicate predicate ) 
    {
        for(; beg != end; ++beg) {
            for(auto iter = searchBeg; iter != searchEnd; ++iter) {
                if(predicate(*beg, *iter)) {
                    return beg;
                }
            }
        }
        return end;
    }

    /**
     * @description: Iter2 is a subsequence, find the last occurence of a subsequence
     * @example: Iter1: abcabcabc   Iter2: abc  ==> return (pos = 6)
     * @return {ForwardIterator1} return the last pos in Iter1
     */    
    template<typename ForwardIterator1, typename ForwardIterator2>
    ForwardIterator1 find_end(ForwardIterator1 beg, ForwardIterator1 end, ForwardIterator2 searchBeg, ForwardIterator2 searchEnd) {
        auto pos = wood_STL::search(beg, end, searchBeg, searchEnd);
        if(pos == end)
            return end;
        auto nextPos = pos;
        while((nextPos = wood_STL::search(nextPos, end, searchBeg, searchEnd)) != end) {
            pos = nextPos;
            ++nextPos;
        }
        return pos;
    }

    /**
     * @description: Iter2 is a subsequence, find the last occurence of a subsequence in a BinaryPredicate way
     * @return {ForwardIterator1} return the last pos in Iter1
     */    
    template <typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
    ForwardIterator1 find_end( ForwardIterator1 beg, ForwardIterator1 end, ForwardIterator2 searchBeg, ForwardIterator2 searchEnd,
                            BinaryPredicate predicate ) 
    {
        auto pos = wood_STL::search( beg, end, searchBeg, searchEnd, predicate );
        if( pos == end ) 
        {
            return end;
        } 
        auto nextPos = pos;
        while( ( nextPos = wood_STL::search( nextPos, end, searchBeg, searchEnd, predicate ) ) != end ) 
        {
            pos = nextPos;
            ++nextPos;
        }
        return pos;
    }

    /**
     * @description: binary_search in a sorted iterator
     * @return {bool} find return true, not return false
     */    
    template<typename ForwardIterator, typename T>
    bool binary_search(ForwardIterator beg, ForwardIterator end, const T &elem) {
        if(beg == end) return false;
        auto size = std::distance(beg, end);
        auto middle = beg;
        // * std::advance(iter, distance) => make a iterator move forward distance
        std::advance(middle, size / 2);
        
        if(elem < *middle) {
            return wood_STL::binary_search(beg, middle, elem);
        } else if(elem > *middle) {
            return wood_STL::binary_search(++middle, end, elem);
        } else{
            return true;
        }
    }

    template <typename ForwardIterator, typename T, typename Comp>
    bool binary_search(ForwardIterator beg, ForwardIterator end, const T &elem, Comp comp) {
        if(beg == end) return false;
        
        auto size = std::distance(beg, end);
        auto middle = beg;
        std::advance(middle, size / 2);
        
        if(comp(elem, *middle)) {
            return wood_STL::binary_search(beg, middle, elem, comp);
        } 
        else if(comp(*middle, elem)) {
            return wood_STL::binary_search(++middle, end, elem, comp);
        } 
        else {
            return true;
        }
    }

    /**
     * @description: checks if a sorted range [beg, end) includes another sorted range [searchBeg, searchEnd).
     * @example: iter1: abcabcabcd   iter2:abd  ==> true
     * @return {bool} true if include
     */    
    template<typename InputIterator1, typename InputIterator2>
    bool includes(InputIterator1 beg, InputIterator1 end, InputIterator2 searchBeg, InputIterator2 searchEnd) {
        if(searchBeg == searchEnd) return true;
        while((beg = wood_STL::find(beg, end, *searchBeg)) != end) {
            if(++searchBeg == searchEnd){
                return true;
            }
            ++beg;
        }
        return false;
    }

    template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
    bool includes(InputIterator1 beg, InputIterator1 end, InputIterator2 searchBeg, InputIterator2 searchEnd, BinaryPredicate predicate) {
        using std::placeholders::_1;
        if(searchBeg == searchEnd) return true;
        while((beg = wood_STL::find(beg, end, std::bind(predicate, _1, *searchBeg))) != end) {
            if(++searchBeg == searchEnd){
                return true;
            }
            ++beg;
        }
        return false;
    }

    /**
     * @description: make a sorted and nonrepetitive union of iter1 and iter2
     * @return {OutputIterator} return a new iter union
     */    
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Comp>
    OutputIterator set_union(InputIterator1 srcBeg1, InputIterator1 srcEnd1, InputIterator2 srcBeg2, InputIterator2 srcEnd2, OutputIterator desBeg, Comp comp) {
        while(srcBeg1 != srcEnd1 && srcBeg2 != srcEnd2) {
            if(comp(*srcBeg1, *srcBeg2)) {
                *desBeg++ = *srcBeg1++;
            } else if(comp(*srcBeg2, *srcBeg1)) {
                *desBeg++ = *srcBeg2++;
            } else {
                *desBeg++ = *srcBeg1++;
                ++srcBeg2;
            }
        }
        if(srcBeg1 != srcEnd1) {
            return wood_STL::copy(srcBeg1, srcEnd1, desBeg);
        } else {
            return wood_STL::copy(srcBeg2, srcEnd2, desBeg);
        }
    }

    /**
     * @description: overload -- make a sorted and nonrepetitive union of iter1 and iter2
     * @return {OutputIterator} return a new iter union
     */  
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator set_union(InputIterator1 srcBeg1, InputIterator1 srcEnd1, InputIterator2 srcBeg2, InputIterator2 srcEnd2, OutputIterator desBeg) {
        while(srcBeg1 != srcEnd1 && srcBeg2 != srcEnd2) {
            if(*srcBeg1 < *srcBeg2) {
                *desBeg++ = *srcBeg1++;
            } else if(*srcBeg2 < *srcBeg1) {
                *desBeg++ = *srcBeg2++;
            } else {
                *desBeg++ = *srcBeg1++;
                ++srcBeg2;
            }
        }
        if(srcBeg1 != srcEnd1) {
            return wood_STL::copy(srcBeg1, srcEnd1, desBeg);
        } else {
            return wood_STL::copy(srcBeg2, srcEnd2, desBeg);
        }
    }

    /**
     * @description: find the elems present in iter1 but not in iter2
     * @return {OutputIterator} return a new iter includes the specified elem in iter1
     */    
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Comp>
    OutputIterator set_difference(InputIterator1 srcBeg1, InputIterator1 srcEnd1, InputIterator2 srcBeg2, InputIterator2 srcEnd2, OutputIterator desBeg, Comp comp) {
        while(srcBeg1 != srcEnd1 && srcBeg2 != srcEnd2) {
            if(comp(*srcBeg1, *srcBeg2)) {
                *desBeg++ = *srcBeg1++;
            } else if(comp(*srcBeg2, *srcBeg1)) {
                ++srcBeg2;
            } else {
                ++srcBeg1;
                ++srcBeg2;
            }
        }
        return wood_STL::copy(srcBeg1, srcEnd1, desBeg);
    }

    /**
     * @description: overload -- find the elems present in iter1 but not in iter2
     * @return {OutputIterator} return a new iter includes the specified elem in iter1
     */
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator set_difference(InputIterator1 srcBeg1, InputIterator1 srcEnd1, InputIterator2 srcBeg2, InputIterator2 srcEnd2, OutputIterator desBeg) {
        while(srcBeg1 != srcEnd1 && srcBeg2 != srcEnd2) {
            if(*srcBeg1 < *srcBeg2) {
                *desBeg++ = *srcBeg1++;
            } else if(*srcBeg2 < *srcBeg1) {
                ++srcBeg2;
            } else {
                ++srcBeg1;
                ++srcBeg2;
            }
        }
        return wood_STL::copy(srcBeg1, srcEnd1, desBeg);
    }

    /**
     * @description: find the elems that are common to both iters
     * @return {OutputIterator} return a new iter includes the specified elems in both iters
     */    
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Comp>
    OutputIterator set_intersection(InputIterator1 srcBeg1, InputIterator1 srcEnd1, InputIterator2 srcBeg2, InputIterator2 srcEnd2, OutputIterator desBeg, Comp comp) {
        while(srcBeg1 != srcEnd1 && srcBeg2 != srcEnd2) {
            if(comp(*srcBeg1, *srcBeg2)) {
                ++srcBeg1;
            } else if(comp(*srcBeg2, *srcBeg1)) {
                ++srcBeg2;
            } else {
                *desBeg++ = *srcBeg1++;
                ++srcBeg2;
            }
        }
        return desBeg;
    }

    /**
     * @description: overload -- find the elems that are smaller in the two iters
     * @return {OutputIterator} return a new iter includes the smaller elems in both iters
     */    
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator set_intersection(InputIterator1 srcBeg1, InputIterator1 srcEnd1, InputIterator2 srcBeg2, InputIterator2 srcEnd2, OutputIterator desBeg) {
        while(srcBeg1 != srcEnd1 && srcBeg2 != srcEnd2) {
            if(*srcBeg1 < *srcBeg2) {
                ++srcBeg1;
            } else if(*srcBeg2 < *srcBeg1) {
                ++srcBeg2;
            } else {
                *desBeg++ = *srcBeg1++;
                ++srcBeg2;
            }
        }
        return desBeg;
    }

    /**
     * @description: find the elems satisfy comp and not in both iters' intersection
     * @return {OutputIterator} return a new iter includes the specific elem
     */    
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Comp>
    OutputIterator set_symmetric_difference(InputIterator1 srcBeg1, InputIterator1 srcEnd1, InputIterator2 srcBeg2, InputIterator2 srcEnd2, OutputIterator desBeg, Comp comp) {
        while(srcBeg1 != srcEnd1 && srcBeg2 != srcEnd2) {
            if(comp(*srcBeg1, *srcBeg2)) {
                *desBeg++ = *srcBeg1++;
            } else if(comp(*srcBeg2 < *srcBeg1)) {
                *desBeg++ = *srcBeg2++;
            } else {
                ++srcBeg1;
                ++srcBeg2;
            }
        }
        if(srcBeg1 != srcEnd1)
            return wood_STL::copy(srcBeg1, srcEnd1, desBeg);
        else
            return wood_STL::copy(srcBeg2, srcEnd2, desBeg);
    }

    /**
     * @description: overload -- find the elems satisfy comp and not in both iters' intersection
     * @return {OutputIterator} return a new iter includes the specific elem
     */    
    template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
    OutputIterator set_symmetric_difference(InputIterator1 srcBeg1, InputIterator1 srcEnd1, InputIterator2 srcBeg2, InputIterator2 srcEnd2, OutputIterator desBeg) {
        while(srcBeg1 != srcEnd1 && srcBeg2 != srcEnd2) {
            if(*srcBeg1 < *srcBeg2) {
                *desBeg++ = *srcBeg1++;
            } else if(*srcBeg2 < *srcBeg1) {
                *desBeg++ = *srcBeg2++;
            } else {
                ++srcBeg1;
                ++srcBeg2;
            }
        }
        if(srcBeg1 != srcEnd1)
            return wood_STL::copy(srcBeg1, srcEnd1, desBeg);
        else
            return wood_STL::copy(srcBeg2, srcEnd2, desBeg);
    }

    /**
     * @description: reverse the iter range
     * @return {void} return the origin iter
     */    
    template<typename BidirectionalIterator>
    void reverse(BidirectionalIterator beg, BidirectionalIterator end) {
        using std::swap;
        for(auto size = std::distance(beg, end); size >= 2; size -= 2) {
            swap(*beg++, *--end);
        }
    }

    /**
     * @description: reverse the iter range
     * @return {OutputIterator} return a new iter which is been reversed
     */ 
    template<typename BidirectionalIterator, typename OutputIterator>
    OutputIterator reverse_copy(BidirectionalIterator beg, BidirectionalIterator end, OutputIterator desBeg) {
        while(end != beg) {
            *desBeg++ = *--end;
        }
        return desBeg;
    }
    
    /**
     * @description: rotate the iter from newBeg
     * @example: rotate_copy(numbers.begin(), numbers.begin() + 2, numbers.end(), std::back_inserter(rotate));
     *          origin = {1, 2, 3, 4, 5}  --->   result = {3, 4, 5, 1, 2}
     * @return {OutputIterator} return a new rotated iter
     */    
    template<typename ForwardIterator, typename OutputIterator>
    OutputIterator rotate_copy(ForwardIterator beg, ForwardIterator newBeg, ForwardIterator end, OutputIterator desBeg) {
        if(beg == newBeg) return desBeg;
        auto current = newBeg;
        do {
            *desBeg++ = *current++;
            if(current == end) current = beg;
        } while(current != newBeg);
        return desBeg;
    }

    /**
     * @description: give a increasing permutation of the iter
     * @example: origin = {1, 2, 3} ==> result = {{1,2,3}, {1,3,2}, {2,1,3}, {2,3,1}, {3,1,2}, {3,2,1}}
     * @return {bool} return if the origin iter can be permutated or not
     */    
    template<typename BidirectionalIterator, typename Comp>
    bool next_permutation(BidirectionalIterator beg, BidirectionalIterator end, Comp comp) {
        using std::swap;
        if(beg == end) return false;
        auto current = beg;
        if(++current == end) return false;
        current = end;
        --current;
        while(current != beg) {
            auto next = current;
            --current;
            if(comp(*current, *next)) {
                auto iter = end;
                while(!comp(*current, *--iter))
                    ;
                swap(*current, *iter);
                wood_STL::reverse(next, end);
                return true;
            }
        }
        return false;
    }

    template <typename BidirectionalIterator>
    bool next_permutation( BidirectionalIterator beg, BidirectionalIterator end ) {
        return wood_STL::next_permutation( beg, end, std::less<decltype(*beg)>{} );
    }

    /**
     * @description: give a decreasing permutation of the iter
     * @example: origin = {1, 2, 3} ==> result = {{3,2,1}, {3,1,2}, {2,3,1}, {2,1,3}, {1,3,2}, {1,2,3}}
     * @return {bool} return if the origin iter can be permutated or not
     */    
    template<typename BidirectionalIterator, typename Comp>
    bool prev_permutation(BidirectionalIterator beg, BidirectionalIterator end, Comp comp) {
        using std::swap;
        if(beg == end) return false;
        auto current = beg;
        if(++current == end) return false;
        current = end;
        --current;
        while(current != beg) {
            auto next = current;
            --current;
            if(comp(*current, *next)) {
                auto iter = end;
                while(!comp(*current, *--iter))
                    ;
                swap(*current, *iter);
                wood_STL::reverse(next, end);
                return true;
            }
        }
        return false;
    }

    template <typename BidirectionalIterator>
    bool prev_permutation( BidirectionalIterator beg, BidirectionalIterator end ) {
        return wood_STL::prev_permutation( beg, end, std::less<decltype(*beg)>{} );
    }

    /**
     * @description: randomly shuffle the iter range
     * @return {void} return the shuffled origin iter
     */    
    template<typename RandomIterator>
    void random_shuffle(RandomIterator beg, RandomIterator end) {
        if(beg == end) return;
        for(auto iter = beg + 1; iter != end; ++iter) {
            std::iter_swap(iter, beg + (std::rand() % (iter - beg + 1)));
        }
    }

    // * '&&': this can directly deliver a rvalue to function
    // * it can avoid extra copy_construct and de-construct
    template<typename RandomIterator, typename RandomNumberGenerator>
    void random_shuffle(RandomIterator beg, RandomIterator end, RandomNumberGenerator &&rand) {
        if(beg == end) return;
        for(auto iter = beg + 1; iter != end; ++iter) {
            std::iter_swap(iter, beg + rand(iter - beg + 1));
        }
    }

    template<typename RandomIterator, typename UniformRandomNumberGenerator>
    void shuffle(RandomIterator beg, RandomIterator end, UniformRandomNumberGenerator &&generator) {
        if(beg == end) return;
        std::uniform_int_distribution<unsigned> distribution;
        using range_type = decltype(distribution)::param_type;
        for(auto iter = beg + 1; iter != end; ++iter) {
            std::iter_swap(iter, beg + distribution(generator, range_type(0, iter - beg)));
        }
    }

};  // namespace wood_STL


#endif /* _ALGORITHM_H_ */
