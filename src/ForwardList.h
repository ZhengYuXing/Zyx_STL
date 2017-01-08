#ifndef ZYX_FORWARD_LIST
#define ZYX_FORWARD_LIST

#include "Iterator.h"
#include "TypeTraits.h"
#include "Alloc.h"
#include "Construct.h"
#include "Utility.h"

namespace Zyx
{

template <typename T>
struct __forward_list_node
{
    T data;
    __forward_list_node<T>* next;
};

template <typename T, typename Ref, typename Ptr>
struct __forward_list_iterator
{
    typedef __forward_list_iterator<T, Ref, Ptr>              self;
    typedef __forward_list_iterator<T, T&, T*>                iterator;
    typedef __forward_list_iterator<T, const T&, const T*>    const_iterator;

    typedef forward_iterator_tag                              iterator_category;
    typedef T                                                 value_type;
    typedef Ptr                                               pointer;
    typedef Ref                                               reference;
    typedef size_t                                            size_type;
    typedef ptrdiff_t                                         difference_type;
    typedef __forward_list_node<T>*                           link_type;

    link_type node;

    __forward_list_iterator() : node(nullptr) { }
    __forward_list_iterator(link_type x) : node(x) { }
    __forward_list_iterator(const iterator& x) : node(x.node) { }

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
        node = node->next;
        return tmp;
    }
};

template <typename T, typename Alloc = alloc>
class ForwardList
{
public:
    typedef T                                                 value_type;
    typedef __forward_list_iterator<T, T&, T*>                iterator;
    typedef __forward_list_iterator<T, const T&, const T*>    const_iterator;
    typedef T*                                                pointer;
    typedef const T*                                          const_pointer;
    typedef T&                                                reference;
    typedef const T&                                          const_reference;
    typedef size_t                                            size_type;
    typedef ptrdiff_t                                         difference_type;

private:
    typedef __forward_list_node<T>            list_node;
    typedef simple_alloc<list_node, Alloc>    list_node_allocator;

public:
    ForwardList();

    explicit ForwardList(size_type count);

    ForwardList(size_type count, const T& value);

    template <typename InputIterator>
    ForwardList(InputIterator first, InputIterator last);

    ForwardList(const ForwardList& other);

    ~ForwardList();

public:
    ForwardList& operator=(const ForwardList& other);

    void assign(size_type count, const T& value);

    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last);

public:
    reference front();
    const_reference front() const;

    iterator before_begin();
    const_iterator before_begin() const;

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    size_type size() const;
    bool empty() const;

public:
    void push_front(const T& value);
    void pop_front();

    iterator insert_after(const_iterator pos, const T& value);
    iterator insert_after(const_iterator pos, size_type count, const T& value);

    template <typename InputIterator>
    iterator insert_after(const_iterator pos, InputIterator first, InputIterator last);

    iterator erase_after(const_iterator pos);
    iterator erase_after(const_iterator first, const_iterator last);

    void resize(size_type count);
    void resize(size_type count, const T& value);

    void clear();

    void swap(ForwardList& other);

public:
    void remove(const T& value);

    template <typename UnaryPredicate>
    void remove_if(UnaryPredicate pred);

    void unique();

    template <typename BinaryPredicate>
    void unique(BinaryPredicate pred);

    void splice_after(const_iterator pos, ForwardList& other);
    void splice_after(const_iterator pos, ForwardList& other, const_iterator it);
    void splice_after(const_iterator pos, ForwardList& other, const_iterator first, const_iterator last);

    void merge(ForwardList& other);

    template <typename Compare>
    void  merge(ForwardList& other, Compare comp);

    void sort();

    template <typename Compare>
    void sort(Compare comp);

    void reverse();

private:
    list_node* create_node(const T& val);
    void destroy_node(list_node* p);

    list_node* make_link(list_node* prev_node, list_node* next_node);

    list_node* previous(list_node* pos, list_node* node);

    void fill_assign(size_type n, const T& val);

    template <typename Integer>
    void assign_dispatch(Integer n, Integer val, _true_type);

    template <typename InputIterator>
    void assign_dispatch(InputIterator first, InputIterator last, _false_type);

    list_node* insert_after(list_node* pos, const T& val);
    list_node* insert_after_fill(list_node* pos, size_type n, const T& val);

    template <typename InputIterator>
    list_node* insert_after_range(list_node* pos, InputIterator first, InputIterator last);

    template <typename Integer>
    list_node* insert_after_range(list_node* pos, Integer n, Integer val, _true_type);

    template <typename InputIterator>
    list_node* insert_after_range(list_node* pos, InputIterator first, InputIterator last, _false_type);

    list_node* erase_after(list_node* pos);
    list_node* erase_after(list_node* before_first, list_node* last);

    void splice_after(list_node* pos, list_node* before_first, list_node* before_last);

private:
    list_node head;
};


//------------------------------------【public functions】------------------------------------

template <typename T, typename Alloc>
ForwardList<T, Alloc>::ForwardList()
{ 
    head.next = nullptr;
}

template <typename T, typename Alloc>
ForwardList<T, Alloc>::ForwardList(size_type count)
{
    head.next = nullptr;
    insert_after_fill(&head, count, T());
}

template <typename T, typename Alloc>
ForwardList<T, Alloc>::ForwardList(size_type count, const T& value)
{
    head.next = nullptr;
    insert_after_fill(&head, count, value);
}

template <typename T, typename Alloc>
template <typename InputIterator>
ForwardList<T, Alloc>::ForwardList(InputIterator first, InputIterator last)
{
    head.next = nullptr;
    insert_after_range(&head, first, last);
}

template <typename T, typename Alloc>
ForwardList<T, Alloc>::ForwardList(const ForwardList<T, Alloc>& other)
{
    head.next = nullptr;
    insert_after_range(&head, other.begin(), other.end());
}

template <typename T, typename Alloc>
ForwardList<T, Alloc>::~ForwardList()
{
    erase_after(&head, nullptr);
}

template <typename T, typename Alloc>
ForwardList<T, Alloc>& ForwardList<T, Alloc>::operator=(const ForwardList<T, Alloc>& other)
{
    if (this != &other)
    {
        list_node* p1 = &head;
        list_node* n1 = head.next;
        list_node* n2 = other.head.next;

        while (n1 != nullptr && n2 != nullptr)
        {
            n1->data = n2->data;
            p1 = n1;
            n1 = n1->next;
            n2 = n2->next;
        }

        if (n2 != nullptr)
        {
            insert_after_range(p1, const_iterator(n2), const_iterator(nullptr));
        }
        else
        {
            erase_after(p1, nullptr);
        }
    }

    return *this;
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::assign(size_type count, const T& value)
{
    fill_assign(count, value);
}

template <typename T, typename Alloc>
template <typename InputIterator>
void ForwardList<T, Alloc>::assign(InputIterator first, InputIterator last)
{
    typedef _is_integer<InputIterator>::integral integral;
    assign_dispatch(first, last, integral());
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::reference ForwardList<T, Alloc>::front()
{
    return head.next->data;
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::const_reference ForwardList<T, Alloc>::front() const
{
    return head.next->data;
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::iterator ForwardList<T, Alloc>::before_begin()
{
    return iterator(&head);
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::const_iterator ForwardList<T, Alloc>::before_begin() const
{
    return const_iterator(&head);
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::iterator ForwardList<T, Alloc>::begin()
{
    return iterator(head.next);
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::const_iterator ForwardList<T, Alloc>::begin() const
{
    return const_iterator(head.next);
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::iterator ForwardList<T, Alloc>::end()
{
    return iterator(nullptr);
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::const_iterator ForwardList<T, Alloc>::end() const
{
    return const_iterator(nullptr);
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::size_type ForwardList<T, Alloc>::size() const
{
    return distance(begin(), end());
}

template <typename T, typename Alloc>
bool ForwardList<T, Alloc>::empty() const
{
    return head.next == nullptr;
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::push_front(const T& value)
{
    make_link(&head, create_node(value));
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::pop_front()
{
    list_node* tmp = head.next;
    head.next = tmp->next;
    destroy_node(tmp);
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::iterator 
ForwardList<T, Alloc>::insert_after(const_iterator pos, const T& value)
{
    return iterator(insert_after(pos.node, value));
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::iterator
ForwardList<T, Alloc>::insert_after(const_iterator pos, size_type count, const T& value)
{
    return iterator(insert_after_fill(pos.node, count, value));
}

template <typename T, typename Alloc>
template <typename InputIterator>
typename ForwardList<T, Alloc>::iterator
ForwardList<T, Alloc>::insert_after(const_iterator pos, InputIterator first, InputIterator last)
{
    return iterator(insert_after_range(pos.node, first, last));
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::iterator
ForwardList<T, Alloc>::erase_after(const_iterator pos)
{
    return iterator(erase_after(pos.node));
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::iterator
ForwardList<T, Alloc>::erase_after(const_iterator first, const_iterator last)
{
    return iterator(erase_after(first.node, last.node));
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::resize(size_type count)
{
    resize(count, T());
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::resize(size_type count, const T& value)
{
    list_node* cur = &head;
    while (cur->next != nullptr && count > 0)
    {
        cur = cur->next;
        --count;
    }

    if (count > 0)
    {
        insert_after_fill(cur, count, value);
    }
    else
    {
        erase_after(cur, nullptr);
    }
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::clear()
{
    erase_after(&head, nullptr);
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::swap(ForwardList<T, Alloc>& other)
{
    Zyx::swap(head.next, other.head.next);
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::remove(const T& value)
{
    list_node* cur = &head;
    while (cur->next != nullptr)
    {
        if (cur->next->data == value)
        {
            erase_after(cur);
        }
        else
        {
            cur = cur->next;
        }
    }
}

template <typename T, typename Alloc>
template <typename UnaryPredicate>
void ForwardList<T, Alloc>::remove_if(UnaryPredicate pred)
{
    list_node* cur = &head;
    while (cur->next != nullptr)
    {
        if (pred(cur->next->data))
        {
            erase_after(cur);
        }
        else
        {
            cur = cur->next;
        }
    }
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::unique()
{
    list_node* cur = head.next;
    if (cur != nullptr)
    {
        while (cur->next != nullptr)
        {
            if (cur->data == cur->next->data)
            {
                erase_after(cur);
            }
            else
            {
                cur = cur->next;
            }
        }
    }
}

template <typename T, typename Alloc>
template <typename BinaryPredicate>
void ForwardList<T, Alloc>::unique(BinaryPredicate pred)
{
    list_node* cur = head.next;
    if (cur != nullptr)
    {
        while (cur->next != nullptr)
        {
            if (pred(cur->data, cur->next->data))
            {
                erase_after(cur);
            }
            else
            {
                cur = cur->next;
            }
        }
    }
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::splice_after(const_iterator pos, ForwardList<T, Alloc>& other)
{
    splice_after(pos.node, &other.head, previous(&other.head, nullptr));
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::splice_after(const_iterator pos, ForwardList<T, Alloc>& other, const_iterator it)
{
    splice_after(pos.node, it.node, previous(&other.head, nullptr));
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::splice_after(const_iterator pos, ForwardList<T, Alloc>& other, 
                                         const_iterator first, const_iterator last)
{
    if (first != last)
    {
        splice_after(pos.node, first.node, previous(&other.head, last.node));
    }
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::merge(ForwardList<T, Alloc>& other)
{
    list_node* p1 = &head;
    while (p1->next != nullptr && other.head.next != nullptr)
    {
        if (other.head.next->data < p1->next->data)
        {
            splice_after(p1, &other.head, other.head.next);
        }
        p1 = p1->next;
    }

    if (other.head.next != nullptr)
    {
        p1->next = other.head.next;
        other.head.next = nullptr;
    }
}

template <typename T, typename Alloc>
template <typename Compare>
void ForwardList<T, Alloc>::merge(ForwardList& other, Compare comp)
{
    list_node* p1 = &head;
    while (p1->next != nullptr && other.head.next != nullptr)
    {
        if (comp(other.head.next->data, p1->next->data))
        {
            splice_after(p1, &other.head, other.head.next);
        }
        p1 = p1->next;
    }

    if (other.head.next != nullptr)
    {
        p1->next = other.head.next;
        other.head.next = nullptr;
    }
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::sort()
{
    if (head.next != nullptr && head.next->next != nullptr)
    {
    	ForwardList carry;
    	ForwardList counter[64];
    	int fill = 0;

        while (!empty())
        {
            splice_after(&carry.head, &head, head.next);
            int i = 0;
            while (i < fill && !counter[i].empty())
            {
                carry.merge(counter[i]);
                // counter[i].merge(carry);
                // carry.swap(counter[i]);
                i++;
            }
            carry.swap(counter[i]);

            if (i == fill)
            {
                fill++;
            }
        }

        for (int i = 1; i < fill; i++)
        {
            counter[i].merge(counter[i - 1]);
        }
        swap(counter[fill - 1]);
    }
}

template <typename T, typename Alloc>
template <typename Compare>
void ForwardList<T, Alloc>::sort(Compare comp)
{
    if (head.next != nullptr && head.next->next != nullptr)
    {
    	ForwardList carry;
    	ForwardList counter[64];
    	int fill = 0;

        while (!empty())
        {
            splice_after(&carry.head, &head, head.next);
            int i = 0;
            while (i < fill && !counter[i].empty())
            {
                carry.merge(counter[i], comp);
                // counter[i].merge(carry);
                // carry.swap(counter[i]);
                i++;
            }
            carry.swap(counter[i]);

            if (i == fill)
            {
                fill++;
            }
        }

        for (int i = 1; i < fill; i++)
        {
            counter[i].merge(counter[i - 1], comp);
        }
        swap(counter[fill - 1]);
    }
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::reverse()
{
    if (head.next != nullptr)
    {
        list_node* prev = head.next;
        list_node* cur = prev->next;
        prev->next = nullptr;
        while (cur != nullptr)
        {
            list_node* tmp = cur->next;
            cur->next = prev;
            prev = cur;
            cur = tmp;
        }
        head.next = prev;
    }
}


//-----------------------------------【private functions】------------------------------------

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::list_node* 
ForwardList<T, Alloc>::create_node(const T& val)
{
    list_node* p = list_node_allocator::allocate();
    construct(&p->data, val);
    p->next = nullptr;
    return p;
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::destroy_node(list_node* p)
{
    destroy(&p->data);
    list_node_allocator::deallocate(p);
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::list_node* 
ForwardList<T, Alloc>::make_link(list_node* prev_node, list_node* new_node)
{
    new_node->next = prev_node->next;
    prev_node->next = new_node;
    return new_node;
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::list_node* 
ForwardList<T, Alloc>::previous(list_node* pos, list_node* node)
{
    while (pos->next != node)
    {
        pos = pos->next;
    }
    return pos;
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::fill_assign(size_type n, const T& val)
{
    list_node* prev = &head;
    list_node* node = head.next;

    while (node != nullptr && n > 0)
    {
        node->data = val;
        prev = node;
        node = node->next;
        n--;
    }

    if (n > 0)
    {
        insert_after_fill(prev, n, val);
    }
    else
    {
        erase_after(prev, nullptr);
    }
}

template <typename T, typename Alloc>
template <typename Integer>
void ForwardList<T, Alloc>::assign_dispatch(Integer n, Integer val, _true_type)
{
    fill_assign(n, val);
}

template <typename T, typename Alloc>
template <typename InputIterator>
void ForwardList<T, Alloc>::assign_dispatch(InputIterator first, InputIterator last, _false_type)
{
    list_node* prev = &head;
    list_node* node = head.next;

    while (node != nullptr && first != last)
    {
        node->data = *first;
        prev = node;
        node = node->next;
        ++first;
    }

    if (first != last)
    {
        insert_after_range(prev, first, last);
    }
    else
    {
        erase_after(prev, nullptr);
    }    
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::list_node* 
ForwardList<T, Alloc>::insert_after(list_node* pos, const T& val)
{
    return make_link(pos, create_node(val));
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::list_node*
ForwardList<T, Alloc>::insert_after_fill(list_node* pos, size_type n, const T& val)
{
    for (size_type i = 0; i < n; i++)
    {
        pos = make_link(pos, create_node(val));
    }
    return pos;
}

template <typename T, typename Alloc>
template <typename InputIterator>
typename ForwardList<T, Alloc>::list_node*
ForwardList<T, Alloc>::insert_after_range(list_node* pos, InputIterator first, InputIterator last)
{
    typedef _is_integer<InputIterator>::integral integral;
    return insert_after_range(pos, first, last, integral());
}

template <typename T, typename Alloc>
template <typename Integer>
typename ForwardList<T, Alloc>::list_node*
ForwardList<T, Alloc>::insert_after_range(list_node* pos, Integer n, Integer val, _true_type)
{
    return insert_after_fill(pos, n, val);
}

template <typename T, typename Alloc>
template <typename InputIterator>
typename ForwardList<T, Alloc>::list_node*
ForwardList<T, Alloc>::insert_after_range(list_node* pos, InputIterator first, InputIterator last, _false_type)
{
    while (first != last)
    {
        pos = make_link(pos, create_node(*first));
        ++first;
    }
    return pos;
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::list_node* 
ForwardList<T, Alloc>::erase_after(list_node* pos)
{
    list_node* tmp = pos->next;
    pos->next = tmp->next;
    destroy_node(tmp);
    return pos->next;
}

template <typename T, typename Alloc>
typename ForwardList<T, Alloc>::list_node* 
ForwardList<T, Alloc>::erase_after(list_node* before_first, list_node* last)
{
    list_node* cur = before_first->next;
    while (cur != last)
    {
        list_node* tmp = cur;
        cur = cur->next;
        destroy_node(tmp);
    }
    before_first->next = last;
    return last;
}

template <typename T, typename Alloc>
void ForwardList<T, Alloc>::splice_after(list_node* pos, list_node* before_first, list_node* before_last)
{
    if (pos != before_first && pos != before_last)
    {
        list_node* first = before_first->next;
        before_first->next = before_last->next;
        before_last->next = pos->next;
        pos->next = first;
    }
}

}

#endif