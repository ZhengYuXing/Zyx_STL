#ifndef ZYX_RED_BLACK_TREE
#define ZYX_RED_BLACK_TREE 

#include "Alloc.h"
#include "Construct.h"
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
    typedef __rb_tree_node_base::base_ptr base_ptr;

    base_ptr node;

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
    typedef __rb_tree_iterator<T, T&, T*>                iterator;
    typedef __rb_tree_iterator<T, const T&, const T*>    const_iterator;
    typedef __rb_tree_iterator<T, Ref, Ptr>              self;
    typedef __rb_tree_node<T>* 	                         link_type;    
    typedef bidirectional_iterator_tag                   iterator_category;
    typedef T                                            value_type;
    typedef Ptr                                          pointer;
    typedef Ref                                          reference;

    __rb_tree_iterator() : __rb_tree_base_iterator(nullptr) { }
    __rb_tree_iterator(link_type x) : __rb_tree_base_iterator(x) { }
    __rb_tree_iterator(const iterator& x) : __rb_tree_base_iterator(x.node) { }

    reference operator*() const { return ((link_type)node)->data; }
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

public:
    RedBlackTree(const Compare& comp = Compare()) 
        : node_count(0), key_compare(comp) { init(); }

    ~RedBlackTree()
    {
        clear();
        put_node(header);
    }

public:
    iterator begin() { return leftmost(); }
    iterator end() { return header; }

    bool empty() const { return node_count == 0; }
    size_type size() const { return node_count; }
    size_type max_size() const { return size_type(-1); }

public:
    iterator insert_equal(const value_type& val)
    {

    }    

private:
    void init()
    {
        header = get_node();
        color(header) = __rb_tree_red;
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;		
    }

    iterator __insert(base_ptr x, base_ptr y, const value_type& val)
    {

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
        link_type tmp = create_node(x.data);
        tmp->color = x->color;
        tmp->left = nullptr;
        tmp->right = nullptr;
        return tmp;
    }

    link_type& root() const { return (link_type&)header->parent; }
    link_type& leftmost() const { return (link_type&)header->left; }
    link_type& rightmost() const { return (link_type&)header->right; }

private:
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

}

#endif