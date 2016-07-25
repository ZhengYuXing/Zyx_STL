#ifndef ZYX_HASH_MAP
#define ZYX_HASH_MAP 

#include "HashTable.h"
#include "HashFun.h"
#include "Functional.h"
#include "Alloc.h"
#include "Utility.h"

namespace Zyx {

template <typename Key, typename T, typename HashFcn = hash<Key>, 
          typename EqualKey = equal_to<Key>, typename Alloc = alloc>
class HashMap
{
public:
    friend bool operator== <> (const HashMap& lhs, const HashMap& rhs);

public:
    typedef Key                   key_type;
    typedef T                     data_type;
    typedef T                     mapped_type;
    typedef Pair<const Key, T>    value_type;
    typedef HashFcn               hasher;
    typedef EqualKey              key_equal;

private:
    typedef HashTable<value_type, key_type, hasher, select1st<value_type>, key_equal, Alloc> 
            ht;

public:
    typedef typename ht::pointer            pointer;
    typedef typename ht::const_pointer      const_pointer;
    typedef typename ht::reference          reference;
    typedef typename ht::const_reference    const_reference;
    typedef typename ht::iterator           iterator;
    typedef typename ht::const_iterator     const_iterator;
    typedef typename ht::size_type          size_type;
    typedef typename ht::difference_type    difference_type;

public:
    HashMap() : rep(100, hasher(), key_equal()) { }
    explicit HashMap(size_type n) : rep(n, hasher(), key_equal()) { }
    HashMap(size_type n, const hasher& hf) : rep(n, hf, key_equal()) { }
    HashMap(size_type n, const hasher& hf, const key_equal& eql) : rep(n, hf, eql) { }

    template <typename InputIterator>
    HashMap(InputIterator first, InputIterator last) 
      : rep(100, hasher(), key_equal())
    {
    	rep.insert_unique(first, last);
    }

    template <typename InputIterator>
    HashMap(InputIterator first, InputIterator last, size_type n) 
      : rep(n, hasher(), key_equal())
    {
    	rep.insert_unique(first, last);
    }    

    template <typename InputIterator>
    HashMap(InputIterator first, InputIterator last, size_type n, const hasher& hf) 
      : rep(n, hf, key_equal())
    {
    	rep.insert_unique(first, last);
    }

    template <typename InputIterator>
    HashMap(InputIterator first, InputIterator last, size_type n, 
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

    iterator begin()  { return rep.begin(); }
    const_iterator begin() const { return rep.begin(); }
    iterator end() { return rep.end(); }
    const_iterator end() const { return rep.end(); }

public:
    void resize(size_type hint) { rep.resize(hint); }

    Pair<iterator, bool> insert(const value_type& obj) 
    { 
        return rep.insert_unique(obj); 
    }

    Pair<iterator, bool> insert_noresize(const value_type& obj) 
    { 
        return rep.insert_unique_noresize(obj); 
    }

    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        rep.insert_unique(first, last);
    }

    mapped_type& operator[](const key_type& key)
    {
        return rep.find_or_insert(value_type(key, T())).second;
    }

    size_type erase(const key_type& key) { return rep.erase(key); }
    void erase(iterator pos) { rep.erase(pos); }
    void erase(iterator first, iterator last) { rep.erase(first, last); }

    void clear() { rep.clear(); }
    void swap(HashMap& hs) { rep.swap(hs.rep); }

public:
    iterator find(const key_type& key) { return rep.find(key); }
    const_iterator find(const key_type& key) const { return rep.find(key); }

    size_type count(const key_type& key) const { return rep.count(key); }

    Pair<iterator, iterator> equal_range(const key_type& key) 
    { 
    	return rep.equal_range(key); 
    }

    Pair<const_iterator, const_iterator> equal_range(const key_type& key) const 
    { 
    	return rep.equal_range(key); 
    }

private:
    ht rep;
};

template <typename Key, typename T, typename HashFcn, typename EqualKey, typename Alloc>
inline bool operator==(const HashMap<Key, T, HashFcn, EqualKey, Alloc>& lhs,
                       const HashMap<Key, T, HashFcn, EqualKey, Alloc>& rhs)
{
    return lhs.rep == rhs.rep;
}

template <typename Key, typename T, typename HashFcn, typename EqualKey, typename Alloc>
inline bool operator!=(const HashMap<Key, T, HashFcn, EqualKey, Alloc>& lhs,
                       const HashMap<Key, T, HashFcn, EqualKey, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename Key, typename T, typename HashFcn, typename EqualKey, typename Alloc>
inline void swap(HashMap<Key, T, HashFcn, EqualKey, Alloc>& hm1,
                 HashMap<Key, T, HashFcn, EqualKey, Alloc>& hm2)
{
    hm1.swap(hm2);
}


//-----------------------------------【HashMultiMap class】-----------------------------------

template <typename Key, typename T, typename HashFcn = hash<Key>, 
          typename EqualKey = equal_to<Key>, typename Alloc = alloc>
class HashMultiMap
{
public:
    friend bool operator== <> (const HashMultiMap& lhs, const HashMultiMap& rhs);

public:
    typedef Key                   key_type;
    typedef T                     data_type;
    typedef T                     mapped_type;
    typedef Pair<const Key, T>    value_type;
    typedef HashFcn               hasher;
    typedef EqualKey              key_equal;

private:
    typedef HashTable<value_type, key_type, hasher, select1st<value_type>, key_equal, Alloc> 
            ht;

public:
    typedef typename ht::pointer            pointer;
    typedef typename ht::const_pointer      const_pointer;
    typedef typename ht::reference          reference;
    typedef typename ht::const_reference    const_reference;
    typedef typename ht::iterator           iterator;
    typedef typename ht::const_iterator     const_iterator;
    typedef typename ht::size_type          size_type;
    typedef typename ht::difference_type    difference_type;

public:
    HashMultiMap() : rep(100, hasher(), key_equal()) { }
    explicit HashMultiMap(size_type n) : rep(n, hasher(), key_equal()) { }
    HashMultiMap(size_type n, const hasher& hf) : rep(n, hf, key_equal()) { }
    HashMultiMap(size_type n, const hasher& hf, const key_equal& eql) : rep(n, hf, eql) { }

    template <typename InputIterator>
    HashMultiMap(InputIterator first, InputIterator last) 
      : rep(100, hasher(), key_equal())
    {
    	rep.insert_equal(first, last);
    }

    template <typename InputIterator>
    HashMultiMap(InputIterator first, InputIterator last, size_type n) 
      : rep(n, hasher(), key_equal())
    {
    	rep.insert_equal(first, last);
    }    

    template <typename InputIterator>
    HashMultiMap(InputIterator first, InputIterator last, size_type n, const hasher& hf) 
      : rep(n, hf, key_equal())
    {
    	rep.insert_equal(first, last);
    }

    template <typename InputIterator>
    HashMultiMap(InputIterator first, InputIterator last, size_type n, 
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

    iterator begin()  { return rep.begin(); }
    const_iterator begin() const { return rep.begin(); }
    iterator end() { return rep.end(); }
    const_iterator end() const { return rep.end(); }

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
    void swap(HashMultiMap& hs) { rep.swap(hs.rep); }

public:
    iterator find(const key_type& key) { return rep.find(key); }
    const_iterator find(const key_type& key) const { return rep.find(key); }

    size_type count(const key_type& key) const { return rep.count(key); }

    Pair<iterator, iterator> equal_range(const key_type& key) 
    { 
    	return rep.equal_range(key); 
    }

    Pair<const_iterator, const_iterator> equal_range(const key_type& key) const 
    { 
    	return rep.equal_range(key); 
    }

private:
    ht rep;
};

template <typename Key, typename T, typename HashFcn, typename EqualKey, typename Alloc>
inline bool operator==(const HashMultiMap<Key, T, HashFcn, EqualKey, Alloc>& lhs,
                       const HashMultiMap<Key, T, HashFcn, EqualKey, Alloc>& rhs)
{
    return lhs.rep == rhs.rep;
}

template <typename Key, typename T, typename HashFcn, typename EqualKey, typename Alloc>
inline bool operator!=(const HashMultiMap<Key, T, HashFcn, EqualKey, Alloc>& lhs,
                       const HashMultiMap<Key, T, HashFcn, EqualKey, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename Key, typename T, typename HashFcn, typename EqualKey, typename Alloc>
inline void swap(HashMultiMap<Key, T, HashFcn, EqualKey, Alloc>& hm1,
                 HashMultiMap<Key, T, HashFcn, EqualKey, Alloc>& hm2)
{
    hm1.swap(hm2);
}

}

#endif