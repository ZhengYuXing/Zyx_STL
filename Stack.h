#ifndef ZYX_STACK
#define ZYX_STACK 

#include "Deque.h"

namespace Zyx {

template <typename T, typename Container = Deque<T>>
class Stack
{
public:
    template <typename T, typename Container>
    friend bool operator==(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs);
    template <typename T, typename Container>
    friend bool operator!=(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs);	
    template <typename T, typename Container>
    friend bool operator<(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs);
    template <typename T, typename Container>
    friend bool operator<=(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs);
    template <typename T, typename Container>
    friend bool operator>(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs);
    template <typename T, typename Container>
    friend bool operator>=(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs);
	
public:
    typedef typename Container::value_type         value_type;
    typedef typename Container::size_type          size_type;
    typedef typename Container::reference          reference;
    typedef typename Container::const_reference    const_reference;
    typedef Container                              container_type;

public:
    Stack() : c() { }

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }

    reference top() { return c.back(); }
    const_reference top() const { return c.back(); }

    void push(const T& val) { c.push_back(val); }
    void pop() { c.pop_back(); }

    void swap(Stack& x) { Zyx::swap(c, x.c); }

private:
    Container c;
};

template <typename T, typename Container>
bool operator==(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs)
{
    return lhs.c == rhs.c;
}

template <typename T, typename Container>
bool operator!=(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs)
{
    return lhs.c != rhs.c;
}

template <typename T, typename Container>
bool operator<(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs)
{
    return lhs.c < rhs.c;
}

template <typename T, typename Container>
bool operator<=(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs)
{
    return lhs.c <= rhs.c;
}

template <typename T, typename Container>
bool operator>(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs)
{
    return lhs.c > rhs.c;
}

template <typename T, typename Container>
bool operator>=(const Stack<T, Container>& lhs, const Stack<T, Container>& rhs)
{
    return lhs.c >= rhs.c;
}

template <typename T, typename Container>
void swap(Stack<T, Container>& x, Stack<T, Container>& y)
{
    x.swap(y);
}

}

#endif