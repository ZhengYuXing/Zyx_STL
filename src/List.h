#ifndef ZYX_LIST
#define ZYX_LIST 

#include "Iterator.h"
#include "TypeTraits.h"
#include "Alloc.h"
#include "Construct.h"
#include "Algorithm.h"
#include "Utility.h"

namespace Zyx 
{

template <typename T>
struct __list_node
{
    T data;	
    __list_node* prev;
    __list_node* next;
};

template <typename T, typename Ref, typename Ptr>
struct __list_iterator  
{
    typedef __list_iterator<T, Ref, Ptr>              self;
    typedef __list_iterator<T, T&, T*>                iterator;
    typedef __list_iterator<T, const T&, const T*>    const_iterator;

    typedef bidirectional_iterator_tag                iterator_category;
    typedef T                                         value_type;
    typedef Ptr                                       pointer;
    typedef Ref                                       reference;
    typedef size_t                                    size_type;
    typedef ptrdiff_t                                 difference_type;
    typedef __list_node<T>                            list_node;
 
    list_node* node;

    __list_iterator() : node(nullptr) { }
    __list_iterator(list_node* x) : node(x) { }
    __list_iterator(const iterator& x) : node(x.node) { }

    bool operator==(const self& x) const { return node == x.node; }
    bool operator!=(const self& x) const { return node != x.node; }

    reference operator*() const { return node->data; }
    pointer operator->() const { return &(operator*()); }

    self& operator++()
    {
        node = node->next;
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--()
    {
        node = node->prev;
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }
};

template <typename T, typename Alloc = alloc>
class List
{
private:
    typedef __list_node<T> 	                  list_node;
    typedef simple_alloc<list_node, Alloc>    list_node_allocator;

public:
    typedef T                                         value_type;
    typedef __list_iterator<T, T&, T*> 	              iterator;
    typedef __list_iterator<T, const T&, const T*>    const_iterator;
    typedef reverse_iterator<const_iterator>          const_reverse_iterator;
    typedef reverse_iterator<iterator>                reverse_iterator;    
    typedef T*                                        pointer;
    typedef const T*                                  const_pointer;
    typedef T& 	                                      reference;
    typedef const T&                                  const_reference;
    typedef size_t 	                                  size_type;
    typedef ptrdiff_t                                 difference_type;

public:
    List() { empty_intialize(); }

    explicit List(size_type n)
    {
        empty_intialize();
        insert(begin(), n, T());
    }

    List(size_type n, const T& val)
    {
        empty_intialize();
        insert(begin(), n, val);
    }
	
    template <typename InputIterator>
    List(InputIterator first, InputIterator last)
    {
        empty_intialize();
        insert(begin(), first, last);
    }

    List(const List& other)
    {
        empty_intialize();
        insert(begin(), other.begin(), other.end());
    }

    ~List() 
    {
        clear();
        destroy_node(node);
    }

public:
    List& operator=(const List& other)
    {
        if (this != &other)
        {
            iterator first1 = begin();
            iterator last1 = end();
            const_iterator first2 = other.begin();
            const_iterator last2 = other.end();

            while (first1 != last1 && first2 != last2)
            {
                *first1 = *first2;
                ++first1;
                ++first2;
            }

            if (first2 != last2)
            {
                insert(last1, first2, last2);
            }
            else
            {
                erase(first1, last1);
            }
        }

        return *this;
    }

    void assign(size_type n, const T& val) { fill_assign(n, val); }

    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last)
    {
        typedef _is_integer<InputIterator>::integral integral;
        assign_dispatch(first, last, integral());        
    }

public:
    iterator begin() { return node->next; }
    const_iterator begin() const { return node->next; }  

    iterator end() { return node; }
    const_iterator end() const { return node; }   

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
	
    reference back() { return *(--end()); }
    const_reference back() const { return *(--end()); }
	
    bool empty() const { return node->next == node; }
    size_type size() const { return distance(begin(), end()); }

public:
    void push_front(const T& val) { insert(begin(), val); }
    void push_back(const T& val) { insert(end(), val); }

    void pop_front() { erase(begin()); }
    void pop_back() { erase(--end()); }

    iterator insert(iterator pos, const T& val)
    {
        list_node* tmp = create_node(val);
        tmp->next = pos.node;
        tmp->prev = pos.node->prev;
        pos.node->prev->next = tmp;
        pos.node->prev = tmp;
        return tmp;
    }

    void insert(iterator pos, size_type n, const T& val)
    {
        fill_insert(pos, n, val);
    }

    template <typename InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last)
    {
        typedef _is_integer<InputIterator>::integral integral;
        insert_dispatch(pos, first, last, integral());
    }

    iterator erase(iterator pos)
    {
        list_node* prev_node = pos.node->prev;
        list_node* next_node = pos.node->next;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destroy_node(pos.node);
        return next_node;
    }

    iterator erase(iterator first, iterator last)
    {
        while (first != last)
        {
            first = erase(first);
        }
        return last;
    }

    void resize(size_type n) { resize(n, T()); }

    void resize(size_type n, const T& val)
    {
        iterator cur = begin();
        while (cur != end() && n > 0)
        {
            ++cur;
            --n;
        }

        if (n > 0)
        {
            insert(end(), n, val);
        }
        else
        {
            erase(cur, end());
        }
    }

    void clear()
    {
        list_node* cur = node->next;
        while (cur != node)
        {
            list_node* tmp = cur;
            cur = cur->next;
            destroy_node(tmp);
        }
        node->next = node;
        node->prev = node;
    }

    void swap(List& other) { Zyx::swap(node, other.node); }

public:
    void remove(const T& val)
    {
        iterator first = begin();
        iterator last = end();
        while (first != last)
        {
            iterator tmp = first;
            ++tmp;
            if (*first == val)
            {
                erase(first);
            }
            first = tmp;
        }
    }

    template <typename UnaryPredicate>
    void remove_if(UnaryPredicate pred)
    {
        iterator first = begin();
        iterator last = end();
        while (first != last)
        {
            iterator tmp = first;
            ++tmp;
            if (pred(*first))
            {
                erase(first);
            }
            first = tmp;
        }
    }

    void unique()
    {
        iterator first = begin();
        iterator last = end();
        if (first == last)
        {
            return;
        }

        iterator cur = first;
        while (++cur != last)
        {
            if (*cur == *first)
            {
                erase(cur);
            }
            else
            {
                first = cur;                 
            }
            cur = first;
        }
    }

    template <typename BinaryPredicate>
    void unique(BinaryPredicate pred)
    {
        iterator first = begin();
        iterator last = end();
        if (first == last)
        {
            return;
        }

        iterator cur = first;
        while (++cur != last)
        {
            if (pred(*first, *cur))
            {
                erase(cur);
            }
            else
            {
                first = cur;
            }
            cur = first;
        }
    }

    void splice(iterator pos, List& x)
    {
        if (!x.empty())
        {
            transfer(pos, x.begin(), x.end());
        }
    }

    void splice(iterator pos, List& x, iterator i)
    {
        iterator j = i;
        ++j;
        if (pos == i || pos == j)
        {
            return;
        }
        transfer(pos, i, j);
    }

    void splice(iterator pos, List& x, iterator first, iterator last)
    {
        if (first != last)
        {
            transfer(pos, first, last);
        }
    }

    void merge(List& x)
    {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();

        while (first1 != last1 && first2 != last2)
        {
            if (*first2 < *first1)
            {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else
            {
                ++first1;
            }
        }

        if (first2 != last2)
        {
            transfer(last1, first2, last2);
        }
    }

    void reverse()
    {
        if (node->next == node || node->next->next == node)
        {
            return;
        }

        iterator first = begin();
        ++first;
        while (first != end()) 
        {
            iterator old = first;
            ++first;
            transfer(begin(), old, first);
        }

        /* 
        // another way:		
        list_node* prev_node = node->next;
        list_node* curr_node = node->next->next;
        while (curr_node != node) 
        {
            prev_node->next = curr_node->next;
            curr_node->next = node->next;
            node->next = curr_node;
            prev_node->next->prev = prev_node;
            curr_node->next->prev = curr_node;
            curr_node->prev = node;
            curr_node = prev_node->next;
        }
        */
	}

    void sort()
    {
        if (node->next == node || node->next->next == node)
        {
            return;
        }

        List carry;
        List counter[64];
        int fill = 0;
        while (!empty())
        {
            carry.splice(carry.begin(), *this, begin());
            int i = 0;
            while (i < fill && !counter[i].empty())
            {
                counter[i].merge(carry);
                carry.swap(counter[i++]);
            }
            carry.swap(counter[i]);
            if (i == fill)
            {
                ++fill;
            }
        }

        for (int i = 1; i < fill; i++) 
        {
            counter[i].merge(counter[i - 1]);
        }
        swap(counter[fill - 1]);
	}

private:
    list_node* get_node() { return list_node_allocator::allocate(); }	
    void put_node(list_node* p) { list_node_allocator::deallocate(p); }

    list_node* create_node(const T& x)
    {
        list_node* p = get_node();
        construct(&p->data, x);
        return p;
    }

    void destroy_node(list_node* p)
    {
        destroy(&p->data);
        put_node(p);
    }

    void empty_intialize()
    {
        node = get_node();
        node->next = node;
        node->prev = node;
    }

    void fill_assign(size_type n, const T& val)
    {
        iterator cur = begin();
        for (; cur != end() && n > 0; ++cur, --n)
        {
            *cur = val;
        }

        if (n > 0)
        {
            insert(end(), n, val);
        }
        else
        {
            erase(cur, end());
        }
    }

    template <typename Integer>
    void assign_dispatch(Integer n, Integer val, _true_type)
    {
        fill_assign(n, val);
    }

    template <typename InputIterator>
    void assign_dispatch(InputIterator first, InputIterator last, _false_type)
    {
        iterator cur = begin();
        for (; cur != end() && first != last; ++cur, ++first)
        {
            *cur = *first;
        }

        if (first != last)
        {
            insert(end(), first, last);
        }
        else
        {
            erase(cur, end());
        }
    }    

    void fill_insert(iterator pos, size_type n, const T& val)
    {
        for (; n > 0; n--)
        {
            insert(pos, val);
        }
    }

    template <typename Integer>
    void insert_dispatch(iterator pos, Integer n, Integer val, _true_type)
    {
        fill_insert(pos, n, val);
    }

    template <typename InputIterator>
    void insert_dispatch(iterator pos, InputIterator first, InputIterator last, _false_type)
    {
        for (; first != last; ++first)
        {
            insert(pos, *first);
        }
    }

    void transfer(iterator pos, iterator first, iterator last)
    {
        last.node->prev->next = pos.node;
        first.node->prev->next = last.node;
        pos.node->prev->next = first.node;
        list_node* tmp = pos.node->prev;
        pos.node->prev = last.node->prev;
        last.node->prev = first.node->prev;
        first.node->prev = tmp;
    }

private:
    list_node* node;
};


//----------------------------------【non-member functions】----------------------------------

template <typename T, typename Alloc>
inline bool operator==(const List<T, Alloc>& lhs, const List<T, Alloc>& rhs)
{
    return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin()); 

    // typedef typename List<T, Alloc>::const_iterator const_iterator;
    // const_iterator end1 = lhs.end();
    // const_iterator end2 = rhs.end();
    // const_iterator i1 = lhs.begin();
    // const_iterator i2 = rhs.begin();    
    // while (i1 != end1 && i2 != end2 && *i1 == *i1)
    // {
    // 	++i1;
    // 	++i2;
    // }    
    // return i1 == end1 && i2 == end2;
}

template <typename T, typename Alloc>
inline bool operator!=(const List<T, Alloc>& lhs, const List<T, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename T, typename Alloc>
inline bool operator<(const List<T, Alloc>& lhs, const List<T, Alloc>& rhs)
{
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename Alloc>
inline bool operator>(const List<T, Alloc>& lhs, const List<T, Alloc>& rhs)
{
    return rhs < lhs;
}

template <typename T, typename Alloc>
inline bool operator<=(const List<T, Alloc>& lhs, const List<T, Alloc>& rhs)
{
    return !(rhs < lhs);
}

template <typename T, typename Alloc>
inline bool operator>=(const List<T, Alloc>& lhs, const List<T, Alloc>& rhs)
{
    return !(lhs < rhs);
}

template <typename T, typename Alloc>
inline void swap(List<T, Alloc>& x, List<T, Alloc>& y)
{
    x.swap(y);
}

}

#endif