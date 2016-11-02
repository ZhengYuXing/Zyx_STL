#ifndef ZYX_VECTOR
#define ZYX_VECTOR 

#include "Iterator.h"
#include "Alloc.h"
#include "Construct.h"
#include "Algorithm.h"
#include "Utility.h"

namespace Zyx {

template <typename T, typename Alloc = alloc>
class Vector
{
public:
    typedef T            value_type;
    typedef T* 	         iterator;
    typedef const T*     const_iterator;
    typedef T*           pointer;
    typedef const T*     const_pointer;
    typedef T&           reference;
    typedef const T&     const_reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    difference_type;

    typedef reverse_iterator<const_iterator>    const_reverse_iterator;
    typedef reverse_iterator<iterator>          reverse_iterator;

private:	
    typedef simple_alloc<value_type, Alloc> data_allocator;

public:
    Vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) 
    { 
    }

    Vector(size_type n, const T& val) 
      : start(nullptr), finish(nullptr), end_of_storage(nullptr)
    {
        start = allocate(n);
        finish = uninitialized_fill_n(start, n, val);
        end_of_storage = start + n;
    }

    explicit Vector(size_type n) 
      : start(nullptr), finish(nullptr), end_of_storage(nullptr)
    {
        start = allocate(n);
        finish = uninitialized_fill_n(start, n, T());
        end_of_storage = start + n;       
    }

    template <typename InputIterator>
    Vector(InputIterator first, InputIterator last) 
      : start(nullptr), finish(nullptr), end_of_storage(nullptr)
    {
        const size_type n = distance(first, last);
        start = allocate(n);
        finish = uninitialized_copy(first, last, start);
        end_of_storage = start + n;        
    }

    Vector(const Vector& x) 
      : start(nullptr), finish(nullptr), end_of_storage(nullptr)
    {
        const size_type n = x.size();
        start = allocate(n);
        finish = uninitialized_copy(x.begin(), x.end(), start);
        end_of_storage = start + n;
    }

    Vector& operator=(const Vector& x)
    {
        Vector(x).swap(*this);
        return *this;

        // if (this != &x) {
        //     Vector tmp(x);
        //     swap(tmp);
        // }
        // return *this;

        // another way:
        // if (this != &x) {
        //     const size_type n = x.size();
        //     if (n > capacity()) {
        //         iterator tmp = allocate_and_copy(n, x.begin(), x.end());
        //         destroy(start, finish);
        //         deallocate(start, end_of_storage - start);
        //         start = tmp;
        //         end_of_storage = start + n;
        //     } else if (size() >= n) {
        //         iterator tmp = copy(x.begin(), x.end(), start);
        //         destroy(tmp, finish);
        //     } else {
        //         copy(x.begin(), x.begin() + size(), start);
        //         uninitialized_copy(x.begin() + size(), x.end(), finish);
        //     }
        //     finish = start + n;
        // }
        // return *this;
    }

    ~Vector()
    {
        destroy(start, finish);
        deallocate(start, end_of_storage - start);
    }

public:
    iterator begin() { return start;  }
    const_iterator begin() const { return start; }
    iterator end() { return finish;  }
    const_iterator end() const { return finish; }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    reference front() { return *start; }
    const_reference front() const { return *start; }
    reference back() { return *(finish - 1); }
    const_reference back() const { return *(finish - 1); }

    reference operator[](size_type i) { return *(start + i); }
    const_reference operator[](size_type i) const { return *(start + i); }   
     
    size_type size() const  { return finish - start;  }
    size_type max_size() const { return size_type(-1) / sizeof(T); }
    size_type capacity() const { return end_of_storage - start; }
    bool empty() const { return start == finish; }

public:
    void push_back(const T& val)
    {
        if (finish != end_of_storage) {
            construct(finish, val);
            ++finish;
        } else {
            insert_aux(finish, val);
        }
    }

    void pop_back()
    {
        --finish;
        destroy(finish);
    }

    iterator insert(iterator pos, const T& val)
    {
        const size_type n = pos - start;
        if (pos == finish && finish != end_of_storage) {
            construct(finish, val);
            ++finish;
        } else {
            insert_aux(finish, val);
        }
        return start + n;
    }

    void insert(iterator pos, size_type n, const T& val) 
    { 
        fill_insert(pos, n, val); 
    }

    template <typename InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last)
    {
        range_insert(pos, first, last);
    }

    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last)
    {
        const size_type n = last - first;
        if (end_of_storage - start >= n) {
            iterator result = uninitialized_copy(first, last, start);
            if (result < finish)
                destroy(result, finish);
            finish = result;
        } else {
            destroy(start, finish);
            deallocate(start, end_of_storage - start);            
            start = allocate(n);
            finish = uninitialized_copy(first, last, start);
            end_of_storage = start + n;            
        }
    }

    void assign(size_type n, const T& val)
    {
        if (end_of_storage - start >= n) {
            iterator result = uninitialized_fill_n(start, n, val);
            if (result < finish)
                destroy(result, finish);
            finish = result;
        } else {
            destroy(start, finish);
            deallocate(start, end_of_storage - start);
            start = allocate(n);
            finish = uninitialized_fill_n(start, n, val);
            end_of_storage = start + n;
        }
    }

    iterator erase(iterator pos)
    {
        if (pos + 1 != finish)
            copy(pos + 1, finish, pos);
        --finish;
        destroy(finish);
        return pos;
    }

    iterator erase(iterator first, iterator last)
    {
        iterator result = copy(last, finish, first);
        destroy(result, finish);
        finish = finish - (last - first);
        return first;
    }

    void resize(size_type n, const T& val)
    {
        if (n < size())
            erase(start + n, finish);
        else
            insert(finish, n - size(), val);
    }

    void resize(size_type n) 
    { 
        resize(n, T()); 
    }

    void reserve(size_type n)
    {
        if (capacity() < n) {
            const size_type old_size = size();
            iterator tmp = allocate_and_copy(n, start, finish);
            destroy(start, finish);
            deallocate(start, end_of_storage - start); 
            start = tmp;
            finish = start + old_size;
            end_of_storage = start + n;
        }
    }

    void clear() 
    { 
        erase(start, finish); 
    }

    void swap(Vector& x)
    {
        Zyx::swap(start, x.start);
        Zyx::swap(finish, x.finish);
        Zyx::swap(end_of_storage, x.end_of_storage);
    }

private:
    iterator allocate(size_type n) { return data_allocator::allocate(n); }
    void deallocate(iterator p, size_type n) { data_allocator::deallocate(p, n); }

    template <typename ForwardIterator>
    iterator allocate_and_copy(size_type n, ForwardIterator first, ForwardIterator last)
    {
        iterator result = allocate(n);
        uninitialized_copy(first, last, result);
        return result;
    }

    void insert_aux(iterator pos, const T& val)
    {
        if (finish != end_of_storage) {
            construct(finish, *(finish - 1));
            ++finish;
            copy_backward(pos, finish - 2, finish - 1);
            *pos = val;
        } else {
            const size_type old_size = size();
            const size_type len = old_size != 0 ? 2 * old_size : 1;
            iterator new_start = allocate(len);
            iterator new_finish = new_start;
            try {
                new_finish = uninitialized_copy(start, pos, new_start);
                construct(new_finish, val);
                ++new_finish;
                new_finish = uninitialized_copy(pos, finish, new_finish);
            } catch (...) {
                destroy(new_start, new_finish);
                deallocate(new_start, len);
                throw;
            }
            destroy(start, finish);
            deallocate(start, end_of_storage - start);
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }

    void fill_insert(iterator pos, size_type n, const T& val)
    {
        if (n != 0) {
            if (end_of_storage - finish >= n) {
                const size_type elems_after = finish - pos; 
                iterator old_finish = finish;
                if (elems_after > n) {
                    uninitialized_copy(finish - n, finish, finish);
                    finish += n;
                    copy_backward(pos, old_finish - n, old_finish);
                    fill(pos, pos + n, val);
                } else {
                    uninitialized_fill_n(finish, n - elems_after, val);
                    finish += n - elems_after;
                    uninitialized_copy(pos, old_finish, finish);
                    finish += elems_after;
                    fill(pos, old_finish, val);
                }
            } else {
                const size_type old_size = size();
                const size_type len = old_size + max(old_size, n);
                iterator new_start = allocate(len);
                iterator new_finish = new_start;
                try {
                    new_finish = uninitialized_copy(start, pos, new_start);
                    new_finish = uninitialized_fill_n(new_finish, n, val);
                    new_finish = uninitialized_copy(pos, finish, new_finish);
                } catch (...) {
                    destroy(new_start, new_finish);
                    deallocate(new_start, len);
                    throw;
                }
                destroy(start, finish);
                deallocate(start, end_of_storage - start);
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;
            }
        }
    }

    // template <typename InputIterator>
    // void range_insert(iterator pos, InputIterator first, InputIterator last)
    // {
    //     for (; first != last; ++first) {
    //         pos = insert(pos, *first);
    //         ++pos;
    //     }
    // }

    template <typename ForwardIterator>
    void range_insert(iterator pos, ForwardIterator first, ForwardIterator last)
    {
        if (first != last) {
            size_type n = distance(first, last);
            if (end_of_storage - finish >= n) {
                const size_type elems_after = finish - pos;
                iterator old_finish = finish;
                if (elems_after > n) {
                    uninitialized_copy(finish - n, finish, finish);
                    finish += n;
                    copy_backward(pos, old_finish - n, old_finish);
                    copy(first, last, pos);
                } else {
                    iterator mid = first;
                    advance(mid, elems_after);
                    uninitialized_copy(mid, last, finish);
                    finish += n - elems_after;
                    uninitialized_copy(pos, old_finish, finish);
                    finish += elems_after;
                    copy(first, mid, pos);
                }
            } else {
                const size_type old_size = size();
                const size_type len = old_size + max(old_size, n);
                iterator new_start = allocate(len);
                iterator new_finish = new_start;
                try {
                    new_finish = uninitialized_copy(start, pos, new_start);
                    new_finish = uninitialized_copy(first, last, new_finish);
                    new_finish = uninitialized_copy(pos, finish, new_finish);
                } catch(...) {
                    destroy(new_start, new_finish);
                    deallocate(new_start, len);
                    throw;
                }
                destroy(start, finish);
                deallocate(start, end_of_storage - start);
                start = new_start;
                finish = new_finish;
                end_of_storage = start + len;
            }            
        }
    }

private:
    iterator start;
    iterator finish;
    iterator end_of_storage;
};

template <typename T, typename Alloc>
inline bool operator==(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
    return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin()); 

    // typedef typename Vector<T, Alloc>::const_iterator const_iterator;
    // const_iterator end1 = lhs.end();
    // const_iterator end2 = rhs.end();
    // const_iterator i1 = lhs.begin();
    // const_iterator i2 = rhs.begin();    
    // while (i1 != end1 && i2 != end2 && *i1 == *i1) {
    //     ++i1;
    //     ++i2;
    // }    
    // return i1 == end1 && i2 == end2;
}

template <typename T, typename Alloc>
inline bool operator!=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename T, typename Alloc>
inline bool operator<(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename Alloc>
inline bool operator>(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
    return rhs < lhs;
}

template <typename T, typename Alloc>
inline bool operator<=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
    return !(rhs < lhs);
}

template <typename T, typename Alloc>
inline bool operator>=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
    return !(lhs < rhs);
}

template <typename T, typename Alloc>
inline void swap(Vector<T, Alloc>& x, Vector<T, Alloc>& y)
{
    x.swap(y);
}

}

#endif