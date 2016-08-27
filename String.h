#ifndef ZYX_STRING
#define ZYX_STRING 

#include <cstring>
#include "TypeTraits.h"
#include "Iterator.h"
#include "Alloc.h"
#include "Construct.h"
#include "Algorithm.h"
#include "Utility.h"

namespace Zyx {

class String
{
public:
    typedef char           value_type;
    typedef char&          reference;
    typedef const char&    const_reference;
    typedef char*          pointer;
    typedef const char*    const_pointer;
    typedef char*          iterator;
    typedef const char*    const_iterator;
    typedef size_t         size_type;
    typedef ptrdiff_t      difference_type;

    typedef reverse_iterator<const_iterator>    const_reverse_iterator;
    typedef reverse_iterator<iterator>          reverse_iterator;

private:
    typedef simple_alloc<value_type, alloc> data_allocator;

public:
    static const size_type npos = static_cast<size_type>(-1);

public:
    String() : start(nullptr), finish(nullptr), end_of_storage(nullptr) 
    {
        allocate_block(8);
        terminate_string();
    }

    String(size_type n) : start(nullptr), finish(nullptr), end_of_storage(nullptr) 
    {
        allocate_block(n + 1);
        terminate_string();
    }

    String(const String& s) : start(nullptr), finish(nullptr), end_of_storage(nullptr)
    {
        range_initialize(s.begin(), s.end());
    }

    String(const String& s, size_type pos, size_type len = npos)
      : start(nullptr), finish(nullptr), end_of_storage(nullptr)
    {
        range_initialize(s.begin() + pos, s.begin() + pos + min(len, s.size() - pos));
    }

    String(const char* s) : start(nullptr), finish(nullptr), end_of_storage(nullptr)
    {
        range_initialize(s, s + strlen(s));
    }

    String(const char* s, size_type n) 
      : start(nullptr), finish(nullptr), end_of_storage(nullptr)
    {
        range_initialize(s, s + n);
    }

    String(size_type n, char c) : start(nullptr), finish(nullptr), end_of_storage(nullptr)
    {
        allocate_block(n + 1);
        finish = uninitialized_fill_n(start, n, c);
        terminate_string();
    }

    template <typename InputIterator>
    String(InputIterator first, InputIterator last)
      : start(nullptr), finish(nullptr), end_of_storage(nullptr)
    {
        typedef typename _is_integer<InputIterator>::integral integral;
        initialize_dispatch(first, last, integral());
    }

    String& operator=(const String& s)
    {
        if (this != &s)
            assign(s.begin(), s.end());
        return *this;
    }

    String& operator=(const char* s) { return assign(s, s + strlen(s)); }

    String& operator=(char c) { return assign(static_cast<size_type>(1), c); }

    ~String() 
    {
        destroy(start, finish + 1);
        deallocate_block(); 
    }

public:
    iterator begin() { return start; }
    const_iterator begin() const { return start; }
    iterator end() { return finish; }
    const_iterator end() const { return finish; }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    reference front() { return *start; }
    const_reference front() const { return *start; }
    reference back() { return *(finish - 1); }
    const_reference back() const { return *(finish - 1); }

    size_type size() const { return	finish - start; }
    size_type length() const { return size(); }
    size_type max_size() const { return static_cast<size_type>(-1) / sizeof(char) - 1; }
    size_type capacity() const { return end_of_storage - start; }
    bool empty() const { return start == finish; }

    const char* c_str() const { return start; }
    const char* data() const { return start; }

public:
    void resize(size_type n) { resize(n, null()); }

    void resize(size_type n, char c)
    {
        if (n <= size())
            erase(begin() + n, end());
        else
            append(n - size(), c);
    }

    void reserve(size_type n = 0)
    {
        size_type len = max(n, size()) + 1;
        char* new_start = allocate(len);
        char* new_finish = uninitialized_copy(start, finish, new_start);
        construct_null(new_finish);
        destroy(start, finish + 1);
        deallocate_block();
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
    }

    void push_back(char c)
    {
        if (finish + 1 == end_of_storage)
            reserve(size() + max(size(), static_cast<size_type>(1)));
        construct_null(finish + 1);
        *finish = c;
        ++finish;
    }

    void pop_back()
    {
        *(finish - 1) = null();
        destroy(finish);
        --finish;
    }

    void clear()
    {
        if (!empty()) {
            *start = null();
            destroy(start + 1, finish + 1);
            finish = start;
        }
    }

    void swap(String& s)
    {
        Zyx::swap(start, s.start);
        Zyx::swap(finish, s.finish);
        Zyx::swap(end_of_storage, s.end_of_storage);
    }

public:
    String& operator+=(const String& s) { return append(s); }
    String& operator+=(const char* s) { return append(s); }
    String& operator+=(char c) { push_back(c); return *this; }

    String& append(const String& s) { return append(s.begin(), s.end()); }

    String& append(const String& s, size_type pos, size_type len)
    {
        return append(s.begin() + pos, s.begin() + pos + min(len, s.size() - pos));
    }

    String& append(const char* s) { return append(s, s + strlen(s)); }

    String& append(const char* s, size_type n) { return append(s, s + n); }  

    String& append(size_type n, char c)
    {
        if (size() + n > capacity())
            reserve(size() + max(size(), n));
        if (n > 0) {
            uninitialized_fill_n(finish + 1, n - 1, c);
            construct_null(finish + n);
            *finish = c;
            finish += n;
        }
        return *this;
    }

    template <typename InputIterator>
    String& append(InputIterator first, InputIterator last)
    {
        typedef typename _is_integer<InputIterator>::integral integral;
        return append_dispatch(first, last, integral());        
    }

public:
    String& assign(const String& s) { return assign(s.begin(), s.end()); }

    String& assign(const String& s, size_type pos, size_type len)
    {
        return assign(s.begin() + pos, s.begin() + pos + min(len, s.size() - pos));
    }   

    String& assign(const char* s) { return assign(s, s + strlen(s)); }

    String& assign(const char* s, size_type n) { return assign(s, s + n); }

    String& assign(size_type n, char c)
    {
        if (n <= size()) {
            fill_n(start, n, c);
            erase(start + n, finish);        
        } else {
            fill_n(start, size(), c);
            append(n - size(), c);
        }
        return *this;
    }

    template <typename InputIterator>
    String& assign(InputIterator first, InputIterator last)
    {
        typedef typename _is_integer<InputIterator>::integral integral;
        return assign_dispatch(first, last, integral());
    }

public:
    String& insert(size_type pos, const String& s)
    {
        insert(start + pos, s.begin(), s.end());
        return *this;
    }

    String& insert(size_type pos, const String& s, size_type subpos, size_type sublen)
    {
        size_type len = min(sublen, s.size() - subpos);
        insert(start + pos, s.begin() + subpos, s.begin() + subpos + len);
        return *this;
    }

    String& insert(size_type pos, const char* s)
    {
        insert(start + pos, s, s + strlen(s));
        return *this;
    }

    String& insert(size_type pos, const char* s, size_type n)
    {
        insert(start + pos, s, s + n);
        return *this;
    }

    String& insert(size_type pos, size_type n, char c)
    {
        insert(start + pos, n, c);
        return *this;
    }

    iterator insert(iterator p, char c)
    {
        if (p == finish) {
            push_back(c);
            return finish - 1;
        } else {
            return insert_aux(p, c);
        }
    }

    void insert(iterator p, size_type n, char c)
    {
        if (n != 0) {
            if (end_of_storage - finish >= n + 1) {
                const size_type elems_after = finish - p;
                iterator old_finish = finish;
                if (elems_after >= n) {
                    uninitialized_copy((finish - n) + 1, finish + 1, finish + 1);
                    finish += n;
                    memmove(p + n, p, elems_after - n + 1);
                    fill_n(p, n, c);
                } else {
                    uninitialized_fill_n(finish + 1, n - elems_after - 1, c);
                    finish += n - elems_after;
                    uninitialized_copy(p, old_finish + 1, finish);
                    finish += elems_after;
                    fill_n(p, elems_after + 1, c);
                }
            } else {
                const size_type old_size = size();
                const size_type len = old_size + max(old_size, n) + 1;
                iterator new_start = allocate(len);
                iterator new_finish = uninitialized_copy(start, p, new_start);
                new_finish = uninitialized_fill_n(new_finish, n, c);
                new_finish = uninitialized_copy(p, finish, new_finish);
                construct_null(new_finish);
                destroy(start, finish + 1);
                deallocate_block();
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;
            }
        }
    }

    template <typename InputIterator>
    void insert(iterator p, InputIterator first, InputIterator last)
    {
        typedef typename _is_integer<InputIterator>::integral integral;
        insert_dispatch(p, first, last, integral());
    }

public:
    String& erase(size_type pos = 0, size_type len = npos)
    {
        erase(start + pos, start + pos + min(len, size() - pos));
        return *this;
    }

    iterator erase(iterator pos)
    {
        memmove(pos, pos + 1, finish - pos);
        destroy(finish);
        --finish;
        return pos;
    }

    iterator erase(iterator first, iterator last)
    {
        if (first != last) {
            memmove(first, last, (finish - last) + 1);
            const iterator new_finish = finish - (last - first);
            destroy(new_finish + 1, finish + 1);
            finish = new_finish;
        }
        return first;
    }

private:
    char* allocate(size_type n) { return data_allocator::allocate(n); }

    void deallocate(char* p, size_type n) 
    { 
        if (p != nullptr)
            data_allocator::deallocate(p, n); 
    }

    void allocate_block(size_type n)
    {
        if (n <= max_size()) {
            start = allocate(n);
            finish = start;
            end_of_storage = start + n;
        }
    }

    void deallocate_block() { deallocate(start, end_of_storage - start); }

private:
    void construct_null(char* p) { construct(p, '\0'); }
    void terminate_string() { construct_null(finish); }
    static char null() { return '\0'; }

private:
    template <typename InputIterator>
    void range_initialize(InputIterator first, InputIterator last)
    {
        range_initialize(first, last, iterator_category(first));
    }

    template <typename InputIterator>
    void range_initialize(InputIterator first, InputIterator last, input_iterator_tag)
    {
        allocate_block(8);
        construct_null(finish);
        append(first, last);
    }

    template <typename ForwardIterator>
    void range_initialize(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
    {
        difference_type n = distance(first, last);
        allocate_block(n + 1);
        finish = uninitialized_copy(first, last, start);
        terminate_string();
    }

    template <typename Integer>
    void initialize_dispatch(Integer n, Integer x, _true_type)
    {
        allocate_block(n + 1);
        finish = uninitialized_fill_n(start, n, x);
        terminate_string();
    }

    template <typename InputIterator>
    void initialize_dispatch(InputIterator first, InputIterator last, _false_type)
    {
        range_initialize(first, last);
    }

    template <typename Integer>
    String& append_dispatch(Integer n, Integer x, _true_type)
    {
        return append(static_cast<size_type>(n), static_cast<char>(x));
    }

    template <typename InputIterator>
    String& append_dispatch(InputIterator first, InputIterator last, _false_type)
    {
        return append(first, last, iterator_category(first));
    }

    template <typename InputIterator>
    String& append(InputIterator first, InputIterator last, input_iterator_tag)
    {
        for (; first != last; ++first)
            push_back(*first);
        return *this;
    }

    template <typename ForwardIterator>
    String& append(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
    {
        if (first != last) {
            const size_type old_size = size();
            difference_type n = distance(first, last);
            if (old_size + n > capacity()) {
                const size_type len = old_size + max(old_size, static_cast<size_type>(n)) + 1;
                char* new_start = allocate(len);
                char* new_finish = uninitialized_copy(start, finish, new_start);
                new_finish = uninitialized_copy(first, last, new_finish);
                construct_null(new_finish);
                destroy(start, finish + 1);
                deallocate_block();
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;
            } else {
                ForwardIterator f = first;
                ++f;
                uninitialized_copy(f, last, finish + 1);
                construct_null(finish + n);
                *finish = *first;
                finish += n;
            }
        }
        return *this;
    }

    template <typename Integer>
    String& assign_dispatch(Integer n, Integer x, _true_type)
    {
        return assign(static_cast<size_type>(n), static_cast<char>(x));
    }

    template <typename InputIterator>
    String& assign_dispatch(InputIterator first, InputIterator last, _false_type)
    {
        char* cur = start;
        while (cur != finish && first != last) {
            *cur = *first;
            ++cur;
            ++first;
        }
        if (first == last)
            erase(cur, finish);
        else
            append(first, last);       
        return *this;
    }

    iterator insert_aux(iterator p, char c)
    {
        iterator new_pos = p;
        if (finish + 1 < end_of_storage) {
            construct_null(finish + 1);
            memmove(p + 1, p, finish - p);
            *p = c;
            ++finish;
        } else {
            const size_type old_len = size();
            const size_type len = old_len + max(old_len, static_cast<size_type>(1)) + 1;
            iterator new_start = allocate(len);
            iterator new_finish = new_start;
            new_pos = uninitialized_copy(start, p, new_start);
            construct(new_pos, c);
            new_finish = new_pos + 1;
            new_finish = uninitialized_copy(p, finish, new_finish);
            construct_null(new_finish);
            destroy(start, finish + 1);
            deallocate_block();
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
        return new_pos;
    }

    template <typename Integer>
    void insert_dispatch(iterator p, Integer n, Integer x, _true_type)
    {
        return insert(p, static_cast<size_type>(n), static_cast<size_type>(x));
    }

    template <typename InputIterator>
    void insert_dispatch(iterator p, InputIterator first, InputIterator last, _false_type)
    {
        insert(p, first, last, iterator_category(first));
    }

    template <typename InputIterator>
    void insert(iterator p, InputIterator first, InputIterator last, input_iterator_tag)
    {
        for (; first != last; ++first) {
            p = insert(p, *first);
            ++p;
        }
    }

    template <typename ForwardIterator>
    void insert(iterator p, ForwardIterator first, ForwardIterator last, forward_iterator_tag)
    {
        if (first != last) {
            difference_type n = distance(first, last);
            if (end_of_storage - finish >= n + 1) {
                const size_type elems_after = finish - p;
                iterator old_finish = finish;
                if (elems_after >= n) {
                    uninitialized_copy((finish - n) + 1, finish + 1, finish + 1);
                    finish += n;
                    memmove(p + n, p, elems_after - n + 1);
                    copy(first, last, p);
                } else {
                    ForwardIterator mid = first;
                    advance(mid, elems_after + 1);
                    uninitialized_copy(mid, last, finish + 1);
                    finish += n - elems_after;
                    uninitialized_copy(p, old_finish + 1, finish);
                    finish += elems_after;
                    copy(first, mid, p);
                }
            } else {
                const size_type old_size = size();
                const size_type len = old_size + max(old_size, static_cast<size_type>(n)) + 1;
                iterator new_start = allocate(len);
                iterator new_finish = uninitialized_copy(start, p, new_start);
                new_finish = uninitialized_copy(first, last, new_finish);
                new_finish = uninitialized_copy(p, finish, new_finish);
                construct_null(new_finish);
                destroy(start, finish + 1);
                deallocate_block();
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;
            }
        }
    }

private:
    char* start;
    char* finish;
    char* end_of_storage;
};

}

#endif