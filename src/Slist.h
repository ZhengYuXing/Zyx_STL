#ifndef ZYX_SLIST
#define ZYX_SLIST 

#include "Iterator.h"
#include "Alloc.h"
#include "Construct.h"

namespace Zyx {

struct __slist_node_base
{
    __slist_node_base* next;
};

template <typename T>
struct __slist_node : public __slist_node_base
{
    T data;
};

__slist_node_base* __slist_make_link(__slist_node_base* prev_node, __slist_node_base* new_node)
{
    new_node->next = prev_node->next;
    prev_node->next = new_node;
    return new_node;
}

size_t __slist_size(__slist_node_base* node)
{
    size_t result = 0;
    for (; node != nullptr; node = node->next)
        ++result;
    return result;
}

struct __slist_iterator_base
{
    __slist_node_base* node;

    __slist_iterator_base(__slist_node_base* x) : node(x) { }

    void incr() { node = node->next; }

    bool operator==(const __slist_iterator_base& x) const { return node == x.node; }
    bool operator!=(const __slist_iterator_base& x) const { return node != x.node; }
};

template <typename T, typename Ref, typename Ptr>
struct __slist_iterator : public __slist_iterator_base
{
    typedef __slist_iterator<T, T&, T*>                iterator;
    typedef __slist_iterator<T, const T&, const T*>    const_iterator;
    typedef __slist_iterator<T, Ref, Ptr>              self;

    typedef forward_iterator_tag                       iterator_category;
    typedef T                                          value_type;
    typedef Ptr                                        pointer;
    typedef Ref                                        reference;
    typedef size_t                                     size_type;
    typedef ptrdiff_t                                  difference_type;
    typedef __slist_node<T>                            list_node;

    __slist_iterator() : __slist_iterator_base(nullptr) { }
    __slist_iterator(list_node* x) : __slist_iterator_base(x) { }
    __slist_iterator(const iterator& x) : __slist_iterator_base(x.node) { }

    reference operator*() const { return ((list_node*)node)->data; }
    pointer operator->() const { return &(operator*()); }

    self& operator++()
    {
        incr();
        return *this;
	}

    self operator++(int)
    {
        self tmp = *this;
        incr();
        return tmp;
    }
};

template <typename T, typename Alloc = alloc>
class Slist
{
public:
    typedef T                                          value_type;
    typedef __slist_iterator<T, T&, T*>                iterator;
    typedef __slist_iterator<T, const T&, const T*>    const_iterator;
    typedef T*                                         pointer;
    typedef const T*                                   const_pointer;
    typedef T&                                         reference;
    typedef const T&                                   const_reference;
    typedef size_t                                     size_type;
    typedef ptrdiff_t                                  difference_type;	

private:
    typedef __slist_node<T>                   list_node;
    typedef __slist_node_base                 list_node_base;
    typedef __slist_iterator_base             iterator_base;
    typedef simple_alloc<list_node, Alloc>    list_node_allocator;

public:
    Slist() { head.next = nullptr; }
    ~Slist() { clear(); }

public:
    iterator before_begin() { return iterator((list_node*)&head); }
    const_iterator before_begin() const { return iterator((list_node*)&head); }

    iterator begin() { return iterator((list_node*)head.next); }
    const_iterator begin() const { return const_iterator((list_node*)head.next); }

    iterator end() { return iterator(nullptr); }
    const_iterator end() const { return const_iterator(nullptr); }

    size_type size() const { return __slist_size(head.next); }
    bool empty() const { return head.next == nullptr; }

    reference front() { return ((list_node*)head.next)->data; }
    const_reference front() const { return ((list_node*)head.next)->data; }

public:
    void push_front(const T& val) { __slist_make_link(&head, create_node(val)); }

    void pop_front()
    {
        list_node* node =(list_node*)head.next;
        head.next = node->next;
        destroy_node(node);
    }

    iterator insert_after(iterator pos, const T& val)
    {
        __slist_make_link(pos.node, create_node(val));
        return ++pos;
    }

    iterator erase_after(iterator pos)
    {
        list_node_base* node = pos.node;
        list_node* tmp = (list_node*)node->next;
        node->next = tmp->next;
        destroy_node(tmp);
        return ++pos;
    }

    void clear()
    {
        while (!empty()) 
            pop_front();
    }

private:
    static list_node* create_node(const T& x)
    {
        list_node* p = list_node_allocator::allocate();
        construct(&p->data, x);
        p->next = nullptr;
        return p;
    }

    static void destroy_node(list_node* p)
    {
        destroy(&p->data);
        list_node_allocator::deallocate(p);
    }

private:
    list_node_base head;
};

}

#endif