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

template <typename T1, typename T2>
struct Pair
{
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    Pair() { }

    Pair(const first_type& a, const second_type& b) : first(a), second(b) { }

    Pair(const Pair& pr) : first(pr.first), second(pr.second) { }

    template <typename U, typename V>
    Pair(const Pair<U, V>& pr) : first(pr.first), second(pr.second) { }

    Pair& operator=(const Pair& pr)
    {
        if (this != &pr) {
            first = pr.first;
            second = pr.second;
        }
        return *this;
    }

    template <typename U, typename V>
    Pair& operator=(const Pair<U, V>& pr)
    {
        if (this != &pr) {
            first = pr.first;
            second = pr.second;
        }
        return *this;   	
    }

    void swap(Pair& pr)
    {
        Zyx::swap(first, pr.first);
        Zyx::swap(second, pr.second);
    }
};

template <typename T1, typename T2>
inline Pair<T1, T2> make_pair(const T1& x, const T2& y)
{
    return Pair<T1, T2>(x, y);
}

template <typename T1, typename T2>
inline bool operator==(const Pair<T1, T2>& lhs, const Pair<T1, T2>& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <typename T1, typename T2>
inline bool operator!=(const Pair<T1, T2>& lhs, const Pair<T1, T2>& rhs)
{
    return !(lhs == rhs);
}

template <typename T1, typename T2>
inline bool operator<(const Pair<T1, T2>& lhs, const Pair<T1, T2>& rhs)
{
    return lhs.first < rhs.first || (!(lhs.first < rhs.first) && lhs.second < rhs.second);
}

template <typename T1, typename T2>
inline bool operator<=(const Pair<T1, T2>& lhs, const Pair<T1, T2>& rhs)
{
    return !(rhs < lhs);
}

template <typename T1, typename T2>
inline bool operator>(const Pair<T1, T2>& lhs, const Pair<T1, T2>& rhs)
{
    return rhs < lhs;
}

template <typename T1, typename T2>
inline bool operator>=(const Pair<T1, T2>& lhs, const Pair<T1, T2>& rhs)
{
    return !(lhs < rhs);
}

template <typename T1, typename T2>
inline void swap(Pair<T1, T2>& x, Pair<T1, T2>& y)
{
    x.swap(y);
}

}

#endif