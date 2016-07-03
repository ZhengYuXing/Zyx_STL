#ifndef ZYX_SET
#define ZYX_SET 

#include "RedBlackTree.h"

namespace Zyx {

template <typename Key, typename Compare = less<Key>, typename Alloc = alloc>
class Set
{
public:
    friend bool operator== <> (const Set& lhs, const Set& rhs);
    friend bool operator< <> (const Set& lhs, const Set& rhs);

    // template <typename K, typename C, typename A>
    // friend bool operator==(const Set<K, C, A>& lhs, const Set<K, C, A>& rhs);
    // template <typename K, typename C, typename A>
    // friend bool operator<(const Set<K, C, A>& lhs, const Set<K, C, A>& rhs);

private:
	typedef RedBlackTree<Key, Key, identity<Key>, Compare, Alloc> rep_type;

public:
    typedef Key                                   key_type;
    typedef Key                                   value_type;
    typedef Compare                               key_compare;
    typedef Compare                               value_compare;

    typedef typename rep_type::const_pointer      pointer;
    typedef typename rep_type::const_pointer      const_pointer;
    typedef typename rep_type::const_reference    reference;
    typedef typename rep_type::const_reference    const_reference;
    typedef typename rep_type::const_iterator     iterator;
    typedef typename rep_type::const_iterator     const_iterator;
    typedef typename rep_type::size_type          size_type;
    typedef typename rep_type::difference_type    difference_type;

public:
    Set() : t(Compare()) { }
    explicit Set(const Compare& comp) : t(comp) { }

    template <typename InputIterator>
    Set(InputIterator first, InputIterator last) : t(Compare()) 
    {
        t.insert_unique(first, last); 
    }

    template <typename InputIterator>
    Set(InputIterator first, InputIterator last, const Compare& comp) : t(comp) 
    {
        t.insert_unique(first, last); 
    }   

    Set(const Set& x) : t(x.t) { } 

    Set& operator=(const Set& x) 
    {
        t = x.t; 
        return *this; 
    }

public:
    key_compare key_comp() const { return t.key_compare(); }
    value_compare value_comp() const { return t.key_compare(); }
    iterator begin() const { return t.begin(); }
    // const_iterator begin() const { return t.begin(); }
    iterator end() const { return t.end(); }
    // const_iterator end() const { return t.end(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }

public:
    Pair<iterator, bool> insert(const value_type& val)
    {
        Pair<typename rep_type::iterator, bool> p = t.insert_unique(val);
        return Pair<iterator, bool>(p.first, p.second);
    }

    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        t.insert_unique(first, last);
    }

    void clear() { t.clear(); }
    void swap(Set& x) { t.swap(x.t); }

    iterator find(const value_type& val) const { return t.find(val); }
    size_type count(const value_type& val) const { return t.count(val); }

    iterator lower_bound(const value_type& val) const { return t.lower_bound(val); }
    iterator upper_bound(const value_type& val) const { return t.upper_bound(val); }

    Pair<iterator, iterator> equal_range(const value_type& val) const 
    { 
        return t.equal_range(val); 
    }

private:
	rep_type t;
};

template <typename Key, typename Compare, typename Alloc>
inline bool operator==(const Set<Key, Compare, Alloc>& lhs, 
                       const Set<Key, Compare, Alloc>& rhs)
{
    return lhs.t == rhs.t;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator!=(const Set<Key, Compare, Alloc>& lhs, 
                       const Set<Key, Compare, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator<(const Set<Key, Compare, Alloc>& lhs, 
                      const Set<Key, Compare, Alloc>& rhs)
{
    return lhs.t < rhs.t;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator>(const Set<Key, Compare, Alloc>& lhs, 
                      const Set<Key, Compare, Alloc>& rhs)
{
    return rhs < lhs;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator<=(const Set<Key, Compare, Alloc>& lhs, 
                       const Set<Key, Compare, Alloc>& rhs)
{
    return !(rhs < lhs);
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator>=(const Set<Key, Compare, Alloc>& lhs, 
                       const Set<Key, Compare, Alloc>& rhs)
{
    return !(lhs < rhs);
}

template <typename Key, typename Compare, typename Alloc>
inline void swap(Set<Key, Compare, Alloc>& x, Set<Key, Compare, Alloc>& y)
{
    x.swap(y);
}

}

#endif