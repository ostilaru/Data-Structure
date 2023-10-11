/*
 * @Author: woodwood
 * @Date: 2023-10-10 15:28:25
 * @LastEditors: woodwood
 * @LastEditTime: 2023-10-11 16:08:56
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
            std::advance(iter, new_size); // move iter to the first illegal node
            erase(iter, end());
        } else if(new_size > size_) {
            insert(cend(), new_size - size_, value);
        }
    }
    
    reference front() {
        if(empty()) {
            throw list_exception("list::front(): list is empty!")
        }
        return *begin();
    }

    // reference can convert to const_reference
    const_reference front() const {
        return const_cast<list*>(this)->front();
    }

    reference back() {
        if(empty()) {
            throw list_exception("list::back(): list is empty!")
        }
        return *rbegin();
    }

    // reference can convert to const_reference
    const_reference back() const {
        return const_cast<list*>(this)->back();
    }

    template<typename... Args>
    void emplace_front(Args&&... args) {
        emplace(cbegin(), std::forward<Args>(args)...);
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        emplace(cend(), std::forward<Args>(args)...);
    }

    void pop_front() {
        if(empty()) {
            throw list_exception("list::pop_front(): list is empty!");
        }
        erase(begin());
    }

    void pop_back() {
        if(empty()) {
            throw list_exception("list::pop_back(): list is empty!");
        }
        erase(--end());
    }

    template<typename... Args>
    iterator emplace(const_iterator position, Args&&... args) {
        auto prev_node = position.ptr_->previous_;
        auto curr_node = std::move(prev_node->next_);

        auto new_node = make_unique<node>(value_type(std::forward<Args>(args)...), prev_node, std::move(curr_node));

        new_node->next_->previous_ = get_raw(new_node);
        prev_node->next_ = std::move(new_node);
        ++size_;
        return to_non_const(--position);
    }

    iterator insert(const_iterator position, const value_type& value) {
        auto copy = value;
        return insert(position, std::move(copy));
    }

    iterator insert(const_iterator position, value_type&& value) {
        return emplace(position, std::move(value));
    }

    iterator insert(const_iterator position, std::initializer_list<value_type> lst) {
        return insert(position, lst.begin(), lst.end());
    }

    /**
       inserts n copies of value before iterator position pos 
       returns the position of the first new element or pos if there is no new element ( n equals to zero )
    **/
    iterator insert(const_iterator position, size_type n, const value_type& value) {
        if(n == 0) {
            return to_non_const(position);
        }
        auto first_inserted = insert(position, value);

        for(size_type i = 1; i < n; ++i) {
            insert(position, value);
        }
        return first_inserted;
    }

    /**
       inserts a copy of all elements of the range [first, last) before iterator position pos 
       returns the position of the first new element or pos if there is no new element
    **/
    template<typename InputIterator, typename = wood_STL::RequireInputIterator<InputIterator>>
    iterator insert(const_iterator position, InputIterator first, InputIterator last) {
        if(first == last) {
            return to_non_const(position);
        }

        auto first_inserted = insert(position, *first);

        for(auto iter = ++first; iter != last; ++iter) {
            insert(position, *iter);
        }
        return first_inserted;
    }

    void remove(const value_type& value) {
        remove_if([&value](const value_type& elem) { return elem == value; });
    }

    template<typename Predicate>
    void remove_if(Predicate pred) {
        if(empty())
            return;
        auto iter = begin();
        auto last = end();

        while(iter != last) {
            if(pred(*iter)) {
                iter = erase(iter);
            } else {
                ++iter;
            }
        }
    }

    void unique() {
        unique(std::equal_to<value_type>());
    }

    template<typename BinaryPredicate>
    void unique(BinaryPredicate binary_pred) {
        if(size_ < 2) 
            return;
        auto last = end();
        auto current = begin();
        auto previous = current;
        ++current;

        while(current != last) {
            if(binary_pred(*previous, *current)) {
                current = erase(current);
            } else {
                previous = current;
                ++current;
            }
        }
    }

    iterator erase(const_iterator position) {
        if(position == cend()) {
            throw list_exception("list::erase(): the specify const_iterator is an off-the-end iterator!");
        }
        auto prev_node = position.ptr_->previous_;
        prev_node->next_ = std::move(position.ptr_->next_);
        prev_node->next_->previous_ = prev_node;
        --size_;
        // use '{}' to avoid return a raw pointer, we need an iterator
        return {get_raw(prev_node->next_)};
    }

    /**
       removes all elements of the range [fist, end) 
       returns the position of the next element
    **/
    iterator erase(const_iterator first, const_iterator last) {
        while(first != last) {
            first = erase(first);
        }
        return to_non_const(last);
    }

    void merge(list&& lst) {
        merge(std::move(lst), std::less<value_type>());
    }

    void merge(list &lst) {
        merge(std::move(lst));
    }

    template<typename Comp>
    void merge(list& lst, Comp comp) {
        merge(std::move(lst), comp);
    }

    template<typename Comp>
    void merge(list&& lst, Comp comp) {
        list new_list;

        auto iter1 = begin(), last1 = end();
        auto iter2 = lst.begin(), last2 = lst.end();

        while(iter1 != last1 && iter2 != last2) {
            if(comp(*iter1, *iter2)) {
                new_list.push_back(std::move(*iter1));
                ++iter1;
            } else {
                new_list.push_back(std::move(*iter2));
                ++iter2;
            }
        }

        if(iter1 != last1) {
            new_list.insert(new_list.end(), iter1, last1);
        } else {
            new_list.insert(new_list.end(), iter2, last2);
        }
        
        swap(new_list);
    }

    void reverse() noexcept {
        if(size() < 2)
            return;
        tail_ = get_raw(head_);
        auto curr = std::move(head_->next_);
        auto prev = std::move(head_);

        while(curr) {
            auto next = std::move(curr->next_);
            prev->previous_ = get_raw(curr);
            curr->next_ = std::move(prev);
            prev = std::move(curr);
            curr = std::move(next);
        }

        head_ = std::move(prev);
    }

    // insert other into this->position
    void splice(const_iterator position, list&& other) noexcept {
        splice(position, std::move(other), other.cbegin(), other.cend());
    }

    void splice(const_iterator position, list& other) {
        splice(position, std::move(other));
    }

    // insert other's ith elem into this->position
    void splice(const_iterator position, list& other, const_iterator i) noexcept {
        splice(position, std::move(other), i);
    }

    void splice(const_iterator position, list& other, const_iterator first, const_iterator last) noexcept {
        splice(position, std::move(other), first, last);
    }

    void splice(const_iterator position, list&& other, const_iterator i) noexcept {
        // TODO
        if (i == other.cend() || position == i || position == i + 1)
            return;

        auto prev_node = position.ptr_->previous_;
        auto curr_node = std::move(prev_node->next_);
        auto moved_node = std::move(i.ptr_);

        // Connect the nodes from 'other' to the current list
        prev_node->next_ = std::move(moved_node);
        moved_node->previous_ = prev_node;

        moved_node->next_ = std::move(curr_node);
        curr_node->previous_ = moved_node;

        // Adjust the size of both lists
        ++size_;
        --(other.size_);

        // Reset the size of the 'other' list to 0 if it's empty
        if (other.empty()) {
            other.head_->next_ = std::move(other.tail_->previous_->next_);
            other.tail_->previous_ = get_raw(other.head_);
        }
    }


    void splice(const_iterator position, list&& other, const_iterator first, const_iterator last) noexcept {
        // TODO
        if(other.empty() || first == last)
            return;
        
        auto prev_node = position.ptr_->previous_;
        auto curr_node = std::move(prev_node->next_);

        // find the last node in the range [first, last)
        auto last_node = first.ptr_;
        while(last_node->next_ != last.ptr_)
            last_node = last_node->next_;

        // connect the surrounding nodes of the range [first, last)
        prev_node->next_ = std::move(first.ptr_);
        first.ptr_->previous_ = prev_node;

        last_node->next_ = std::move(curr_node);
        curr_node->previous_ = last_node;

        // Adjust the size of the lists
        size_ += std::distance(first, last);
        other.size_ -= std::distance(first, last);

        // Reset the size of the 'other' list to 0
        other.head_->next_ = std::move(other.tail_->previous_->next_);
        other.tail_->previous_ = get_raw(other.head_);
        other.size_ = 0;
    }

    void sort() {
        sort(std::less<value_type>());
    }

    template <class Comp>
    void sort(Comp comp) {
        // TODO
        if (size_ < 2)
            return;

        auto mid = begin() + size_ / 2;

        list left(begin(), mid);
        list right(mid, end());

        left.sort(comp);
        right.sort(comp);

        merge(std::move(left), std::move(right), comp);
    }

private:
    static node_raw_ptr get_raw(node_ptr& ptr) noexcept {
        return ptr.get();
    }

    void init() {
        head_ = make_unique<node>();
        head_->next_ = make_unique<node>(value_type(), get_raw(head_), nullptr);
        tail_ = get_raw(head_->next_);
        size_ = 0;
    }

    iterator to_non_const(const_iterator iter) noexcept {
        return {iter.ptr_};
    }

public:
    bool operator==(const list& other) const noexcept {
        if(this == &other) {
            return true;
        }
        if(size_ != other.size_) {
            return false;
        }
        return wood_STL::equal(cbegin(), cend(), other.cbegin());
    }

    bool operator!=(const list& other) const noexcept {
        return !(*this == other);
    }

    bool operator<(const list& other) const noexcept {
        return std::lexicographical_compare(cbegin(), cend(), other.cbegin(), other.cend());
    }

    bool operator>(const list& other) const noexcept {
        return other < *this;
    }

    bool operator>=(const list& other) const noexcept {
        return !(*this < other);
    }

    bool operator<=(const list& other) const noexcept {
        return !(*this > other);
    }


};  // class list

template<typename T>
inline void swap(list<T>& left, list<T>& right) noexcept {
    left.swap(right);
}

template<typename T>
inline std::ostream &operator<<(std::ostream &os, const list<T> &lst) {
    for(const auto &elem : lst) {
        os << elem << " ";
    }
    return os;
}

};  // namespace wood_STL

#endif