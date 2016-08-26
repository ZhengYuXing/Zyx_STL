#ifndef ZYX_ALLOC
#define ZYX_ALLOC 

#include <new>
#include <iostream>

namespace Zyx {

//-----------------------------------【simple_alloc class】-----------------------------------

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


//-----------------------------【__malloc_alloc_template class】------------------------------

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
// typedef malloc_alloc alloc;


//-----------------------------【__default_alloc_template class】-----------------------------

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
        if (result == nullptr) {
            void* r = refill(ROUND_UP(n));
            return r;
        }
        *my_free_list = result->free_list_link;
        return result;
    }

    static void deallocate(void* p, size_t n)
    {
        if (n > __MAX_BYTES) {
            malloc_alloc::deallocate(p, n);
            return;
        }
        obj* q = static_cast<obj*>(p);
        obj** my_free_list = free_list + FREELIST_INDEX(n);
        q->free_list_link = *my_free_list;
        *my_free_list = q;
    }

    static void* reallocate(void* p, size_t old_sz, size_t new_sz) 
    {
        deallocate(p, old_sz);
        p = allocate(new_sz);
        return p;
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
        int nobjs = 20;
        char* chunk = chunk_alloc(n, nobjs);

        if (nobjs == 1)
            return chunk;

        obj** my_free_list = free_list + FREELIST_INDEX(n);
        obj* result = reinterpret_cast<obj*>(chunk);
        obj* current_obj = nullptr;
        obj* next_obj = reinterpret_cast<obj*>(chunk + n);
        *my_free_list = next_obj;
        
        for (int i = 1; ; i++) {
            current_obj = next_obj;
            next_obj = reinterpret_cast<obj*>(reinterpret_cast<char*>(next_obj) + n);
            if (nobjs - 1 == i) {
                current_obj->free_list_link = nullptr;
                break;
            } else {
                current_obj->free_list_link = next_obj;
            }
        }
        return result;
    }

    static char* chunk_alloc(size_t size, int& nobjs)
    {
        char* result = nullptr;
        size_t total_bytes = size * nobjs;
        size_t bytes_left = end_free - start_free;

        if (bytes_left >= total_bytes) {
            result = start_free;
            start_free += total_bytes;
            return result;
        } else if (bytes_left >= size) {
            nobjs = bytes_left / size;
            total_bytes = size * nobjs;
            result = start_free;
            start_free += total_bytes;
            return result;
        } else {
            if (bytes_left > 0) {
                obj** my_free_list = free_list + FREELIST_INDEX(bytes_left);
                reinterpret_cast<obj*>(start_free)->free_list_link = *my_free_list;
                *my_free_list = reinterpret_cast<obj*>(start_free);
            }

            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
            start_free = static_cast<char*>(malloc(bytes_to_get));
            if (start_free == nullptr) {
                obj** my_free_list = nullptr;
                obj* p = nullptr;
                for (int i = size; i <= __MAX_BYTES; i += __ALIGN) {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if (p != nullptr) {
                        *my_free_list = p->free_list_link;
                        start_free = reinterpret_cast<char*>(p);
                        end_free = start_free + i;
                        return chunk_alloc(size, nobjs);
                    }
                }
                end_free = nullptr;
                start_free = static_cast<char*>(malloc_alloc::allocate(bytes_to_get));
            }

            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(size, nobjs);
        }
    }

private:
    static char* start_free;
    static char* end_free;
    static size_t heap_size;

private:
    static obj* free_list[__NFREELISTS];
};

template <bool threads, int inst>
char* __default_alloc_template<threads, inst>::start_free = nullptr;

template <bool threads, int inst>
char* __default_alloc_template<threads, inst>::end_free = nullptr;

template <bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

template <bool threads, int inst>
typename __default_alloc_template<threads, inst>::obj*
__default_alloc_template<threads, inst>::free_list[__NFREELISTS] = 
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

typedef __default_alloc_template<false, 0> alloc;

}

#endif 