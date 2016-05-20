#ifndef ZYX_PRIORIY_QUEUE
#define ZYX_PRIORIY_QUEUE 

#include "Vector.h"
#include "Algorithm.h"
#include "Functional.h"
#include <iostream>

namespace Zyx {

template <typename T, typename Container = Vector<T>, 
		  typename Compare = less<typename Container::value_type>>
class PriorityQueue
{
public:
	typedef typename Container::value_type 		   value_type;
	typedef typename Container::size_type 		   size_type;
	typedef typename Container::reference 		   reference;
	typedef typename Container::const_reference    const_reference;
	typedef Container  				   			   container_type;

 public:
 	PriorityQueue() : c() { }

 	template <typename InputIterator>
 	PriorityQueue(InputIterator first, InputIterator last) 
 	  : c(first, last) { make_heap(first, last); }

 public:
 	bool empty() const { return c.empty(); }
 	size_type size() const { return c.size(); }

 	const_reference top() const { return c.front(); }

 	void push(const T& val) 
 	{
 		c.push_back(val);
 		push_heap(c.begin(), c.end());
 	}

 	void pop()
 	{
 		pop_heap(c.begin(), c.end());
 		c.pop_back();
 	}

private:
	Container c;
	Compare comp;
}; 

}

#endif