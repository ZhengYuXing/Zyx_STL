#ifndef ZYX_ALLOC
#define ZYX_ALLOC 

#include <new>
#include <iostream>

namespace Zyx {
	
template <typename T, typename Alloc>
class simple_alloc 
{
public:	
    static T* allocate(size_t n)
    {
        return n == 0 ? nullptr : static_cast<T*>(Alloc::allocate(n * sizeof(T)));
    }

    static T* allocate()
    {
        return static_cast<T*>(Alloc::allocate(sizeof(T)));		
    }

    static void deallocate(T* p, size_t n)
    {
        if (n != 0) 
            Alloc::deallocate(static_cast<void*>(p), n * sizeof(T));
    }

    static void deallocate(T* p)
    {
        Alloc::deallocate(static_cast<void*>(p), sizeof(T));
    }
};

template <int inst>
class __malloc_alloc_template
{
public:
    static void* allocate(size_t n)
    {
        void* result = malloc(n);
        if (result == nullptr)
            result = oom_malloc(n);
        return result;
    }

    static void deallocate(void* p, size_t n)
    {
        free(p);
    }

    static void* reallocate(void* p, size_t old_sz, size_t new_sz)
    {
        void* result = realloc(p, new_sz);
        if (result == nullptr)
            result = oom_realloc(p, new_sz);
        return result;
    }

    static void (*set_malloc_handler(void (*f)()))()
    {
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return old;
    }

private:
    static void* oom_malloc(size_t n)
    {
        void (*my_malloc_handler)();
        void* result;
        while (true) 
        {
            my_malloc_handler = __malloc_alloc_oom_handler;
            if (my_malloc_handler == nullptr) {
                std::cerr << "out of memory" << std::endl;
                exit(1);
            }
            (*my_malloc_handler)();
            result = malloc(n);
            if (result) 
                return result;
        }
    }

    static void* oom_realloc(void* p, size_t n)
    {
        void (*my_malloc_handler)();
        void* result;
        while (true) 
        {
            my_malloc_handler = __malloc_alloc_oom_handler;
            if (my_malloc_handler == nullptr) {
                std::cerr << "out of memory" << std::endl;
                exit(1);
            }
            (*my_malloc_handler)();
            result = realloc(p, n);
            if (result) 
                return result;
        }
    }

    static void (*__malloc_alloc_oom_handler)();
};

template <int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = nullptr;

typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;


template <bool threads, int inst>
class __default_alloc_template
{
private:
    enum { __ALIGN = 8 };
    enum { __MAX_BYTES = 128 };
    enum { __NFREELISTS = __MAX_BYTES / __ALIGN };

private:
    union obj
    {
        union obj* free_list_link;
        char client_data[1];
    };

public:
    static void* allocate(size_t n)
    {
        if (n > __MAX_BYTES)
            return malloc_alloc::allocate(n);

        obj** my_free_list = free_list + FREELIST_INDEX(n);
        obj* result = *my_free_list;
        if (result) {
			
        }
    }

    static void deallocate(void* p, size_t n)
    {

    }

    static void* reallocate(void* p, size_t old_sz, size_t new_sz) 
    {

    }

public:
    static size_t FREELIST_INDEX(size_t bytes)
    {
        return (bytes + __ALIGN - 1) / __ALIGN - 1;
    }

    static size_t ROUND_UP(size_t bytes)
    {
        return (bytes + __ALIGN - 1) & ~(__ALIGN - 1);
    }

    static void* refill(size_t n)
    {

    }

    static char* chunk_alloc(size_t size, int& nobjs)
    {

    }

private:
    static obj* free_list[__NFREELISTS];

private:
    static char* start_free;
    static char* end_free;
    static size_t heap_size;
};

template <bool threads, int inst>
typename __default_alloc_template<threads, inst>::obj* 
__default_alloc_template<threads, inst>::free_list[__NFREELISTS] = 
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

template <bool threads, int inst>
char* __default_alloc_template<threads, inst>::start_free = nullptr;

template <bool threads, int inst>
char* __default_alloc_template<threads, inst>::end_free = nullptr;

template <bool threads, int inst>
char* __default_alloc_template<threads, inst>::heap_size = 0;


}

#endif 