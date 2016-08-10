#ifndef ZYX_MULTI_SET
#define ZYX_MULTI_SET 

#include "RedBlackTree.h"
#include "Functional.h"

namespace Zyx {

template <typename Key, typename Compare = less<Key>, typename Alloc = alloc>
class MultiSet
{
public:
    friend bool operator== <> (const MultiSet& lhs, const MultiSet& rhs);
    friend bool operator< <> (const MultiSet& lhs, const MultiSet& rhs);

    // template <typename K, typename C, typename A>
    // friend bool operator==(const MultiSet<K, C, A>& lhs, const MultiSet<K, C, A>& rhs);
    // template <typename K, typename C, typename A>
    // friend bool operator<(const MultiSet<K, C, A>& lhs, const MultiSet<K, C, A>& rhs);

private:
    typedef RedBlackTree<Key, Key, identity<Key>, Compare, Alloc> rep_type;

public:
    typedef Key        key_type;
    typedef Key        value_type;
    typedef Compare    key_compare;
    typedef Compare    value_compare;

    typedef typename rep_type::const_pointer      pointer;
    typedef typename rep_type::const_pointer      const_pointer;
    typedef typename rep_type::const_reference    reference;
    typedef typename rep_type::const_reference    const_reference;
    typedef typename rep_type::const_iterator     iterator;
    typedef typename rep_type::const_iterator     const_iterator;
    typedef typename rep_type::size_type          size_type;
    typedef typename rep_type::difference_type    difference_type;

public:
    MultiSet() : t(Compare()) { }
    explicit MultiSet(const Compare& comp) : t(comp) { }

    template <typename InputIterator>
    MultiSet(InputIterator first, InputIterator last) : t(Compare()) 
    {
        t.insert_equal(first, last); 
    }

    template <typename InputIterator>
    MultiSet(InputIterator first, InputIterator last, const Compare& comp) : t(comp) 
    {
        t.insert_equal(first, last); 
    }   

    MultiSet(const MultiSet& x) : t(x.t) { } 

    MultiSet& operator=(const MultiSet& x) 
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
    iterator insert(const value_type& val)
    {
        return t.insert_equal(val);
    }

    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        t.insert_equal(first, last);
    }

    void erase(iterator pos)
    {
        typedef typename rep_type::iterator rep_iterator;
        t.erase((rep_iterator&)pos);
    }

    void erase(iterator first, iterator last)
    {
        typedef typename rep_type::iterator rep_iterator;
        t.erase((rep_iterator&)first, (rep_iterator&)last);
    }

    size_type erase(const key_type& k) { return t.erase(k); }

    void clear() { t.clear(); }
    void swap(MultiSet& x) { t.swap(x.t); }

public:
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
inline bool operator==(const MultiSet<Key, Compare, Alloc>& lhs, 
                       const MultiSet<Key, Compare, Alloc>& rhs)
{
    return lhs.t == rhs.t;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator!=(const MultiSet<Key, Compare, Alloc>& lhs, 
                       const MultiSet<Key, Compare, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator<(const MultiSet<Key, Compare, Alloc>& lhs, 
                      const MultiSet<Key, Compare, Alloc>& rhs)
{
    return lhs.t < rhs.t;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator>(const MultiSet<Key, Compare, Alloc>& lhs, 
                      const MultiSet<Key, Compare, Alloc>& rhs)
{
    return rhs < lhs;
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator<=(const MultiSet<Key, Compare, Alloc>& lhs, 
                       const MultiSet<Key, Compare, Alloc>& rhs)
{
    return !(rhs < lhs);
}

template <typename Key, typename Compare, typename Alloc>
inline bool operator>=(const MultiSet<Key, Compare, Alloc>& lhs, 
                       const MultiSet<Key, Compare, Alloc>& rhs)
{
    return !(lhs < rhs);
}

template <typename Key, typename Compare, typename Alloc>
inline void swap(MultiSet<Key, Compare, Alloc>& x, MultiSet<Key, Compare, Alloc>& y)
{
    x.swap(y);
}

}

#endif