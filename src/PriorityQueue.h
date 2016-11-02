#ifndef ZYX_PRIORIY_QUEUE
#define ZYX_PRIORIY_QUEUE 

#include "Vector.h"
#include "Heap.h"
#include "Functional.h"

namespace Zyx {

template <typename T, typename Container = Vector<T>, 
		  typename Compare = less<typename Container::value_type>>
class PriorityQueue
{
public:
    typedef typename Container::value_type         value_type;
    typedef typename Container::size_type          size_type;
    typedef typename Container::reference          reference;
    typedef typename Container::const_reference    const_reference;
    typedef Container                              container_type;

public:
    PriorityQueue() : c() 
    { 
    }

    explicit PriorityQueue(const Compare& x) : c(), comp(x) 
    { 
    }

    template <typename InputIterator>
    PriorityQueue(InputIterator first, InputIterator last) 
      : c(first, last) 
    {
        make_heap(c.begin(), c.end(), comp); 
    }

    template <typename InputIterator>
    PriorityQueue(InputIterator first, InputIterator last, const Compare& x) 
      : c(first, last) 
    {
        make_heap(c.begin(), c.end(), comp); 
    }

public:
    bool empty() const 
    { 
        return c.empty(); 
    }

    size_type size() const 
    { 
        return c.size(); 
    }

    const_reference top() const 
    { 
        return c.front(); 
    }

    void push(const T& val) 
    {
        c.push_back(val);
        push_heap(c.begin(), c.end(), comp);
    }

    void pop()
    {
        pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
    }

private:
    Container c;
    Compare comp;
}; 

}

#endif