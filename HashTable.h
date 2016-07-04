#ifndef ZYX_HASH_TABLE
#define ZYX_HASH_TABLE

#include "Iterator.h"
#include "Alloc.h"
#include "Construct.h"
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

    reference operator*() const { return cur->val; }
    pointer operator->() const { return &(operator*()); }

    iterator& operator++()
    {
        const node* old = cur;
        cur = cur->next;
        if (!cur) {
        }
    }

    iterator operator++(int)
    {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }
};

static const int __stl_num_primes = 28;

static const unsigned long __stl_prime_list[__stl_num_primes] = 
{
    53ul,         97ul,         193ul,       389ul,       769ul,
    1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
    49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
    1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
    50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
    1610612741ul, 3221225473ul, 4294967291ul
};

inline unsigned long __stl_next_prime(unsigned long n)
{
    const unsigned long* first = __stl_prime_list;
    const unsigned long* last = __stl_prime_list + __stl_num_primes;
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

private:
	typedef __hashtable_node<Value>  node;
    typedef simple_alloc<node, Alloc> node_allocator;

public:
    size_type bucket_count() const { return buckets.size(); }
    size_type max_bucket_count() const { return __stl_prime_list[__stl_num_primes - 1]; }

private:
	node* new_node()

private:
	Zyx::Vector<node*, Alloc> buckets;
	size_type num_elements;

	hasher hash;
	key_equal equals;
	ExtractKey get_key;
};

}

#endif