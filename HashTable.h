#ifndef ZYX_HASH_TABLE
#define ZYX_HASH_TABLE

#include "Iterator.h"
#include "Alloc.h"
#include "Construct.h"
#include "Algorithm.h"
#include "Utility"
#include "Vector.h"

namespace Zyx {

template <typename Value>
struct __hashtable_node
{
    __hashtable_node* next;
    Value val;
};

template <typename Value, typename Key, typename HashFcn, typename ExtractKey, 
          typename EqualKey, typename Alloc = alloc>
class HashTable;

template <typename Value, typename Key, typename HashFcn, 
          typename ExtractKey, typename EqualKey, typename Alloc>
struct __hashtable_iterator
{
	typedef HashTable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
    typedef __hashtable_node<Value> node;

    typedef forward_iterator_tag    iterator_category;
    typedef Value                   value_type;
    typedef Value*                  pointer;  
    typedef Value&                  reference;
    typedef size_t                  size_type;
    typedef ptrdiff_t               difference_type;

    node* cur;
    hashtable* ht;

    __hashtable_iterator() : cur(nullptr), ht(nullptr) { }
    __hashtable_iterator(node* n, hashtable* tab) : cur(n), ht(tab) { }

    bool operator==(const iterator& x) const { return cur == x.cur; }
    bool operator!=(const iterator& x) const { return cur != x.cur; }

    reference operator*() const { return cur->val; }
    pointer operator->() const { return &(operator*()); }

    iterator& operator++()
    {
        const node* old = cur;
        cur = cur->next;
        if (cur == nullptr) {
            size_type bucket = ht->bkt_num(old->val);
            while (cur == nullptr && ++bucket < ht->buckets.size())
                cur = ht->buckets[bucket];
        }
        return *this;
    }

    iterator operator++(int)
    {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }
};

static const int num_primes = 28;

static const unsigned long prime_list[num_primes] = 
{
    53ul,         97ul,         193ul,       389ul,       769ul,
    1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
    49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
    1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
    50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
    1610612741ul, 3221225473ul, 4294967291ul
};

inline unsigned long next_prime(unsigned long n)
{
    const unsigned long* first = prime_list;
    const unsigned long* last = prime_list + num_primes;
    const unsigned long* pos = lower_bound(first, last, n);
    return pos == last ? *(last - 1) : *pos;	
}

template <typename Value, typename Key, typename HashFcn, 
          typename ExtractKey, typename EqualKey, typename Alloc>
class HashTable
{
public:
    friend struct __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;

public:
    typedef Key                  key_type;
    typedef Value                value_type;
    typedef HashFcn              hasher;
    typedef EqualKey             key_equal;
    typedef value_type*          pointer;
    typedef const value_type*    const_pointer;
    typedef value_type&          reference;
    typedef const value_type&    const_reference;
    typedef size_t               size_type;
    typedef ptrdiff_t            difference_type;

    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;

private:
	typedef __hashtable_node<Value> node;
    typedef simple_alloc<node, Alloc> node_allocator;

public:
    HashTable(size_type n, const hasher& hf, const key_equal& eql) 
      : hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0)
    {
        initialize_buckets(n);
    }

    HashTable(const HashTable& ht) 
      : hash(ht.hash), equals(ht.equals), get_key(ht.get_key), num_elements(0)
    {
        
    }

public:
    size_type size() const { return num_elements; }
    size_type max_size() const { return size_type(-1); }
    bool empty() const { return num_elements == 0; }

    size_type bucket_count() const { return buckets.size(); }
    
    size_type max_bucket_count() const 
    { 
        return prime_list[num_primes - 1]; 
    }

    size_type elems_in_bucket(size_type bucket)
    {
        size_type result = 0;
        for (node* cur = buckets[bucket]; cur != nullptr; cur = cur->next)
            ++result;
        return result;
    }

    iterator begin()
    {
        for (size_type i = 0; i < buckets.size(); i++)
            if (buckets[i])
                return iterator(buckets[i], this);
        return end();
    }

    iterator end() { return iterator(nullptr, this); }

public:
    void resize(size_type num_elements_hint)
    {
        const size_type old_n = buckets.size();
        if (num_elements_hint > old_n) {
            const size_type n = next_size(num_elements_hint);
            if (n > old_n) {
                Vector<node*, Alloc> tmp(n, nullptr);
                for (size_type bucket = 0; bucket < old_n; ++bucket) {
                    node* first = buckets[bucket];
                    while (first != nullptr) {
                        size_type new_bucket = bkt_num(first->val, n);
                        buckets[bucket] = first->next;
                        first->next = tmp[new_bucket];
                        tmp[new_bucket] = first;
                        first = buckets[bucket];
                    }
                }
                buckets.swap(tmp);
            }
        }
    }

    Pair<iterator, bool> insert_unique(const value_type& obj)
    {
        resize(num_elements + 1);
        return insert_unique_noresize(obj);
    }

    iterator insert_equal(const value_type& obj)
    {
        resize(num_elements + 1);
        return insert_equal_noresize(obj);
    }

    Pair<iterator, bool> insert_unique_noresize(const value_type& obj)
    {
        const size_type n = bkt_num(obj);
        node* first = buckets[n];

        for (node* cur = first; cur != nullptr; cur = cur->next)
            if (equals(get_key(cur->val), get_key(obj)))
                return make_pair(iterator(cur, this), false);

        node* tmp = new_node(obj);
        tmp->next = first;
        buckets[n] = tmp;
        ++num_elements;
        return make_pair(iterator(tmp, this), true);
    }

    iterator insert_equal_noresize(const value_type& obj)
    {
        const size_type n = bkt_num(obj);
        node* first = buckets[n];

        for (node* cur = first; cur != nullptr; cur = cur->next) {
            if (equals(get_key(cur->val), get_key(obj))) {
                node* tmp = new_node(obj);
                tmp->next = cur->next;
                cur->next = tmp;
                ++num_elements;
                return iterator(tmp, this);
            }
        }

        node* tmp = new_node(obj);
        tmp->next = first;
        buckets[n] = tmp;
        ++num_elements;
        return iterator(tmp, this);
    }

private:
    node* new_node(const value_type& obj)
    {
        node* n = node_allocator::allocate();
        n->next = nullptr;
        construct(&n->val, obj);
        return n;
    }

    void delete_node(node* n)
    {
    	destroy(&n->val);
        node_allocator::deallocate(n);
    }

private:
    size_type next_size(size_type n) const { return next_prime(n); }

    void initialize_buckets(size_type n)
    {
        const size_type n_buckets = next_size(n);
        buckets.reserve(n_buckets);
        buckets.insert(buckets.end(), n_buckets, nullptr);
        num_elements = 0;
    }

    size_type bkt_num(const value_type& obj, size_t n) const
    {
        return bkt_num_key(get_key(obj), n);
    }

    size_type bkt_num(const value_type& obj) const
    {
        return bkt_num_key(get_key(obj));
    }

    size_type bkt_num_key(const key_type& key) const
    {
        return bkt_num_key(key, buckets.size());
    }

    size_type bkt_num_key(const key_type& key, size_t n) const
    {
        return hash(key) % n;
    }

private:
    hasher hash;
    key_equal equals;
    ExtractKey get_key;    
	Vector<node*, Alloc> buckets;
	size_type num_elements;
};

}

#endif