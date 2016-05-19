#ifndef ZYX_QUEUE
#define ZYX_QUEUE 

#include "Deque.h"

namespace Zyx {

template <typename T, typename Container = Deque<T>>
class Queue
{
public:
	typedef typename Container::value_type 		   value_type;
	typedef typename Container::size_type 		   size_type;
	typedef typename Container::reference          reference;
	typedef typename Container::const_reference    const_reference;
	typedef Container 							   container_type;

public:
	Queue() : c() { }

	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }

	reference front() { return c.front(); }
	const_reference front() const { return c.front(); }

	reference back() { return c.back(); }
	const_reference back() const { return c.back(); }

	void push(const T& val) { c.push_back(val); }
	void pop() { c.pop_front(); }

	void swap(Queue& x) { Zyx::swap(c, x.c); }

private:
	Container c;
};

}

#endif