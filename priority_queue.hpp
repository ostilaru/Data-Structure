/*
 * @Author: woodwood
 * @Date: 2023-10-19 23:47:36
 * @LastEditors: woodwood
 * @LastEditTime: 2023-10-19 23:57:53
 * @FilePath: /Data-Structure/priority_queue.hpp
 * @Description: todo
 */
#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include "heap.hpp"
#include "vector.hpp"
#include <exception>
#include <string>

namespace wood_STL {

class priority_queue_exception : public std::exception {
public:
    explicit priority_queue_exception(const std::string &msg) : msg_(msg) {}

    virtual const char *what() const noexcept override { return msg_.c_str(); }

private:
    std::string msg_;
}


template <typename T, typename Container = wood_STL::vector<T>, typename Comp = std::less<T>>
class priority_queue {
public:
    using container_type    = Container;
    using value_type        = typename Container::value_type;
    using reference         = typename Container::reference;
    using const_reference   = typename Container::const_reference;
    using size_type         = typename Container::size_type;

protected:
    Comp        comp_;          // for compare elem
    Container   container_;     // the underlying container

public:
    explicit priority_queue(const Comp &comp = Comp{}, const Container &c = Container{}) : comp_(comp) , container_(c) {
        wood_STL::make_heap(container_.begin(), container_.end(), comp_);
    }

}



}   // namespace wood_STL


#endif /* _PRIORITY_QUEUE_H_ */