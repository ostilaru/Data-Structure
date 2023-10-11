/*
 * @Author: woodwood
 * @Date: 2023-10-10 15:28:25
 * @LastEditors: woodwood
 * @LastEditTime: 2023-10-11 11:29:44
 * @FilePath: \Data-Structure\list.hpp
 * @FileName: list.hpp
 * @Description: Double Linked List
 *      1. introduce smart pointer for automatic memory management
 *      2. introduce exceptions: illegal operations will trigger exceptions
 *      3. Prohibit the use of allocators for memory allocations
 */

#ifndef _LIST_H_
#define _LIST_H_

#include "algorithm.hpp"
#include "memory.hpp"     
#include "iterator.hpp"         
#include <string>
#include <exception>               // for std::exception
#include <cstddef>                 // for std::size_t
#include <iostream>                // for debug
#include <functional>              // for std::less<>
#include <initializer_list>        // for std::initializer_list<>


namespace wood_STL {

class list_exception : public std::exception {
public:
    list_exception(const std::string &message) : message_(message) {}

    virtual const char *what() const noexcept override { return message_.c_str(); }

private:
    std::string message_;

};

template<typename T>
class list {
private:
    struct node;
    using node_raw_ptr = node *;
    using node_ptr = std::unique_ptr<node>;

    struct node {
        explicit node(const T &value, node_raw_ptr prev, node_ptr &&next)
            :value_(value), previous_(prev), next_(std::move(next)) {}

        explicit node(T &&value, node_raw_ptr prev, node_ptr &&next)
            :value_(std::move(value)), previous_(prev), next_(std::move(next)) {}

        explicit node(const T &value = T{}) : value_(value), previous_(nullptr), next_(nullptr) {}

        // can not be copied
        node(const node &) = delete;
        ndoe &operator=(const node &) = delete;

        node(node &&) = default;
        node &operator=(node &&) = default;

        T value_;
        node_raw_ptr previous_;     // raw pointer points to the previous node
        node_ptr next_;             // smart pointer points to the next node
    };

public:
    using value_type                = T;
    using pointer                   = T*;
    using const_pointer             = const T*;
    using reference                 = T&;
    using const_reference           = const T&;
    using size_type                 = std::size_t;
    using difference_type           = std::ptrdiff_t;

    class const_iterator 
    {
        friend class list;
    public:
        using value_type = T;
        using pointer    = const T*;
        using reference  = const T&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        const_iterator() noexcept : ptr_(nullptr) {}

        reference operator*() const {
            return ptr_->value_;
        }

        pointer operator->() const {
            return &(opperator*());
        }

        const_iterator &operator++() {
            ptr = get_raw(ptr_->next_);
            return *this;
        }

        const_iterator operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        const_iterator &operator--() {
            ptr_ = ptr_->previous_;
            return *this;
        }

        consrt_iterator operator--(int) {
            auto tmp = this:
            --*this;
            return tmp;
        }

        bool operator==(const const_iterator &other) const noexcept{
            return ptr_ == other.ptr_;
        }

        bool operator!=(const const_iterator &other) const noexcept{
            return ptr_ != other.ptr_;
        }

    protected:
        const_iterator(node_raw_ptr) noexcept : ptr_(ptr) {}

        node_raw_ptr ptr_;  // point to node
    };  // class const_iterator


    class iterator : public const_iterator 
    {
        friend class list;
    public:
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator() noexcept = default;

        reference operator*() const {
            return this->ptr_->value_;
        }

        pointer operator->() const {
            return &(operator*());
        }

        // front: ++ first then return 
        iterator &operator++() {
            this->ptr_ = get_raw(this->ptr_->next_);
            return *this;
        }

        // back: first return then ++
        iterator operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        iterator &operator--() {
            this->ptr_ = this->ptr_->previous_;
            return *this;
        }

        iterator operator--(int) {
            auto tmp = *this;
            --*this;
            return tmp;
        }

    protected:
        // construct
        iterator(node_raw_ptr ptr) noexcept : const_iterator(ptr) {}

    };  // class iterator

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    node_ptr        head_;  // dummy node for head
    node_raw_ptr    tail_;  // dummy node for tail
    size_type       size_;  // number of nodes

public:
    list() {
        init();
    }

    explicit list(size_type n) : list(n, value_type()) {}

    list(size_type n, const value_type &value) {
        init();
        insert(cend(), n, value);
    }

    /**
     * @description: construct list with [begin, end) iterator
     * @example: std::vector<int> vec = {1, 2, 3};  wood_STL::list<int> mylist(vec.begin(), vec.end());
     * @return {*}
     */    
    template <class InputIterator, typename = wood_STL::RequireInputIterator<InputIterator>>
    list(InputIterator first, InputIterator last) {
        init();
        insert(cend(), first, last);
    }

    /**
     * @description: construct list with exist initializer_list
     * @example: wood_STL::list<int> mylist = {1, 2, 3};
     * @return {*}
     */    
    list(std::initializer_list<value_type> lst) : list(lst.begin(), lst.end()) {}

    /**
     * @description: construct list with another list
     * @example: wood_STL::list<int> other = {1,2,3}; wood_STL::list<int> mylist(other);
     * @return {*}
     */   
    list(const list &other) : list(other.cbegin(), other.cend()) {}

    // can handle the problem of self-assignment, see C++ Primer 5th section 13.3
    list &operator=(const list &other) {
        auto copy = other;
        swap(copy);
        return *this;
    }

    list(list &&other) noexcept {
        swap(other);
    }

    list &operator=(list &&other) noexcept {
        if(this != &other) {
            clear();
            swap(other);
        }
        return *this;
    }

    // call head_'s destructor, all node's memory will be free
    ~list() = default;

    void swap(list &other) noexcept {
        using std::swap;
        swap(head_, other.head_);
        swap(tail_, other.tail_);
        swap(size_, other.size_);
    }

    void clear() noexcept {
        head_->next_ = std::move(tail_->previous_->next_);
        tail_->previous_ = get_raw(head_);
        size_ = 0;
    }

    list &operator=(std::initializer_list<value_type> lst) {
        assign(lst.begin(), lst.end());
        return *this;
    }

    void assign(size_type n, const value_type &value) {
        clear();
        insert(cend(), n, value);
    }

    template<typename InputIterator, typename = wood_STL::RequireInputIterator<InputIterator>>
    void assign(InputIterator first, InputIterator last) {
        clear();
        insert(cend(), first, last);
    }

    void assign(std::initializer_list<value_type> lst) {
        assign(lst.begin(), lst.end());
    }

    iterator begin() noexcept {
        return {get_raw(head_->next_)};
    }

    const_iterator begin() const noexcept {
        return {get_raw(head_->next_)};
    }

    iterator end() noexcept {
        return {tail_};
    }

    const_iterator end() const noexcept {
        return {tail_};
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_iterator cend() const noexcept {
        return end();
    }

    const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    const_reverse_iterator crend() const noexcept {
        return rend();
    }

    void push_back(const value_type& value) {
        auto copy = value;
        push_back(std::move(copy));
    }

    void push_back(value_type&& value) {
        emplace_back(std::move(value));
    }

    void push_front(const value_type& value) {
        auto copy = value;
        push_front(std::move(copy));
    }

    void push_front(value_type&& value) {
        emplace_front(std::move(value));
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    size_type size() const noexcept {
        return size_;
    }

    void resize(size_type new_size) {
        resize(new_size, value_type());
    }

    void resize(size_type new_size, const value_type& value) {
        // if new_size equals to size_, do nothing
        if(new_size < size_) {
            auto iter = begin();
            std::advance(iter, new_size);
            erase(iter, end());
        } else if(new_size > size_) {
            insert(cend(), new_size - size_, value);
        }
    }
    


};  // class list


};  // namespace wood_STL
#endif