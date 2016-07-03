#ifndef ZYX_MAP
#define ZYX_MAP 

#include "RedBlackTree.h"
#include "Functional.h"

namespace Zyx {

template <typename Key, typename Value, typename Compare = less<Key>, 
	      typename Alloc = alloc>
class Map
{
public:
    friend bool operator== <> (const Map& lhs, const Map& rhs);
    friend bool operator< <> (const Map& lhs, const Map& rhs);

public:
    typedef Key                                  key_type;
    typedef Value                                data_type;
    typedef Value                                mapped_type;
    typedef Pair<const key_type, mapped_type>    value_type;
    typedef Compare                              key_compare;

public:
    class value_compare
    {
    public:
        friend class Map;
        typedef value_type first_argument_type;
        typedef value_type second_argument_type;
        typedef bool result_type;
        bool operator()(const value_type& x, const value_type& y) const
        {
            return comp(x.first, x.second);
        }

    private:
        Compare comp;
        value_compare(Compare c) : comp(c) { }    	
    };

private:
    typedef RedBlackTree<key_type, value_type, 
                         select1st<value_type>, key_compare, Alloc> rep_type;

public:
    typedef typename rep_type::pointer            pointer;
    typedef typename rep_type::const_pointer      const_pointer;
    typedef typename rep_type::reference          reference;
    typedef typename rep_type::const_reference    const_reference;
    typedef typename rep_type::iterator           iterator;
    typedef typename rep_type::const_iterator     const_iterator;
    typedef typename rep_type::size_type          size_type;
    typedef typename rep_type::difference_type    difference_type;

public:
    Map() : t(Compare()) { }
    explicit Map(const Compare& comp) : t(comp) { }

    template <typename InputIterator>
    Map(InputIterator first, InputIterator last)
    {
        t.insert_unique(first, last);
    }

    template <typename InputIterator>
    Map(InputIterator first, InputIterator last, const Compare& comp) : t(comp)
    {
        t.insert_unique(first, last);
    }    

    Map(const Map& x) : t(x.t) { }

    Map& operator=(const Map& x)
    {
        t = x.t;
        return *this;
    }

public:
    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return value_compare(t.key_comp()); }
    iterator begin() { return t.begin(); }
    const_iterator begin() const { return t.begin(); }
    iterator end() { return t.end(); }
    const_iterator end() const { return t.end(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }

public:
    Pair<iterator, bool> insert(const value_type& val) 
    { 
        return t.insert_unique(val); 
    }

    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        t.insert_unique(first, last);
    }

    mapped_type& operator[](const key_type& k)
    {
        return (*((insert(value_type(k, Value()))).first)).second;
    }	

    void clear() { t.clear(); }
    void swap(Map& x) { t.swap(x.t); }

    iterator find(const key_type& k) { return t.find(k); }
    const_iterator find(const key_type& k) const { return t.find(k); }
    
    size_type count(const key_type& k) const { return t.count(k); }

    iterator lower_bound(const key_type& k) { return t.lower_bound(k); }
    const_iterator lower_bound(const key_type& k) const { return t.lower_bound(k); }

    iterator upper_bound(const key_type& k) { return t.upper_bound(k); }
    const_iterator upper_bound(const key_type& k) const { return t.lower_bound(k); }

    Pair<iterator, iterator> equal_range(const key_type& k) 
    { 
    	return t.equal_range(k); 
    }

    Pair<const_iterator, const_iterator> equal_range(const key_type& k) const 
    { 
    	return t.equal_range(k); 
    }

private:
    rep_type t;
};

template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator==(const Map<Key, Value, Compare, Alloc>& lhs, 
                       const Map<Key, Value, Compare, Alloc>& rhs)
{
	return lhs.t == rhs.t;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator!=(const Map<Key, Value, Compare, Alloc>& lhs, 
                       const Map<Key, Value, Compare, Alloc>& rhs)
{
	return !(lhs == rhs);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator<(const Map<Key, Value, Compare, Alloc>& lhs, 
                       const Map<Key, Value, Compare, Alloc>& rhs)
{
	return lhs.t < rhs.t;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator>(const Map<Key, Value, Compare, Alloc>& lhs, 
                       const Map<Key, Value, Compare, Alloc>& rhs)
{
	return rhs < lhs;
}

template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator<=(const Map<Key, Value, Compare, Alloc>& lhs, 
                       const Map<Key, Value, Compare, Alloc>& rhs)
{
	return !(rhs < lhs);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
inline bool operator>=(const Map<Key, Value, Compare, Alloc>& lhs, 
                       const Map<Key, Value, Compare, Alloc>& rhs)
{
	return !(lhs < rhs);
}

template <typename Key, typename Value, typename Compare, typename Alloc>
inline void swap(Map<Key, Value, Compare, Alloc>& lhs, 
	             Map<Key, Value, Compare, Alloc>& rhs)
{
	lhs.swap(rhs);
}

}

#endif