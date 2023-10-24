#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

#include "memory.hpp"
#include "iterator.hpp"
#include "stack.hpp"
#include "algorithm.hpp"
#include <string>
#include <exception>
#include <functional>
#include <iostream>
#include <initializer_list>

namespace wood_STL {

class binary_tree_exception : public std::exception {
public:
    explicit binary_tree_exception(const std::string &message) : message_(message) {}

    virtual const char * what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

template<typename T, typename Comp = std::less<T>>
class binary_tree {
public:
    using value_type            = T;
    using pointer               = T*;
    using const_poiner          = const T*;
    using reference             = T&;
    using const_reference       = const T&;
    using size_type             = std::size_t;
    using difference_type       = std::ptrdiff_t;

private:
    struct node;
    using node_ptr      = std::unique_ptr<node>;
    using node_raw_ptr  = node*;

    struct node {
        node(const T &value, node_ptr &&left, node_ptr &&right) : value_(value), left_(std::move(left)), right_(std::move(right)) {}

        node(T &&value, node_ptr &&left, node_ptr &&right) : value_(std::move(value)), left_(std::move(left)), right_(std::move(right)) {}

        explicit node(const T &value = T{}) : value_(value), left_(nullptr), right_(nullptr) {}

        node(const node &) = delete;
        node &operator=(const node &) = delete;

        
    }

}



};   // namespace wood_STL

#endif /* _BINARY_TREE_H_ */