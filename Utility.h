#ifndef ZYX_UTILITY
#define ZYX_UTILITY 

namespace Zyx {

template <typename T>
void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

template <typename ForwardIterator1, typename ForwardIterator2>
void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
{
    swap(*a, *b);
}

}

#endif