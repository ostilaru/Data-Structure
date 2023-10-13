/*
 * @Author: woodwood
 * @Date: 2023-10-12 16:41:24
 * @LastEditors: woodwood
 * @LastEditTime: 2023-10-13 14:42:01
 * @FilePath: \Data-Structure\forward_list.hpp
 * @FileName: forward_list.hpp
 * @Description: 1. use smart-pointer to manage memory
 *               2. introduce exception
 *               3. no allocator
 */

#ifndef _FORWARD_LIST_H_
#define _FORWARD_LIST_H_

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

class forward_list_exception : public std::exception {
public:
    explicit forward_list_exception(const std::string &msg) : msg_(msg) {}

    virtual const char *what() const noexcept override {
        return msg_.c_str();
    }

private:
    std::string msg_;

};  // class forward_list_exception

template<typename T>
class forward_list {
private:
    struct node;
    using node_ptr      = std::unique_ptr<node>;    // smart pointer pointing to node
    using node_raw_ptr  = node *;                   // raw pointer pointing to node

    struct node {
        node(const T &value, node_ptr &&next) : value_(value), next_(std::move(next)) {}

        node(T &&value, node_ptr &&next) : value_(std::move(value)), next_(std::move(next)) {}

        explicit node(const T &value = T{}) : value_(value), next_(nullptr) {}

        // copy-constructor is not allowed
        node(const node &) = delete;
        node &operator=(const node &) = delete;

        // move-constructor is not allowed
        node(node &&) = default;
        node &operator=(node &&) = default;

        T value_;
        node_ptr next_;
    };
    
public:
    using value_type        = T;
    using pointer           = T*;
    using const_pointer     = const T*;
    using reference         = T&;
    using const_reference   = const T&;
    using size_type         = std::size_t;
    using difference_type   = std::ptrdiff_t;

    class const_iterator {
        friend class forward_list;
    public:
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        const_iterator() noexcept : ptr_(nullptr) {}
    
        reference operator*() const {
            return ptr_->value_;
        }

        pointer operator->() const {
            return &(operator*());
        }

        // front ++
        const_iterator &operator++() {
            ptr_ = get_raw(ptr_->next_);
            return *this;
        }

        // back ++
        const_iterator operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const const_iterator &other) const noexcept {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const const_iterator &other) const noexcept {
            return !(*this == other);
        }

    protected:
        const_iterator(node_raw_ptr ptr) noexcept : ptr_(ptr) {}
        node_raw_ptr ptr_;

    };  // class const_iterator

    class iterator : public const_iterator {
        friend class forward_list;
    public:
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        iterator() noexcept = default;

        reference operator*() const {
            return this->ptr_->value_;
        }

        pointer operator->() const {
            return &(operator*());
        }

        iterator &operator++() {
            this->ptr_ = get_raw(this->ptr_->next_);
            return *this;
        }

        iterator operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        /**
           The fowllowing two function inherit from const_iterator:

           bool operator==( const iterator &other ) const noexcept;
           bool operator!=( const iterator &other ) const noexcept;
        **/

    protected:
        iterator(node_raw_ptr ptr) noexcept : const_iterator(ptr) {}

    };  // class iterator


private:
    node_ptr head_ = make_unique<node>();   // dummy node
    size_type size_ = 0;                    // number of nodes

public: 
    forward_list() = default;

    explicit forward_list(size_type n) : forward_list(n, value_type()) {}

    forward_list(size_type n, const value_type &value) {
        insert_after(cbefore_begin(), n, value);
    }

    template<class InputIterator, typename = wood_STL::RequireInputIterator<InputIterator>>
    forward_list(InputIterator first, InputIterator last) {
        insert_after(cbefore_begin(), first, last);
    }

    forward_list(const forward_list &other) : forward_list(other.cbegin(), other.cend()) {}

    forward_list(forward_list &&other) noexcept {
        swap(other);
    }

    forward_list(std::initializer_list<value_type> lst) : forward_list(lst.begin(), lst.end()) {}

    /**
       call head_'s destructor, all nodes' memory will be free 
    **/
    ~forward_list() = default;

    /**
       can handle the problem of self-assignment, see C++ Primer 5th section 13.3
    **/
    forward_list &operator=(const forward_list &other) {
        auto copy = other;
        swap(copy);
        return *this;
    }

    forward_list &operator=(forward_list &&other) noexcept {
        if(this != &other) {
            clear();
            swap(other);
        }
        return *this;
    }

    forward_list &operator=(std::initializer_list<value_type> lst) {
        assign(lst.begin(), lst.end());
        return *this;
    }

    void push_front(const value_type &value) {
        auto copy = value;
        push_front(std::move(copy));
    }

    void push_front(value_type &&value) {
        emplace_front(std::move(value));
    }

    template<class... Args>
    void emplace_front(Args&&... args) {
        emplace_after(cbefore_begin(), std::forward<Args>(args)...);
    }

    size_type size() const noexcept {
        return size_;
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    void pop_front() {
        if(empty()) {
            throw forward_list_exception("forward_list::pop_front(): forward_list is empty!");
        }
        erase_after(before_begin());
    }

    reference front() {
        if(empty()) {
            throw forward_list_exception("forward_list::front(): forward_list is empty!");
        }
        return *begin();
    }

    const_reference front() const {
        return const_cast<forward_list*>(this)->front();
    }

    iterator before_begin() noexcept {
        return {get_raw(head_)};
    }

    const_iterator before_begin() const noexcept {
        return {get_raw(head_)};
    }

    iterator begin() noexcept {
        return {get_raw(head_->next_)};
    }

    const_iterator begin() const noexcept {
        return {get_raw(head_->next_)};
    }

    iterator end() noexcept {
        return { };
    }

    const_iterator end() const noexcept {
        return { };
    }

    const_iterator cbefore_begin() const noexcept {
        return before_begin();
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_iterator cend() const noexcept {
        return end();
    }

    template<typename InputIterator, typename = wood_STL::RequireInputIterator<InputIterator>>
    void assign(InputIterator first, InputIterator last) {
        clear();
        insert_after(cbefore_begin(), first, last);
    }

    void assign(size_type n, const value_type &value) {
        clear();
        insert_after(cbefore_begin(), n, value);
    }

    void assign(std::initializer_list<value_type> lst) {
        assign(lst.begin(), lst.end());
    }

    void swap(forward_list &other) noexcept {
        using std::swap;
        swap(size_, other.size_);
        swap(head_, other.head_);
    }

    void clear() noexcept {
        head_->next_ = nullptr;
        size_ = 0;
    }

    /** 
        erase one element after position and return iterator pointing to the next element after the erased one. 
        if position is an off-the-end iterator or no element after position, then throw exception
     **/
    iterator erase_after(const_iterator position) {
        auto ptr = position.ptr_;
        if(!ptr) {
            throw forward_list_exception("forward_list::erase_after(): can't erase element after an off-the-end iterator");
        }
        if(!ptr->next_) {
            throw forward_list_exception("forward_list::erase_after(): no element after the specify iterator");
        }

        ptr->next_ = std::move(ptr->next_->next_);
        --size_;

        return to_non_const(++position);
    }

    /**
       erase elements in range ( position, last ), but not include position and last
       you must insure this range is valid, that means the range must be empty or contain at least one element, otherwise throw exception
    **/
    iterator erase_after(const_iterator position, const_iterator last) {
        // if range (position, last) is empty, then we just return
        auto pos = position;
        if(++pos == last) {
            return to_non_const(last);
        }
        auto next_erased = erase_after(position);
        while(next_erased != last) {
            next_erased = erase_after(next_erased);
        }
        return to_non_const(last);
    }

    iterator insert_after(const_iterator position, const value_type &value) {
        auto copy = value;
        return insert_after(position, std::move(copy));
    }

    /** 
        insert value after position and return iterator pointing to the new element
        if position is an off-the-end iterator, then throw exception
    **/
    iterator insert_after(const_iterator position, value_type &&value) {
        if(position == cend()) {
            throw forward_list_exception("forward_list::insert_after(): can't insert element after an off-the-end iterator");
        }
        return emplace_after(position, std::move(value));
    }

    /**
       insert n values after position and return iterator pointing to the last inserted element
       if position is an off-the-end iterator, then throw exception
    **/
    iterator insert_after(const_iterator position, size_type n, const value_type &value) {
        for(size_type i = 0; i < n; ++i) {
            position = insert_after(position, value);
        }
        return to_non_const(position);
    }

    /**
       insert n values after position and return iterator pointing to the last inserted element
       if position is an off-the-end iterator, then throw exception
    **/
    template<typename InputIterator, typename = wood_STL::RequireInputIterator<InputIterator>>
    iterator insert_after(const_iterator position, InputIterator first, InputIterator last) {
        for(auto iter = first; iter != last; ++iter) {
            position = insert_after(position, *iter);
        }
        return to_non_const(position);
    }

    iterator insert_after(const_iterator position, std::initializer_list<value_type> lst) {
        return insert_after(position, lst.begin(), lst.end());
    }

    template<class... Args>
    iterator emplace_after(const_iterator position, Args&&... args) {
        if(position == cend()) {
            throw forward_list_exception("forward_list::emplace_after(): can't emplace element after an off-the-end iterator");
        }

        auto ptr = position.ptr_;
        ptr->next_ = make_unique<node>(value_type(std::forward<Args>(args)...), std::move(ptr->next_));
        ++size_;
        return to_non_const(++position);
    }

    void resize(size_type new_size) {
        resize(new_size, value_type());
    }

    void resize(size_type new_size, const value_type& value) {
        // if new_size equals to size_, do nothing
        if(new_size < size_) {
            auto iter = cbefore_begin();
            std::advance(iter, new_size);
            erase_after(iter, cend());
        } else if(new_size > size_) {
            auto iter = cbefore_begin();
            std::advance(iter, size_);
            insert(iter, new_size - size_, value);
        }
    }



private:
    /**
       get the raw pointer in unique_ptr
     **/
    static node_raw_ptr get_raw(const node_ptr &ptr) noexcept {
        return ptr.get();
    }

    iterator to_non_const(const_iterator iter) noexcept {
        return {iter.ptr_};
    }





};  // class forward_list

template<typename T>
inline void swap(forward_list<T> &left, forward_list<T> &right) noexcept {
    left.swap(right);
}

template<typename T>
inline std::ostream &operator<<(std::ostream &os, const forward_list<T> &lst) {
    for(const auto &elem : lst) {
        os << elem << " ";
    }
    return os;
}


};  // namespace wood_STL



#endif /* _FORWARD_LIST_H_ */