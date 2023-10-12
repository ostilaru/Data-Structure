/*
 * @Author: woodwood
 * @Date: 2023-10-11 16:11:36
 * @LastEditors: woodwood
 * @LastEditTime: 2023-10-12 16:35:20
 * @FilePath: \Data-Structure\queue.hpp
 * @FileName: queue.hpp
 * @Description: Description
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "list.hpp"
#include <string>
#include <exception>

namespace wood_STL {

class queue_exception : public std::exception {
public:
    explicit queue_exception(const std::string &msg) : msg_(msg) {}

    virtual const char *what() const noexcept override {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

template<typename T, typename Container = wood_STL::list<T>>
class queue {
public:
    using container_type        = Container;
    using value_type            = typename Container::value_type;
    using reference             = typename Container::reference;
    using const_reference       = typename Container::const_reference;
    using size_type             = typename Container::size_type;

protected:
    Container container_;

public:
    explicit queue(const Container &container) : container_(container) {}

    explicit queue(Container &&container) : container_(std::move(container)) {}

    ~queue() = default;

    bool empty() const {
        return container_.empty();
    }

    size_type size() const {
        return container_.size();
    }

    reference front() {
        if(empty()) {
            throw queue_exception("queue::front(): the queue is empty!");
        }
        return container_.front();
    }

    const_reference front() const {
        if(empty()) {
            throw queue_exception("queue::front(): the queue is empty!");
        }
        return container_.front();
    }

    reference back() {
        if(empty()) {
            throw queue_exception("queue::back(): the queue is empty!");
        }
        return container_.back();
    }

    const_reference back() const {
        if(empty()) {
            throw queue_exception("queue::back(): the queue is empty!");
        }
        return container_.back();
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
        if(empty()) {
            throw queue_exception("queue::pop(): the queue is empty!");
        }
        container_.pop_front();
    }

    void swap(queue &other) noexcept(noexcept(swap(container_, other.container_))) {
        using std::swap;
        swap(container_, other.container_);
    }

    bool operator==(const queue &other) {
        return container_ == other.container_;
    }

    bool operator!=(const queue &other) {
        return !(*this == other);
    }

    bool operator<(const queue &other) {
        return container_ < other.container_;
    }

    bool operator>=(const queue &other) {
        return !(*this < other);
    }

    bool operator>(const queue &other) {
        return other < *this;
    }

    bool operator<=(const queue &other) {
        return !(*this > other);
    }


};  // class queue

template<typename T, typename Container>
inline void swap(queue<T, Container> &left, queue<T, Container> &right) noexcept(noexcept(left.swap(right))) {
    left.swap(right);
}


    
};  // namespace wood_STL

#endif  /* _QUEUE_H_ */