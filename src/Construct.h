#ifndef ZYX_CONSTRUCT
#define ZYX_CONSTRUCT 

#include <new>
#include "TypeTraits.h"
#include "Algorithm.h"

namespace Zyx
{

//------------------------------------【construct() function】---------------------------------

template <typename T1, typename T2>
inline void construct(T1* p, const T2& value)
{
    new(p) T1(value);
}

//-------------------------------------【destroy() function】----------------------------------

template <typename T>
inline void destroy(T* p)
{
    p->~T();
}

template <typename ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
    __destroy(first, last, value_type(first));
}

template <typename ForwardIterator, typename T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
{
    typedef typename _type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
}

template <typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, _false_type)
{
    for (; first != last; ++first)
    {
         destroy(&*first);
    }
}

template <typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, _true_type)
{
}

inline void destroy(char*, char*)
{
}

inline void destroy(wchar_t*, wchar_t*)
{
}

//---------------------------【uninitialized_copy() function】-------------------------------

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, 
                                          ForwardIterator result)
{
    return __uninitialized_copy(first, last, result, value_type(result));
}

template <typename InputIterator, typename ForwardIterator, typename T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, 
                                            ForwardIterator result, T*)
{
    typedef typename _type_traits<T>::is_POD_type is_POD;
    return __uninitialized_copy_aux(first, last, result, is_POD());
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, 
                                                ForwardIterator result, _true_type)
{
    return copy(first, last, result);
}

template <typename InputIterator, typename ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, 
                                                ForwardIterator result, _false_type)
{
    for (; first != last; ++first, ++result)
    {
        construct(&*result, *first);
    }
    return result;
}

inline char* uninitialized_copy(const char* first, const char* last, char* result)
{
    memmove(result, first, last - first);
    return result + (last - first);
}

inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last - first);
}

//---------------------------【uninitialized_fill() function】-------------------------------

template <typename ForwardIterator, typename T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& val)
{
    __uninitialized_fill(first, last, val, value_type(first));
}

template <typename ForwardIterator, typename T, typename T1>
inline void 
__uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& val, T1*)
{
    typedef typename _type_traits<T1>::is_POD_type is_POD;
    __uninitialized_fill_aux(first, last, val, is_POD());
}

template <typename ForwardIterator, typename T>
inline void 
__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& val, _true_type)
{
    fill(first, last, val);
}

template <typename ForwardIterator, typename T>
inline void 
__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& val, _false_type)
{
    while (first != last) 
    {
        construct(&*first, val);
        ++first;
    }
}

//--------------------------【uninitialized_fill_n() function】------------------------------

template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& val)
{
    return __uninitialized_fill_n(first, n, val, value_type(first));
}

template <typename ForwardIterator, typename Size, typename T, typename T1>
inline ForwardIterator 
__uninitialized_fill_n(ForwardIterator first, Size n, const T& val, T1*)
{
    typedef typename _type_traits<T1>::is_POD_type is_POD;
    return __uninitialized_fill_n_aux(first, n, val, is_POD());
}

template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator 
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& val, _true_type)
{
    return fill_n(first, n, val);
}

template <typename ForwardIterator, typename Size, typename T>
inline ForwardIterator 
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& val, _false_type)
{
    while (n > 0) 
    {
        construct(&*first, val);
        --n;
        ++first;
    }
    return first;
}

}

#endif