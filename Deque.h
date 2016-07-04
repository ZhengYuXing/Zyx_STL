#ifndef ZYX_DEQUE
#define ZYX_DEQUE 

#include "Iterator.h"
#include "Alloc.h"
#include "Construct.h"
#include "Algorithm.h"
#include "Utility.h"

namespace Zyx {

size_t __deque_buf_size(size_t n, size_t sz)
{
	return n != 0 ? n : (sz < 512 ? 512 / sz : 1);
}

template <typename T, typename Ref, typename Ptr, size_t BufSiz = 0>
struct __deque_iterator
{
	typedef __deque_iterator<T, T&, T*, BufSiz> 			   iterator;
	typedef __deque_iterator<T, const T&, const T*, BufSiz>    const_iterator;
	typedef __deque_iterator 								   self;

	typedef random_access_iterator_tag    					   iterator_category;
	typedef T 							  					   value_type;
	typedef Ptr 						  					   pointer;
	typedef Ref 						  					   reference;
	typedef size_t 						  					   size_type;
	typedef ptrdiff_t					  					   difference_type;
	typedef T**							  					   map_pointer;

	T* cur;
	T* first;
	T* last;
	map_pointer node;

	__deque_iterator() : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) { }
	
	__deque_iterator(const iterator& x) 
	  : cur(x.cur), first(x.first), last(x.last), node(x.node) { }

	static size_type buffer_size() { return __deque_buf_size(BufSiz, sizeof(T)); }

	void set_node(map_pointer new_node)
	{
		node = new_node;
		first = *new_node;
		last = first + buffer_size();
	}

	reference operator*() const { return *cur; }
	pointer operator->() const { return &(operator*()); }

	difference_type operator-(const self& x) const
	{
		return (node - x.node - 1) * difference_type(buffer_size()) 
		                                             + (cur - first) 
		                                             + (x.last - x.cur);
	}

	self& operator++()
	{
		++cur;
		if (cur == last) {
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}

	self operator++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator--()
	{
		if (cur == first) {
			set_node(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}

	self operator--(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}

	self& operator+=(difference_type n)
	{
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size())) {
			cur += n;
		} else {
			difference_type node_offset = offset >= 0 
			                            ? offset / difference_type(buffer_size()) 
	                                    : -difference_type((-offset - 1) / buffer_size()) - 1;
			set_node(node + node_offset);
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}

	self operator+(difference_type n) const
	{
		self tmp = *this;
		return tmp += n;
	}

	self& operator-=(difference_type n) { return *this += -n; }

	self operator-(difference_type n) const
	{
		self tmp = *this;
		return tmp -= n;
	}

	reference operator[](difference_type n) const { return *(*this + n); }

	bool operator==(const self& x) const { return cur == x.cur; }
	bool operator!=(const self& x) const { return !(*this == x); }

	bool operator<(const self& x) const
	{
		return (node == x.node) ? (cur < x.cur) : (node < x.node);
	}
};

template <typename T, typename Alloc = alloc, size_t BufSiz = 0>
class Deque
{
public:
	typedef T 												   value_type;
	typedef __deque_iterator<T, T&, T*, BufSiz> 			   iterator;
	typedef __deque_iterator<T, const T&, const T*, BufSiz>    const_iterator;	
	typedef T* 												   pointer;
	typedef const T* 										   const_pointer;
	typedef T& 												   reference;
	typedef const T& 										   const_reference;
	typedef size_t 											   size_type;
	typedef ptrdiff_t 										   difference_type;

private:
	typedef pointer*						   map_pointer;
	typedef simple_alloc<value_type, Alloc>    data_allocator;
	typedef simple_alloc<pointer, Alloc> 	   map_allocator;

public:
	Deque() { create_map_and_nodes(0); }
	
	Deque(size_type n, const T& val) { fill_initialize(n, val); }

	Deque(const Deque& x)
	{
		create_map_and_nodes(0);
		for (const_iterator iter = x.begin(); iter != x.end(); ++iter)
			push_back(*iter);
	}

	Deque& operator=(const Deque& x)
	{
		if (this != &x) {
			Deque tmp(x);
			swap(tmp);
		}
		return *this;

		/*
		if (this != &x) {
			clear();
			for (const_iterator iter = x.begin(); iter != x.end(); ++iter)
				push_back(*iter);
		}
		return *this;
		*/
	}

	~Deque()
	{
		clear();
		deallocate_node(start.first);
		map_allocator::deallocate(map, map_size);
	}

public:
	iterator begin() { return start; }
	const_iterator begin() const { return start; }

	iterator end() { return finish; }
	const_iterator end() const { return finish; }

	bool empty() const { return start == finish; }
	size_type size() const { return finish - start; }
	size_type max_size() const { return size_type(-1); }

	reference front() { return *start; }
	const_reference front() const { return *start; }

	reference back() { return *(finish - 1); }
	const_reference back() const { return *(finish - 1);  }

	reference operator[](size_type n) { return *(start + n); }
	const_reference operator[](size_type n) const { return *(start + n); }

public:
	void push_back(const T& val)
	{
		if (finish.cur != finish.last - 1) {
			construct(finish.cur, val);
			++finish.cur;
		} else {
			push_back_aux(val);
		}
	}

	void push_front(const T& val)
	{
		if (start.cur != start.first) {
			construct(start.cur, val);
			--start.cur;
		} else {
			push_front_aux(val);
		}
	}

	void pop_back()
	{
		if (finish.cur != finish.first) {
			--finish.cur;
			destroy(finish.cur);
		} else {
			pop_back_aux();
		}
	}

	void pop_front()
	{
		if (start.cur != start.last - 1) {
			destroy(start.cur);
			++start.cur;
		} else {
			pop_front_aux();
		}
	}

	void clear()
	{
		for (map_pointer node = start.node + 1; node < finish.node; ++node) {
			destroy(*node, *node + buffer_size());
			deallocate_node(*node);
		}

		if (start.node != finish.node) {
			destroy(start.cur, start.last);
			destroy(finish.first, finish.cur);
			deallocate_node(finish.first);
		} else {
			destroy(start.cur, finish.cur);
		}

		finish = start;
	}

	iterator insert(iterator pos, const T& val)
	{
		if (pos.cur == start.cur) {
			push_front(val);
			return start;
		} else if (pos.cur == finish.cur) {
			push_back(val);
			return finish - 1;
		} else {
			return insert_aux(pos, val);
		}
	}

	iterator erase(iterator pos)
	{
		iterator next = pos + 1;
		difference_type index = pos - start;
		if (index < size() / 2) {
			copy_backward(start, pos, next);
			pop_front();
		} else {
			copy(next, finish, pos);
			pop_back();
		}
		return start + index;
	}

	iterator erase(iterator first, iterator last)
	{
		if (first == start && last == finish) {
			clear();
			return finish;
		}	

		difference_type n = last - first;
		difference_type elems_before = first - start;
		if (elems_before < (size() - n) / 2) {
			copy_backward(start, first, last);
			iterator new_start = start + n;
			destroy(start, new_start);
			for (map_pointer cur = start.node; cur < new_start.node; ++cur)
				deallocate_node(*cur);
			start = new_start;
		} else {
			copy(last, finish, first);
			iterator new_finish = finish - n;
			destroy(new_finish, finish);
			for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
				deallocate_node(*cur);
			finish = new_finish;
		}
		return start + elems_before;
	}

	void swap(Deque& x)
	{
		Zyx::swap(start, x.start);
		Zyx::swap(finish, x.finish);
		Zyx::swap(map, x.map);
		Zyx::swap(map_size, x.map_size);
	}

private:
	static size_type buffer_size() { return __deque_buf_size(BufSiz, sizeof(T)); }

	T* allocate_node() { return data_allocator::allocate(buffer_size()); }
	void deallocate_node(T* p) { data_allocator::deallocate(p, buffer_size()); }

	void create_map_and_nodes(size_type num_elements)
	{
		const size_type initial_map_size = 8;		
		const size_type num_nodes = num_elements / buffer_size() + 1;
		map_size = max(initial_map_size, num_nodes + 2);
		
		map = map_allocator::allocate(map_size);		
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;

		for (map_pointer cur = nstart; cur <= nfinish; ++cur)
			*cur = allocate_node();

		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + num_elements % buffer_size();
	}

	void fill_initialize(size_type n, const T& val)
	{
		create_map_and_nodes(n);
		for (map_pointer cur = start.node; cur < finish.node; ++cur)
			uninitialized_fill(*cur, *cur + buffer_size(), val);
		uninitialized_fill(finish.first, finish.cur, val);
	}

	void push_back_aux(const T& val)
	{
		reserve_map_at_back();
		*(finish.node + 1) = allocate_node();
		construct(finish.cur, val);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}

	void push_front_aux(const T& val)
	{
		reserve_map_at_front();
		*(start.node - 1) = allocate_node();
		start.set_node(start.node - 1);
		start.cur = start.last - 1;
		construct(start.cur, val);
	}

	void pop_back_aux()
	{
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		destroy(finish.cur);
	}

	void pop_front_aux()
	{
		destroy(start.cur);
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}

	iterator insert_aux(iterator pos, const T& val)
	{
		difference_type index = pos - start;
		if (index < size() / 2) {
			push_front(front());
			iterator front1 = start + 1;
			iterator front2 = start + 2;
			pos = start + index;
			iterator pos1 = pos + 1;
			copy(front2, pos1, front1);
		} else {
			push_back(back());
			iterator back1 = finish - 1;
			iterator back2 = finish - 2;
			pos = start + index;
			copy_backward(pos, back2, back1);
		}
		*pos = val;
		return pos;	
	}

private:
	void reserve_map_at_back(size_type nodes_to_add = 1)
	{
		if (map_size - (finish.node - map) < nodes_to_add + 1)
			reallocate_map(nodes_to_add, false);
	}

	void reserve_map_at_front(size_type nodes_to_add = 1)
	{
		if (start.node - map < nodes_to_add)
			reallocate_map(nodes_to_add, true);
	}

	void reallocate_map(size_type nodes_to_add, bool add_at_front)
	{
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;
		map_pointer new_start;
		if (map_size > 2 * new_num_nodes) {
			new_start = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			if (new_start < start.node)
				copy(start.node, finish.node + 1, new_start);
			else
				copy_backward(start.node, finish.node + 1, new_start + old_num_nodes);
		} else {
			size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_start = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			copy(start.node, finish.node + 1, new_start);
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
		}
		start.set_node(new_start);
		finish.set_node(new_start + old_num_nodes - 1);
	}

private:
	iterator start;
	iterator finish;	
	map_pointer map;
	size_type map_size;
};

template <typename T, typename Alloc, size_t BufSiz>
inline bool operator==(const Deque<T, Alloc, BufSiz>& lhs, const Deque<T, Alloc, BufSiz>& rhs)
{
    typedef typename Deque<T, Alloc>::const_iterator const_iterator;
    const_iterator end1 = lhs.end();
    const_iterator end2 = rhs.end();
    const_iterator i1 = lhs.begin();
    const_iterator i2 = rhs.begin();    
    while (i1 != end1 && i2 != end2 && *i1 == *i1) {
    	++i1;
    	++i2;
    }    
    return i1 == end1 && i2 == end2;

	// if (lhs.size() != rhs.size())
	// 	return false;
	
	// for (Deque<T, Alloc, BufSiz>::size_type i = 0; i < lhs.size(); i++)
	// 	if (lhs[i] != rhs[i])
	// 		return false;
	// return true;
}

template <typename T, typename Alloc>
inline bool operator!=(const Deque<T, Alloc>& lhs, const Deque<T, Alloc>& rhs)
{
    return !(lhs == rhs);
}

template <typename T, typename Alloc>
inline bool operator<(const Deque<T, Alloc>& lhs, const Deque<T, Alloc>& rhs)
{
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename Alloc>
inline bool operator>(const Deque<T, Alloc>& lhs, const Deque<T, Alloc>& rhs)
{
    return rhs < lhs;
}

template <typename T, typename Alloc>
inline bool operator<=(const Deque<T, Alloc>& lhs, const Deque<T, Alloc>& rhs)
{
    return !(rhs < lhs);
}

template <typename T, typename Alloc>
inline bool operator>=(const Deque<T, Alloc>& lhs, const Deque<T, Alloc>& rhs)
{
    return !(lhs < rhs);
}

template <typename T, typename Alloc>
inline void swap(Deque<T, Alloc>& x, Deque<T, Alloc>& y)
{
	x.swap(y);
}

}

#endif