#ifndef ZYX_HASH_SET
#define ZYX_HASH_SET 

#include "HashTable.h"
#include "HashFun.h"
#include "Functional.h"
#include "Alloc.h"

namespace Zyx {

//-------------------------------------【HashSet class】--------------------------------------

template <typename Value, typename HashFcn = hash<Value>, 
          typename EqualKey = equal_to<Value>, typename Alloc = alloc>
class HashSet
{
public:
    friend bool operator== <> (const HashSet& lhs, const HashSet& rhs);

private:
    typedef HashTable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;

public:
    typedef typename ht::key_type           key_type;
    typedef typename ht::value_type         value_type;
    typedef typename ht::hasher             hasher;
    typedef typename ht::key_equal          key_equal;

    typedef typename ht::const_pointer      pointer;
    typedef typename ht::const_pointer      const_pointer;
    typedef typename ht::const_reference    reference;
    typedef typename ht::const_reference    const_reference;
    typedef typename ht::const_iterator     iterator;
    typedef typename ht::const_iterator     const_iterator;
    typedef typename ht::size_type          size_type;
    typedef typename ht::difference_type    difference_type;

public:
    HashSet() : rep(100, hasher(), key_equal()) { }	
    explicit HashSet(size_type n) : rep(n, hasher(), key_equal()) { }
    HashSet(size_type n, const hasher& hf) : rep(n, hf, key_equal()) { }
    HashSet(size_type n, const hasher& hf, const key_equal& eql) : rep(n, hf, eql) { }

    template <typename InputIterator>
    HashSet(InputIterator first, InputIterator last) 
      : rep(100, hasher(), key_equal())
    {
    	rep.insert_unique(first, last);
    }

    template <typename InputIterator>
    HashSet(InputIterator first, InputIterator last, size_type n) 
      : rep(n, hasher(), key_equal())
    {
    	rep.insert_unique(first, last);
    }

    template <typename InputIterator>
    HashSet(InputIterator first, InputIterator last, size_type n, const hasher& hf) 
      : rep(n, hf, key_equal())
    {
    	rep.insert_unique(first, last);
    }

    template <typename InputIterator>
    HashSet(InputIterator first, InputIterator last, size_type n, 
    	    const hasher& hf, const key_equal& eql) 
      : rep(n, hf, eql)
    {
    	rep.insert_unique(first, last);
    }

public:
    hasher hash_function() const { return rep.hash_function(); }
    key_equal key_eq() const { return rep.key_eq(); }

    size_type size() const { return rep.size(); }
    size_type max_size() const { return rep.max_size(); }
    bool empty() const { return rep.empty(); }

    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const { return rep.elems_in_bucket(n); }

    iterator begin() const { return rep.begin(); }
    iterator end() const { return rep.end(); }

public:
    void resize(size_type hint) { rep.resize(hint); }

    Pair<iterator, bool> insert(const value_type& obj) 
    {
        Pair<typename ht::iterator, bool> p = rep.insert_unique(obj);
        return Pair<iterator, bool>(p.first, p.second);
    }

    Pair<iterator, bool> insert_noresize(const value_type& obj)
    {
        Pair<typename ht::iterator, bool> p = rep.insert_unique_noresize(obj);
        return Pair<iterator, bool>(p.first, p.second);     	
    }

    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
    	rep.insert_unique(first, last);
    }

    size_type erase(const key_type& key) { return rep.erase(key); }
    void erase(iterator pos) { rep.erase(pos); }
    void erase(iterator first, iterator last) { rep.erase(first, last); }

    void clear() { rep.clear(); }
    void swap(HashSet& hs) { rep.swap(hs.rep); }

public:
    iterator find(const key_type& key) const { return rep.find(key); }
    size_type count(const key_type& key) const { return rep.count(key); }

    Pair<iterator, iterator> equal_range(const key_type& key) const 
    { 
    	return rep.equal_range(key); 
    }

private:
    ht rep;
};

template <typename Value, typename HashFcn, typename EqualKey, typename Alloc>
inline bool operator==(const HashSet<Value, HashFcn, EqualKey, Alloc>& lhs,
                       const HashSet<Value, HashFcn, EqualKey, Alloc>& rhs)
{
    return lhs.rep == rhs.rep;
}

template <typename Value, typename HashFcn, typename EqualKey, typename Alloc>
inline bool operator!=(const HashSet<Value, HashFcn, EqualKey, Alloc>& lhs,
                       const HashSet<Value, HashFcn, EqualKey, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename Value, typename HashFcn, typename EqualKey, typename Alloc>
inline void swap(HashSet<Value, HashFcn, EqualKey, Alloc>& hs1, 
                 HashSet<Value, HashFcn, EqualKey, Alloc>& hs2)
{
    hs1.swap(hs2);
}


//-----------------------------------【HashMultiSet class】-----------------------------------

template <typename Value, typename HashFcn = hash<Value>, 
          typename EqualKey = equal_to<Value>, typename Alloc = alloc>
class HashMultiSet
{
public:
    friend bool operator== <> (const HashMultiSet& lhs, const HashMultiSet& rhs);

private:
    typedef HashTable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;

public:
    typedef typename ht::key_type           key_type;
    typedef typename ht::value_type         value_type;
    typedef typename ht::hasher             hasher;
    typedef typename ht::key_equal          key_equal;

    typedef typename ht::const_pointer      pointer;
    typedef typename ht::const_pointer      const_pointer;
    typedef typename ht::const_reference    reference;
    typedef typename ht::const_reference    const_reference;
    typedef typename ht::const_iterator     iterator;
    typedef typename ht::const_iterator     const_iterator;
    typedef typename ht::size_type          size_type;
    typedef typename ht::difference_type    difference_type;

public:
    HashMultiSet() : rep(100, hasher(), key_equal()) { }	
    explicit HashMultiSet(size_type n) : rep(n, hasher(), key_equal()) { }
    HashMultiSet(size_type n, const hasher& hf) : rep(n, hf, key_equal()) { }
    HashMultiSet(size_type n, const hasher& hf, const key_equal& eql) : rep(n, hf, eql) { }

    template <typename InputIterator>
    HashMultiSet(InputIterator first, InputIterator last) 
      : rep(100, hasher(), key_equal())
    {
    	rep.insert_equal(first, last);
    }

    template <typename InputIterator>
    HashMultiSet(InputIterator first, InputIterator last, size_type n) 
      : rep(n, hasher(), key_equal())
    {
    	rep.insert_equal(first, last);
    }

    template <typename InputIterator>
    HashMultiSet(InputIterator first, InputIterator last, size_type n, const hasher& hf) 
      : rep(n, hf, key_equal())
    {
    	rep.insert_equal(first, last);
    }

    template <typename InputIterator>
    HashMultiSet(InputIterator first, InputIterator last, size_type n, 
    	    const hasher& hf, const key_equal& eql) 
      : rep(n, hf, eql)
    {
    	rep.insert_equal(first, last);
    }

public:
    hasher hash_function() const { return rep.hash_function(); }
    key_equal key_eq() const { return rep.key_eq(); }

    size_type size() const { return rep.size(); }
    size_type max_size() const { return rep.max_size(); }
    bool empty() const { return rep.empty(); }

    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const { return rep.elems_in_bucket(n); }

    iterator begin() const { return rep.begin(); }
    iterator end() const { return rep.end(); }

public:
    void resize(size_type hint) { rep.resize(hint); }

    iterator insert(const value_type& obj) 
    {
        return rep.insert_equal(obj); 
    }

    iterator insert_noresize(const value_type& obj) 
    { 
    	return rep.insert_equal_noresize(obj); 
    }

    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
    	rep.insert_equal(first, last);
    }

    size_type erase(const key_type& key) { return rep.erase(key); }
    void erase(iterator pos) { rep.erase(pos); }
    void erase(iterator first, iterator last) { rep.erase(first, last); }

    void clear() { rep.clear(); }
    void swap(HashMultiSet& hs) { rep.swap(hs.rep); }

public:
    iterator find(const key_type& key) const { return rep.find(key); }
    size_type count(const key_type& key) const { return rep.count(key); }

    Pair<iterator, iterator> equal_range(const key_type& key) const 
    { 
    	return rep.equal_range(key); 
    }

private:
    ht rep;
};

template <typename Value, typename HashFcn, typename EqualKey, typename Alloc>
inline bool operator==(const HashMultiSet<Value, HashFcn, EqualKey, Alloc>& lhs,
                       const HashMultiSet<Value, HashFcn, EqualKey, Alloc>& rhs)
{
    return lhs.rep == rhs.rep;
}

template <typename Value, typename HashFcn, typename EqualKey, typename Alloc>
inline bool operator!=(const HashMultiSet<Value, HashFcn, EqualKey, Alloc>& lhs,
                       const HashMultiSet<Value, HashFcn, EqualKey, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename Value, typename HashFcn, typename EqualKey, typename Alloc>
inline void swap(HashMultiSet<Value, HashFcn, EqualKey, Alloc>& hs1, 
                 HashMultiSet<Value, HashFcn, EqualKey, Alloc>& hs2)
{
    hs1.swap(hs2);
}

}

#endif