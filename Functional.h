#ifndef ZYX_FUNCTIONAL
#define ZYX_FUNCTIONAL 

namespace Zyx {

template <typename T>
struct equal_to
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef bool result_type;
    bool operator()(const T& x, const T& y) const { return x == y; }
};

template <typename T>
struct not_equal_to
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef bool result_type;
    bool operator()(const T& x, const T& y) const { return x != y; }
};

template <typename T>
struct less
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef bool result_type;
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template <typename T>
struct less_equal
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef bool result_type;
    bool operator()(const T& x, const T& y) const { return x <= y; }
};

template <typename T>
struct greater
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef bool result_type;
    bool operator()(const T& x, const T& y) const { return x > y; }
};

template <typename T>
struct greater_equal
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef bool result_type;
    bool operator()(const T& x, const T& y) const { return x >= y; }
};

template <typename T> 
struct negate
{
    typedef T argument_type;
    typedef T result_type;
    T operator()(const T& x) const { return -x; }
};

template <typename T>
struct plus
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;
    T operator()(const T& x, const T& y) const { return x + y; }
};

template <typename T>
struct minus
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;
    T operator()(const T& x, const T& y) const { return x - y; }
};

template <typename T>
struct multiplies
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;
    T operator()(const T& x, const T& y) const { return x * y; }
};

template <typename T>
struct divides
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;
    T operator()(const T& x, const T& y) const { return x / y; }
};

template <typename T>
struct modulus
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;
    T operator()(const T& x, const T& y) const { return x % y; }
};

template <typename T>
struct logical_and
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;
    bool operator()(const T& x, const T& y) const { return x && y; }
};

template <typename T>
struct logical_or
{
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef T result_type;
    bool operator()(const T& x, const T& y) const { return x || y; }
};

template <typename T>
struct logical_not
{
    typedef T argument_type;
    typedef T result_type;
    bool operator()(const T& x) const { return !x; }
};

template <typename T>
struct identity
{
    typedef T argument_type;
    typedef T result_type;
    const T& operator()(const T& x) const { return x; }
};

template <typename Pair>
struct select1st
{
    typedef Pair argument_type;
    typedef typename Pair::first_type result_type;
    const typename Pair::first_type& operator()(const Pair& x) const 
    { 
        return x.first; 
    }
};

}

#endif