#ifndef ZYX_VECTOR
#define ZYX_VECTOR 

#include "Alloc.h"
#include "Construct.h"
#include "Algorithm.h"
#include "Utility.h"

namespace Zyx {
	
template <typename T, typename Alloc = alloc>
class Vector
{
public:
	typedef T 		     value_type;
	typedef T* 		     iterator;
	typedef const T*	 const_iterator;
	typedef T* 		     pointer;
	typedef const T*	 const_pointer;
	typedef T&		     reference;
	typedef const T&     const_reference;
	typedef size_t       size_type;
	typedef ptrdiff_t    difference_type;

private:	
	typedef simple_alloc<value_type, Alloc> data_allocator;

public:
	Vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) { }

	Vector(size_type n, const T& val) { fill_initialize(n, val); }

	explicit Vector(size_type n) { fill_initialize(n, T()); }

	template <typename InputIterator>
	Vector(InputIterator first, InputIterator last) 
	    : start(nullptr), finish(nullptr), end_of_storage(nullptr)
	{
        while (first != last) {
            push_back(*first);
            ++first;
        }
    }

	Vector(const Vector& x) : start(nullptr), finish(nullptr), end_of_storage(nullptr)
	{
		for (const_iterator iter = x.begin(); iter != x.end(); ++iter)
			push_back(*iter);
	}

	Vector& operator=(const Vector& x)
	{
		if (this != &x) {
			Vector tmp(x);
			swap(tmp);
		}
		return *this;
	}

	~Vector()
	{
		destroy(start, finish);
		deallocate();
	}

public:
	iterator begin()  { return start;  }
	const_iterator begin() const { return start; }

	iterator end()  { 	return finish;  }
	const_iterator end() const { return finish; }

	size_type size() const  { return finish - start;  }
	size_type capacity() const { return end_of_storage - start; }

	bool empty() const { return start == finish; }

	reference operator[](size_type i) { return *(start + i); }
	const_reference operator[](size_type i) const { return *(start + i); }

	reference front() { return *start; }
	const_reference front() const { return *start; }

	reference back() { return *(finish - 1); }
	const_reference back() const { return *(finish - 1); }

public:
	void push_back(const T& val)
	{
		if (finish != end_of_storage) {
			construct(finish, val);
			++finish;
		} else {
			insert_aux(finish, val);
		}
	}

	void pop_back()
	{
		--finish;
		destroy(finish);
	}

	iterator insert(iterator pos, const T& val)
	{
		return insert_aux(pos, val);
	}

	void insert(iterator pos, size_type n, const T& val)
	{
		if (n != 0) {
			if (end_of_storage - finish >= n) {
				const size_type elems_after = finish - pos; 
				iterator old_finish = finish;
				if (elems_after > n) {
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					copy_backward(pos, old_finish - n, old_finish);
					fill(pos, pos + n, val);
				} else {
					uninitialized_fill_n(finish, n - elems_after, val);
					finish += n - elems_after;
					uninitialized_copy(pos, old_finish, finish);
					finish += elems_after;
					fill(pos, old_finish, val);
				}
			} else {
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);

				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;

				try {
					new_finish = uninitialized_copy(start, pos, new_start);
					new_finish = uninitialized_fill_n(new_finish, n, val);
					new_finish = uninitialized_copy(pos, finish, new_finish);
				} catch (...) {
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}

				destroy(start, finish);
				deallocate();

				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
		}
	}

	//template <typename InputIterator>
	//void insert(iterator pos, InputIterator first, InputIterator last)

	iterator erase(iterator pos)
	{
		if (pos + 1 != finish)
			copy(pos + 1, finish, pos);
		--finish;
		destroy(finish);
		return pos;
	}

	iterator erase(iterator first, iterator last)
	{
		iterator result = copy(last, finish, first);
		destroy(result, finish);
		finish = finish - (last - first);
		return first;
	}

	void clear() { erase(start, finish); }

	void swap(Vector& x)
	{
		Zyx::swap(start, x.start);
		Zyx::swap(finish, x.finish);
		Zyx::swap(end_of_storage, x.end_of_storage);
	}

private:
	void fill_initialize(size_type n, const T& val)
	{
		start = allocate_and_fill(n, val);
		finish = start + n;
		end_of_storage = finish;
	}

	iterator allocate_and_fill(size_type n, const T& val)
	{
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, val);
		return result;
	}

	void deallocate()
	{
		if (start)
			data_allocator::deallocate(start, end_of_storage - start);
	}

	iterator insert_aux(iterator pos, const T& val)
	{
		if (finish != end_of_storage) {
			++finish;
			copy_backward(pos, finish - 1, finish);
			*pos = val;
			return pos;
		} else {
			const size_type count = pos - start;
			const size_type old_size = size();
			const size_type len = old_size != 0 ? 2 * old_size : 1;

			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;

			try {
				new_finish = uninitialized_copy(start, pos, new_start);
				construct(new_finish, val);
				++new_finish;
				new_finish = uninitialized_copy(pos, finish, new_finish);
			} catch (...) {
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}

			destroy(start, finish);
			deallocate();

			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;

			return start + count;
		}
	}

private:
	iterator start;
	iterator finish;
	iterator end_of_storage;

};

template <typename T, typename Alloc>
void swap(Vector<T, Alloc>& x, Vector<T, Alloc>& y)
{
	x.swap(y);
}

}
#endif