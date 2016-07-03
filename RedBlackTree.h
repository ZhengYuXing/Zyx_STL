#ifndef ZYX_RED_BLACK_TREE
#define ZYX_RED_BLACK_TREE 

#include "Iterator.h"
#include "Alloc.h"
#include "Construct.h"
#include "Algorithm.h"
#include "Utility.h"

namespace Zyx {

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

struct __rb_tree_node_base
{
    typedef __rb_tree_color_type color_type;
    typedef __rb_tree_node_base* base_ptr;

    color_type color;
    base_ptr parent;
    base_ptr left;
    base_ptr right;

    static base_ptr minimum(base_ptr x)
    {
        while (x->left != nullptr)
            x = x->left;
        return x;
    }

    static base_ptr maximum(base_ptr x)
    {
        while (x->right != nullptr)
            x = x->right;
        return x;
    }	
};

template <typename T>
struct __rb_tree_node : public __rb_tree_node_base
{
    typedef __rb_tree_node<T>* link_type;
    T data;
};

struct __rb_tree_base_iterator
{
    typedef __rb_tree_node_base::base_ptr    base_ptr;
    typedef bidirectional_iterator_tag       iterator_category;
    typedef ptrdiff_t                        difference_type;

    base_ptr node;

    // __rb_tree_base_iterator(base_ptr x) : node(x) { }

    void increment()
    {
        if (node->right != nullptr) {
            node = node->right;
            while (node->left != nullptr)
                node = node->left;
        } else {
            base_ptr y = node->parent;
            while (y->right == node) {
                node = y;
                y = y->parent;
            }
            if (node->right != y) 
                node = y;
        }
    }

    void decrement()
    {
        if (node->color == __rb_tree_red && node->parent->parent == node) {
            node = node->right;
        } else if (node->left != nullptr) {
            node = node->left;
            while (node->right != nullptr) 
                node = node->right;
        } else {
            base_ptr y = node->parent;
            while (y->left == node) {
                node = y;
                y = y->parent;
            }
            node = y;
        }
    }
};

template <typename T, typename Ref, typename Ptr>
struct __rb_tree_iterator : public __rb_tree_base_iterator
{
    typedef __rb_tree_iterator<T, Ref, Ptr>              self;
    typedef __rb_tree_iterator<T, T&, T*>                iterator;
    typedef __rb_tree_iterator<T, const T&, const T*>    const_iterator;
    typedef __rb_tree_node<T>* 	                         link_type;    
    typedef bidirectional_iterator_tag                   iterator_category;
    typedef T                                            value_type;
    typedef Ptr                                          pointer;
    typedef Ref                                          reference;

    __rb_tree_iterator() { }
    __rb_tree_iterator(link_type x) { node = x; } // ????
    __rb_tree_iterator(const iterator& x) { node = x.node; } // ????

    bool operator==(const self& x) const { return node == x.node; }
    bool operator!=(const self& x) const { return node != x.node; }

    reference operator*() const { return static_cast<link_type>(node)->data; }
    pointer operator->() const { return &(operator*()); }

    self& operator++()
    {
        increment();
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        increment();
        return tmp;
    }

    self& operator--()
    {
        decrement();
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        decrement();
        return tmp;
    }
};

template <typename Key, typename Value, typename KeyOfValue, 
          typename Compare, typename Alloc = alloc>
class RedBlackTree
{
private:
    typedef __rb_tree_node_base*                 base_ptr;
    typedef __rb_tree_node<Value>                rb_tree_node;
    typedef simple_alloc<rb_tree_node, Alloc>    rb_tree_node_allocator;
    typedef __rb_tree_color_type                 color_type;

public:
    typedef Key                  key_type;
    typedef Value                value_type;
    typedef value_type*          pointer;
    typedef const value_type*    const_pointer;
    typedef value_type&          reference;
    typedef const value_type&    const_reference;
    typedef size_t               size_type;
    typedef ptrdiff_t            difference_type;
    typedef rb_tree_node*        link_type;

    typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
    typedef __rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;

public:
    RedBlackTree(const Compare& comp = Compare()) 
      : node_count(0), key_compare(comp) { empty_intialize(); }

    RedBlackTree(const RedBlackTree& x) 
      : node_count(x.node_count), key_compare(x.key_compare)
    {
        if (x.root() == nullptr) {
            empty_intialize();
        } else {
            header = get_node();
            color(header) = __rb_tree_red;
            root() = __copy(x.root(), header);
            leftmost() = minimum(root());  // ????
            rightmost() = maximum(root()); // ????
        }        
    }

    ~RedBlackTree()
    {
        clear();
        put_node(header);
    }

    RedBlackTree& operator=(const RedBlackTree& x)
    {
        if (this != &x) {
            RedBlackTree tmp(x);
            swap(tmp);
        }
        return *this;

        // if (this != &x) {
        //     clear();
        //     node_count = x.node_count;
        //     key_compare = x.key_compare;
        //     if (x.root() == nullptr) {
        //         root() = nullptr;
        //         leftmost() = header;
        //         rightmost() = header;
        //     } else {
        //         root() = __copy(x.root(), header);
        //         leftmost() = minimum(root());
        //         rightmost() = maximum(root());
        //     }
        // }
        // return *this;        
    }

public:
    Compare key_comp() const { return key_compare; }
    iterator begin() { return leftmost(); }
    const_iterator begin() const { return leftmost(); }
    iterator end() { return header; }
    const_iterator end() const { return header; }
    bool empty() const { return node_count == 0; }
    size_type size() const { return node_count; }
    size_type max_size() const { return size_type(-1); }

public:
    iterator insert_equal(const value_type& val)
    {
        link_type y = header;
        link_type x = root();
        while (x != nullptr) {
            y = x;
            x = key_compare(KeyOfValue()(val), key(x)) ? left(x) : right(x);
        }
        return __insert(x, y, val);
    }    

    Pair<iterator, bool> insert_unique(const value_type& val)
    {
        link_type y = header;
        link_type x = root();
        bool comp = true;
        while (x != nullptr) {
            y = x;
            comp = key_compare(KeyOfValue()(val), key(x));
            x = comp ? left(x) : right(x);
        }

        iterator j = iterator(y);
        if (comp) {
            if (j == begin())
                return make_pair(__insert(x, y, val), true);
            else 
                --j;
        }

        if (key_compare(key(j.node), KeyOfValue()(val)))
            return make_pair(__insert(x, y, val), true);

        return make_pair(j, false);
    }

    template <typename InputIterator>
    void insert_equal(InputIterator first, InputIterator last)
    {
        for (; first != last; ++first)
            insert_equal(*first);
    }

    template <typename InputIterator>
    void insert_unique(InputIterator first, InputIterator last)
    {
        for (; first != last; ++first)
            insert_unique(*first);
    }

    void clear() 
    {
        if (node_count != 0) {
            __erase(root());
            root() = nullptr;
            leftmost() = header;
            rightmost() = header;
            node_count = 0;
        }
    }

    void swap(RedBlackTree& t)
    {
        Zyx::swap(header, t.header);
        Zyx::swap(node_count, t.node_count);
        Zyx::swap(key_compare, t.key_compare);
    }

    iterator find(const key_type& k)
    {
        link_type y = header;
        link_type x = root();
        while (x != nullptr) {
            if (!key_compare(key(x), k)) {
                y = x;
                x = left(x);
            } else {
                x = right(x);
            }
        }
        iterator j = iterator(y);
        return (j == end() || key_compare(k, key(j.node))) ? end() : j;
    }

    const_iterator find(const key_type& k) const
    {
        link_type y = header;
        link_type x = root();
        while (x != nullptr) {
            if (!key_compare(key(x), k)) {
                y = x;
                x = left(x);
            } else {
                x = right(x);
            }
        }
        const_iterator j = const_iterator(y);
        return (j == end() || key_compare(k, key(j.node))) ? end() : j;
    }    

    size_type count(const key_type& k) const
    {
        Pair<const_iterator, const_iterator> p = equal_range(k);
        return distance(p.first, p.second);
    }

    iterator lower_bound(const key_type& k)
    {
        link_type y = header;
        link_type x = root();
        while (x != nullptr) {
            if (!key_compare(key(x), k)) {
                y = x;
                x = left(x);
            } else {
                x = right(x);
            }
        }
        return iterator(y);
    }

    const_iterator lower_bound(const key_type& k) const
    {
        link_type y = header;
        link_type x = root();
        while (x != nullptr) {
            if (!key_compare(key(x), k)) {
                y = x;
                x = left(x);
            } else {
                x = right(x);
            }
        }
        return const_iterator(y);
    }

    iterator upper_bound(const key_type& k)
    {
        link_type y = header;
        link_type x = root();
        while (x != nullptr) {
            if (key_compare(k, key(x))) {
                y = x;
                x = left(x);
            } else {
                x = right(x);
            }
        }
        return iterator(y);
    }

    const_iterator upper_bound(const key_type& k) const
    {
        link_type y = header;
        link_type x = root();
        while (x != nullptr) {
            if (key_compare(k, key(x))) {
                y = x;
                x = left(x);
            } else {
                x = right(x);
            }
        }
        return const_iterator(y);
    }

    Pair<iterator, iterator> equal_range(const key_type& k)
    {
        return make_pair(lower_bound(k), upper_bound(k));
    }

    Pair<const_iterator, const_iterator> equal_range(const key_type& k) const
    {
        return make_pair(lower_bound(k), upper_bound(k));
    }

private:
    void empty_intialize()
    {
        header = get_node();
        color(header) = __rb_tree_red;
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;		
    }

    link_type __copy(link_type x, link_type p)
    {
        link_type top = clone_node(x);
        top->parent = p;
        if (x->right)
            top->right = __copy(right(x), top);
        p = top;
        x = left(x);

        while (x != nullptr) {
            link_type y = clone_node(x);
            p->left = y;
            y->parent = p;
            if (x->right)
                y->right = __copy(right(x), y);
            p = y;
            x = left(x);
        }

        return top;

        // another way:
        // link_type top = clone_node(x);
        // top->parent = p;
        // if (x->left) 
        //     top->left = __copy(left(x), top);
        // if (x->right) 
        //     top->right = __copy(right(x), top);
        // return top;
    }

    iterator __insert(base_ptr x_, base_ptr y_, const value_type& val)
    {
        link_type x = (link_type) x_;
        link_type y = (link_type) y_;    
        link_type z;

        if (y == header || x != nullptr || key_compare(KeyOfValue()(val), key(y))) {
            z = create_node(val);
            left(y) = z;
            if (y == header) {
                root() = z;
                rightmost() = z;
            } else if (y == leftmost()) {
                leftmost() = z;
            }
        } else {
            z = create_node(val);
            right(y) = z;
            if (y == rightmost()) 
                rightmost() = z;
        }
        
        parent(z) = y;
        left(z) = nullptr;
        right(z) = nullptr;
        __rb_tree_rebalance(z, header->parent);
        ++node_count;
        return iterator(z);
    }

    void __erase(link_type x)
    {
        if (x != nullptr) {
            __erase(right(x));
            link_type y = left(x);
            destroy_node(x);
            x = y;
        }
    }

private:
    void __rb_tree_rotate_left(base_ptr x, base_ptr& root)
    {
        base_ptr y = x->right;
        x->right = y->left;
        if (y->left != nullptr)
            y->left->parent = x;
        y->parent = x->parent;

        if (x == root)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void __rb_tree_rotate_right(base_ptr x, base_ptr& root)
    {
        base_ptr y = x->left;
        x->left = y->right;
        if (y->right != nullptr)
            y->right->parent = x;
        y->parent = x->parent;

        if (x == root)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->right = x;
        x->parent = y;
    }

    void __rb_tree_rebalance(base_ptr x, base_ptr& root)
    {
        x->color = __rb_tree_red;
        while (x != root && x->parent->color == __rb_tree_red) {
            if (x->parent == x->parent->parent->left) {
                base_ptr y = x->parent->parent->right;
                if (y != nullptr && y->color == __rb_tree_red) {
                    x->parent->color = __rb_tree_black;
                    y->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->right) {
                        x = x->parent;
                        __rb_tree_rotate_left(x, root);
                    }
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_right(x->parent->parent, root);
                }
            } else {
                base_ptr y = x->parent->parent->left;
                if (y != nullptr && y->color == __rb_tree_red) {
                    x->parent->color = __rb_tree_black;
                    y->color =  __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        __rb_tree_rotate_right(x, root);
                    }
                    x->parent->color = __rb_tree_black;
                    x->parent->parent->color = __rb_tree_red;
                    __rb_tree_rotate_left(x->parent->parent, root);
                }
            }
        }
        root->color = __rb_tree_black;
    }

private:
    link_type get_node() { return rb_tree_node_allocator::allocate(); }
    void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

    link_type create_node(const value_type& x)
    {
        link_type p = get_node();
        construct(&p->data, x);
        return p;
    }

    void destroy_node(link_type p)
    {
        destroy(&p->data);
        put_node(p);
    }

    link_type clone_node(link_type x)
    {
        link_type tmp = create_node(x->data);
        tmp->color = x->color;
        tmp->left = nullptr;
        tmp->right = nullptr;
        return tmp;
    }

private:
    link_type& root() const { return (link_type&)header->parent; }
    link_type& leftmost() const { return (link_type&)header->left; }
    link_type& rightmost() const { return (link_type&)header->right; }

    static link_type& left(link_type x) { return (link_type&)x->left; }
    static link_type& right(link_type x) { return (link_type&)x->right; }
    static link_type& parent(link_type x) { return (link_type&)x->parent; }
    static reference value(link_type x) { return x->data; }
    static const key_type& key(link_type x) { return KeyOfValue()(value(x)); } //????
    static color_type& color(link_type x) { return (color_type&)x->color; }

    static link_type& left(base_ptr x) { return (link_type&)x->left; }
    static link_type& right(base_ptr x) { return (link_type&)x->right; }
    static link_type& parent(base_ptr x) { return (link_type&)x->parent; }
    static reference value(base_ptr x) { return ((link_type)x)->data; }
    static const key_type& key(base_ptr x) { return KeyOfValue()(value(x)); } //????
    static color_type& color(base_ptr x) { return (color_type&)x->color; }

    static link_type minimum(link_type x) { return (link_type)__rb_tree_node_base::minimum(x); }
    static link_type maximum(link_type x) { return (link_type)__rb_tree_node_base::maximum(x); }

private:
    size_type node_count;
    link_type header;
    Compare key_compare;
};

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
inline bool operator==(const RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                       const RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
{
    return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin()); 
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
inline bool operator!=(const RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                       const RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
inline bool operator<(const RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                      const RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
{
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());  
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
inline bool operator>(const RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                      const RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
{
    return rhs < lhs; 
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
inline bool operator<=(const RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                      const RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
{
    return !(rhs < lhs); 
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
inline bool operator>=(const RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                      const RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
{
    return !(lhs < rhs);
}

template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
inline void swap(RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& x,
                 RedBlackTree<Key, Value, KeyOfValue, Compare, Alloc>& y)
{
    x.swap(y);
}

}

#endif