#ifndef ZYX_ALLOCATOR
#define ZYX_ALLOCATOR 

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

namespace Zyx 
{

template <typename T>
T* _allocate(size_t n, T*)
{
    std::set_new_handler(0);
    T* tmp = (T*)(operator new(n * sizeof(T)));
    if (tmp == nullptr)
    {
        std::cerr << "out of memory" << std::endl;
        exit(1);
    }
    return tmp;
}

template <typename T>
void _deallocate(T* p)
{
    operator delete(p);
}

template <typename T1, typename T2>
void _construct(T1* p, const T2& value)
{
    new(p) T1(value);
}

template <typename T>
void _destroy(T* p)
{
    p->~T();
}

template <typename T>
class Allocator
{
public:
    typedef T            value_type;
    typedef T*           pointer;
    typedef const T*     const_pointer;
    typedef T&           reference;
    typedef const T&     const_reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    difference_type;

public:
    template <typename U>
    struct rebind
    {
        typedef Allocator<U> other;
    };

public:
    pointer allocate(size_type n, const void* hint = 0) 
    {
        return _allocate(n, (pointer)0);
    }

    void deallocate(pointer p, size_type n) 
    {
        _deallocate(p);
    }

    void construct(pointer p, const T& value) 
    {
        _construct(p, value);
    }

    void destroy(pointer p) 
    {
        _destroy(p);
    }

    pointer address(reference x) 
    {
        return (pointer)&x;
    }

    const_pointer const_address(const_reference x) 
    {
        return (const_pointer)&x;
    }

    size_type max_size() const 
    {
        return (size_type)(UINT_MAX / sizeof(T));
    }
};

}

#endif