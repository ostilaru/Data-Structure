/*
 * @Author: woodwood
 * @Date: 2023-10-07 14:40:00
 * @LastEditors: woodwood
 * @LastEditTime: 2023-10-10 14:44:08
 * @FilePath: \Data-Structure\stack.hpp
 * @FileName: stack.hpp
 * @Description: 1. based on vector as a lower container instead of a deque
 *               2. throw an exception when illegal operation happen
 */


#ifndef _STACK_H_
#define _STACK_H_

#include "vector.hpp"
#include <exception>
#include <string>

namespace wood_STL {

class stack_exception : public std::exception {
public:
    explicit stack_exception(const std::string &message) : message_(message) {}

    virtual const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

template<typename T, typename Container = wood_STL::vector<T>>
class stack {
public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type = typename Container::size_type;

protected:
    Container container_;

public:
    explicit stack(const Container &container) : container_(container) {}

    // std::move() => transfer a lvalue to a rvalue
    explicit stack(Container &&container = Container()) : container_(std::move(container)) {}

    bool empty() const {
        return container_.empty();
    }

    size_type size() const {
        return container_.size();
    }

    reference top() {
        if(container_.empty()) {
            throw stack_exception("stack::top(): stack is empty");
        }
        return container_.back();
    }

    const_reference top() const {
        return const_cast<stack *>(this)->top();
    }

    void push(const value_type &value) {
        auto copy = value;
        push(std::move(copy));
    }

    void push(value_type &&value) {
        emplace(std::move(value));
    }

    template<typename... Args>
    void emplace(Args &&... args) {
        container_.emplace_back(std::forward<Args>(args)...);
    }

    void pop() {
        if(container_.empty())
            throw stack_exception("stack::pop(): stack is empty");
        container_.pop_back();
    }

    void swap(stack &other) noexcept(noexcept(swap(container_, other.container_))) {
        using std::swap;
        swap(container_, other.container_);
    }

    bool operator==(const stack &other) {
        return container_ == other.container_;
    }

    bool operator!=(const stack &other) {
        return !(*this == other);
    }

    bool operator<(const stack &other) {
        return container_ < other.container_;
    }

    bool operator>=(const stack &other) {
        return !(*this < other);
    }
    
    bool operator>(const stack &other) {
        return other < *this;
    }

    bool operator<=(const stack &other) {
        return !(*this > other);
    }

};

template<typename T, typename Container>
inline void swap(stack<T, Container> &left, stack<T, Container> &right) noexcept(noexcept(left.swap(right))) {
    left.swap(right);
}

};


#endif /* _STACK_H_ */