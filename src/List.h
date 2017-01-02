#ifndef ZYX_LIST
#define ZYX_LIST 

#include "Iterator.h"
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
    typedef __list_node<T>*                           link_type;
 
    link_type node;

    __list_iterator() : node(nullptr) { }
    __list_iterator(link_type x) : node(x) { }
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
    typedef T*                                        pointer;
    typedef const T*                                  const_pointer;
    typedef T& 	                                      reference;
    typedef const T&                                  const_reference;
    typedef size_t 	                                  size_type;
    typedef ptrdiff_t                                 difference_type;
    typedef list_node*                                link_type;

public:
    List() { empty_intialize(); }

    // explicit List(size_type n) { default_initialize(n); }

    List(size_type n, const value_type& val)
    {
        empty_intialize();
        fill_initialize(n, val);
    }
	
    template <typename InputIterator>
    List(InputIterator first, InputIterator last)
    {
        empty_intialize();
        while (first != last)
        {
            push_back(*first);
            ++first;
        }
    }

    List(const List& x)
    {
        empty_intialize();
        for (const_iterator iter = x.begin(); iter != x.end(); ++iter)
        {
            push_back(*iter);
        }
    }

    List& operator=(const List& x)
    {
        if (this != &x) 
        {
            List tmp(x);
            swap(tmp);
        }
        return *this;
        
        // if (this != &x) 
        // {
        // 	clear();
        // 	for (const_iterator iter = x.begin(); iter != x.end(); ++iter)
        //     {
        // 	    push_back(*iter);
        //     }
        // }
        // return *this;		
    }

    ~List() 
    {
        clear();
        destroy_node(node);
    }

public:
    iterator begin() { return node->next; }
    const_iterator begin() const { return node->next; }  

    iterator end() { return node; }
    const_iterator end() const { return node; }   
	
    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
	
    reference back() { return *(--end()); }
    const_reference back() const { return *(--end()); }
	
    bool empty() const { return node->next == node; }
    size_type size() const { return distance(begin(), end()); }

public:
    void push_front(const value_type& val) { insert(begin(), val); }
    void push_back(const value_type& val) { insert(end(), val); }

    void pop_front() { erase(begin()); }
    void pop_back() { erase(--end()); }

    iterator insert(iterator pos, const value_type& val)
    {
        link_type tmp = create_node(val);
        tmp->next = pos.node;
        tmp->prev = pos.node->prev;
        pos.node->prev->next = tmp;
        pos.node->prev = tmp;
        return tmp;
    }

    void insert(iterator pos, size_type n, const value_type& val)
    {
        List tmp(n, val);
        splice(pos, tmp);
    }

    template <typename InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last)
    {
        List tmp(first, last);
        splice(pos, tmp);
    }

    iterator erase(iterator pos)
    {
        link_type prev_node = pos.node->prev;
        link_type next_node = pos.node->next;
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

    void clear()
    {
        link_type cur = node->next;
        while (cur != node)
        {
            link_type tmp = cur;
            cur = cur->next;
            destroy_node(tmp);
        }
        node->next = node;
        node->prev = node;
    }

    void swap(List& x) { Zyx::swap(node, x.node); }

public:
    void remove(const value_type& val)
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

    template <typename Predicate>
    void remove_if(Predicate pred)
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
        link_type prev_node = node->next;
        link_type curr_node = node->next->next;
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
            counter[i].merge(counter[i-1]);
        }
        swap(counter[fill-1]);
	}

private:
    link_type get_node() { return list_node_allocator::allocate(); }	
    void put_node(link_type p) { list_node_allocator::deallocate(p); }

    link_type create_node(const T& x)
    {
        link_type p = get_node();
        construct(&p->data, x);
        return p;
    }

    void destroy_node(link_type p)
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

    void default_initialize(size_type n)
    {

    }

    void fill_initialize(size_type n, const value_type& val)
    {
        while (n--)
        {
            push_back(val);
        }
    }

    void transfer(iterator pos, iterator first, iterator last)
    {
        last.node->prev->next = pos.node;
        first.node->prev->next = last.node;
        pos.node->prev->next = first.node;
        link_type tmp = pos.node->prev;
        pos.node->prev = last.node->prev;
        last.node->prev = first.node->prev;
        first.node->prev = tmp;
    }

private:
    link_type node;
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