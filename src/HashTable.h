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
    typedef HashTable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> 
            hashtable;
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> 
            iterator;
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

template <typename Value, typename Key, typename HashFcn, 
          typename ExtractKey, typename EqualKey, typename Alloc>
struct __hashtable_const_iterator
{
    typedef HashTable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> 
            hashtable;
    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> 
            iterator;
    typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> 
            const_iterator;
    typedef __hashtable_node<Value> node;

    typedef forward_iterator_tag    iterator_category;
    typedef Value                   value_type;
    typedef const Value*            pointer;  
    typedef const Value&            reference;
    typedef size_t                  size_type;
    typedef ptrdiff_t               difference_type;

    const node* cur;
    const hashtable* ht;

    __hashtable_const_iterator() : cur(nullptr), ht(nullptr) { }
    __hashtable_const_iterator(const node* n, const hashtable* tab) : cur(n), ht(tab) { }
    __hashtable_const_iterator(const iterator& x) : cur(x.cur), ht(x.ht) { }

    bool operator==(const const_iterator& x) const { return cur == x.cur; }
    bool operator!=(const const_iterator& x) const { return cur != x.cur; }

    reference operator*() const { return cur->val; }
    pointer operator->() const { return &(operator*()); }

    const_iterator& operator++()
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

    const_iterator operator++(int)
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
    friend bool operator== <> (const HashTable& lhs, const HashTable& rhs);

    friend struct 
    __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
    friend struct 
    __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;

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

    typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> 
            iterator;
    typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> 
            const_iterator;

private:
    typedef __hashtable_node<Value> node;
    typedef simple_alloc<node, Alloc> node_allocator;

public:
    HashTable(size_type n, const hasher& hf, const key_equal& eql) 
      : hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0)
    {
        initialize_buckets(n);
    }

    HashTable(size_type n, const hasher& hf, const key_equal& eql, const ExtractKey& ext) 
      : hash(hf), equals(eql), get_key(ext), num_elements(0)
    {
        initialize_buckets(n);
    }

    HashTable(const HashTable& ht) 
      : hash(ht.hash), equals(ht.equals), get_key(ht.get_key), num_elements(0)
    {
        copy_from(ht);
    }

    HashTable& operator=(const HashTable& ht)
    {
        if (this != &ht) {
            clear();
            hash = ht.hash;
            equals = ht.equals;
            get_key = ht.get_key;
            copy_from(ht);
        }
        return *this;
    }

    ~HashTable() { clear(); }

public:
    hasher hash_function() const { return hash; }
    key_equal key_eq() const { return equals; }
    size_type size() const { return num_elements; }
    size_type max_size() const { return size_type(-1); }
    bool empty() const { return num_elements == 0; }
    size_type bucket_count() const { return buckets.size(); }
    size_type max_bucket_count() const { return prime_list[num_primes - 1]; }

    size_type elems_in_bucket(size_type bucket)
    {
        size_type result = 0;
        for (node* cur = buckets[bucket]; cur != nullptr; cur = cur->next)
            ++result;
        return result;
    }

    iterator begin()
    {
        for (size_type i = 0; i < buckets.size(); ++i)
            if (buckets[i] != nullptr)
                return iterator(buckets[i], this);
        return end();
    }

    const_iterator begin() const
    {
        for (size_type i = 0; i < buckets.size(); ++i)
            if (buckets[i] != nullptr)
                return const_iterator(buckets[i], this);
        return end();
    }

    iterator end() { return iterator(nullptr, this); }
    const_iterator end() const { return const_iterator(nullptr, this); }

public:
    Pair<iterator, bool> insert_unique(const value_type& obj)
    {
        resize(num_elements + 1);
        return insert_unique_noresize(obj);
    }

    template <typename InputIterator>
    void insert_unique(InputIterator first, InputIterator last)
    {
        insert_unique(first, last, iterator_category(first));
    }

    template <typename InputIterator>
    void insert_unique(InputIterator first, InputIterator last, input_iterator_tag)
    {
        for (; first != last; ++first)
            insert_unique(*first);
    }

    template <typename ForwardIterator>
    void insert_unique(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
    {
        size_type n = distance(first, last);
        resize(num_elements + n);
        for (; n > 0; --n, ++first)
            insert_unique_noresize(*first);
    }

    iterator insert_equal(const value_type& obj)
    {
        resize(num_elements + 1);
        return insert_equal_noresize(obj);
    }

    template <typename InputIterator>
    void insert_equal(InputIterator first, InputIterator last)
    {
        insert_equal(first, last, iterator_category(first));
    }

    template <typename InputIterator>
    void insert_equal(InputIterator first, InputIterator last, input_iterator_tag)
    {
        for (; first != last; ++first)
            insert_equal(*first);
    }

    template <typename ForwardIterator>
    void insert_equal(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
    {
        size_type n = distance(first, last);
        resize(num_elements + n);
        for (; n > 0; --n, ++first)
            insert_equal_noresize(*first);
    }

    void resize(size_type num_elements_hint);
    reference find_or_insert(const value_type& obj);
    Pair<iterator, bool> insert_unique_noresize(const value_type& obj);
    iterator insert_equal_noresize(const value_type& obj);

    size_type erase(const key_type& key)
    {
        const size_type n = bkt_num_key(key);
        node* first = buckets[n];
        size_type erased = 0;
        if (first != nullptr) {
            node* cur = first;
            node* next = cur->next;
            while (next) {
                if (equals(get_key(next->val), key)) {
                    cur->next = next->next;
                    delete_node(next);
                    next = cur->next;
                    ++erased;
                    --num_elements;
                } else {
                    cur = next;
                    next = cur->next;
                }
            }
            if (equals(get_key(first->val), key)) {
                buckets[n] = first->next;
                delete_node(first);
                ++erased;
                --num_elements;
            }
        }
        return erased;
    }

    void erase(const iterator& pos)
    {
        node* p = pos.cur;
        if (p != nullptr) {
            const size_type n = bkt_num(p->val);
            node* cur = buckets[n];
            if (cur == p) {
                buckets[n] = cur->next;
                delete_node(cur);
                --num_elements;
            } else {
                node* next = cur->next;
                while (next) {
                    if (next == p) {
                        cur->next = next->next;
                        delete_node(next);
                        --num_elements;
                        break;
                    } else {
                        cur = next;
                        next = cur->next;
                    }
                }
            }
        }
    }

    void erase(const const_iterator& pos)
    {
        erase(iterator(const_cast<node*>(pos.cur), const_cast<HashTable*>(pos.ht)));
    }

    void erase(iterator first, iterator last) 
    {
        size_type f_bucket = first.cur ? bkt_num(first.cur->val) : buckets.size();
        size_type l_bucket = last.cur ? bkt_num(last.cur->val) : buckets.size();
        if (first.cur == last.cur) {
            return;
        } else if (f_bucket == l_bucket) {
            erase_bucket(f_bucket, first.cur, last.cur);
        } else {
            erase_bucket(f_bucket, first.cur, nullptr);
            for (size_type n = f_bucket + 1; n < l_bucket; n++)
                erase_bucket(n, nullptr);
            if (l_bucket != buckets.size())
                erase_bucket(l_bucket, last.cur);
        }
    }

    void erase(const_iterator first, const_iterator last) 
    {
        erase(iterator(const_cast<node*>(first.cur), const_cast<HashTable*>(first.ht)),
              iterator(const_cast<node*>(last.cur), const_cast<HashTable*>(last.ht)));
    }

    void clear()
    {
        for (size_type i = 0; i < buckets.size(); ++i) {
            node* cur = buckets[i];
            while (cur != nullptr) {
                node* next = cur->next;
                delete_node(cur);
                cur = next;
            }
            buckets[i] = nullptr;
        }
        num_elements = 0;
    }  

    void swap(HashTable& ht)
    {
        Zyx::swap(hash, ht.hash);
        Zyx::swap(equals, ht.equals);
        Zyx::swap(get_key, ht.get_key);
        buckets.swap(ht.buckets);
        Zyx::swap(num_elements, ht.num_elements);
    }

public:
    iterator find(const key_type& key)
    {
        const size_type n = bkt_num_key(key);
        node* first = buckets[n];
        while (first != nullptr && !equals(get_key(first->val), key))
            first = first->next;
        return iterator(first, this);
    }

    const_iterator find(const key_type& key) const
    {
        const size_type n = bkt_num_key(key);
        node* first = buckets[n];
        while (first != nullptr && !equals(get_key(first->val), key))
            first = first->next;
        return const_iterator(first, this);
    }

    size_type count(const key_type& key) const
    {
        const size_type n = bkt_num_key(key);
        size_type result = 0;
        for (const node* cur = buckets[n]; cur != nullptr; cur = cur->next)
            if (equals(get_key(cur->val), key))
                ++result;
        return result;
    }

    Pair<iterator, iterator> equal_range(const key_type& key)
    {
        const size_type n = bkt_num_key(key);
        for (node* first = buckets[n]; first != nullptr; first = first->next) {
            if (equals(get_key(first->val), key)) {
                for (node* cur = first->next; cur != nullptr; cur = cur->next)
                    if (!equals(get_key(cur->val), key))
                        return make_pair(iterator(first, this), 
                                         iterator(cur, this));
                for (size_type m = n + 1; m < buckets.size(); ++m)
                    if (buckets[m] != nullptr)
                        return make_pair(iterator(first, this), 
                                         iterator(buckets[m], this));
                return make_pair(iterator(first, this), end());
            }
        }
        return make_pair(end(), end());
    }

    Pair<const_iterator, const_iterator> equal_range(const key_type& key) const
    {
        const size_type n = bkt_num_key(key);
        for (node* first = buckets[n]; first != nullptr; first = first->next) {
            if (equals(get_key(first->val), key)) {
                for (node* cur = first->next; cur != nullptr; cur = cur->next)
                    if (!equals(get_key(cur->val), key))
                        return make_pair(const_iterator(first, this), 
                                         const_iterator(cur, this));
                for (size_type m = n + 1; m < buckets.size(); ++m)
                    if (buckets[m] != nullptr)
                        return make_pair(const_iterator(first, this), 
                                         const_iterator(buckets[m], this));
                return make_pair(const_iterator(first, this), end());
            }
        }
        return make_pair(end(), end());
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
    void initialize_buckets(size_type n)
    {
        const size_type n_buckets = next_size(n);
        buckets.reserve(n_buckets);
        buckets.insert(buckets.end(), n_buckets, nullptr);
        num_elements = 0;
    }

    void copy_from(const HashTable& ht)
    {
        buckets.clear();
        buckets.reserve(ht.buckets.size());
        buckets.insert(buckets.end(), ht.buckets.size(), nullptr);
        for (size_type i = 0; i < ht.buckets.size(); ++i) {
            const node* cur = ht.buckets[i];
            if (cur != nullptr) {
                node* copy = new_node(cur->val);
                buckets[i] = copy;
                for (cur = cur->next; cur != nullptr; cur = cur->next) {
                    copy->next = new_node(cur->val);
                    copy = copy->next;
                }
            }
        }
        num_elements = ht.num_elements;
    }

    void erase_bucket(size_type n, node* first, node* last)
    {
        node* cur = buckets[n];
        if (cur == first) {
            erase_bucket(n, last);
        } else {
            node* next = cur->next;
            while (next != first) {
                cur = next;
                next = cur->next;
            }
            while (next != last) {
                cur->next = next->next;
                delete_node(next);
                next = cur->next;
                --num_elements;
            }
        }
    }

    void erase_bucket(size_type n, node* last)
    {
        node* cur = buckets[n];
        while (cur != last) {
            buckets[n] = cur->next;
            delete_node(cur);
            cur = buckets[n];
            --num_elements;
        }
    }

private:
    size_type next_size(size_type n) const { return next_prime(n); }

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

template <typename Val, typename Key, typename HF, typename Ex, typename Eq, typename Alloc>
bool operator==(const HashTable<Val, Key, HF, Ex, Eq, Alloc>& lhs,
                const HashTable<Val, Key, HF, Ex, Eq, Alloc>& rhs)
{
    typedef typename HashTable<Val, Key, HF, Ex, Eq, Alloc>::node node;
    if (lhs.buckets.size() != rhs.buckets.size())
        return false;
    for (int n = 0; n < lhs.buckets.size(); ++n) {
        node* cur1 = lhs.buckets[n];
        node* cur2 = rhs.buckets[n];
        while (cur1 != nullptr && cur2 != nullptr && cur1->val == cur2->val) {
            cur1 = cur1->next;
            cur2 = cur2->next;
        }
        if (cur1 || cur2)
            return false;
    }
    return true;
}

template <typename Val, typename Key, typename HF, typename Ex, typename Eq, typename Alloc>
inline bool operator!=(const HashTable<Val, Key, HF, Ex, Eq, Alloc>& lhs,
                       const HashTable<Val, Key, HF, Ex, Eq, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename Val, typename Key, typename HF, typename Ex, typename Eq, typename Alloc>
inline void swap(HashTable<Val, Key, HF, Ex, Eq, Alloc>& ht1, 
                 HashTable<Val, Key, HF, Ex, Eq, Alloc>& ht2)
{
    ht1.swap(ht2);
}


template <typename Val, typename Key, typename HF, typename Ex, typename Eq, typename Alloc>
void HashTable<Val, Key, HF, Ex, Eq, Alloc>::resize(size_type num_elements_hint)
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

template <typename Val, typename Key, typename HF, typename Ex, typename Eq, typename Alloc>
typename HashTable<Val, Key, HF, Ex, Eq, Alloc>::reference 
HashTable<Val, Key, HF, Ex, Eq, Alloc>::find_or_insert(const value_type& obj)
{
    resize(num_elements + 1);
    const size_type n = bkt_num(obj);
    node* first = buckets[n];
    for (node* cur = first; cur != nullptr; cur = cur->next)
        if (equals(get_key(cur->val), get_key(obj)))
            return cur->val;
    node* tmp = new_node(obj);
    tmp->next = first;
    buckets[n] = tmp;
    ++num_elements;
    return tmp->val;
}

template <typename Val, typename Key, typename HF, typename Ex, typename Eq, typename Alloc>
Pair<typename HashTable<Val, Key, HF, Ex, Eq, Alloc>::iterator, bool> 
HashTable<Val, Key, HF, Ex, Eq, Alloc>::insert_unique_noresize(const value_type& obj)
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

template <typename Val, typename Key, typename HF, typename Ex, typename Eq, typename Alloc>
typename HashTable<Val, Key, HF, Ex, Eq, Alloc>::iterator 
HashTable<Val, Key, HF, Ex, Eq, Alloc>::insert_equal_noresize(const value_type& obj)
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

}

#endif