#ifndef ZYX_ALGORITHM
#define ZYX_ALGORITHM 

#include "Iterator.h"

namespace Zyx {

//-------------------------------------【max() function】-------------------------------------

template <typename T>
const T& max(const T& a, const T& b)
{
    return a < b ? b : a;
}

//-------------------------------------【min() function】-------------------------------------

template <typename T>
const T& min(const T& a, const T& b)
{
    return b < a ? b : a;
}

//--------------------------------【max_elements() function】---------------------------------

template <typename ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
{
    if (first == last)
        return last;

    ForwardIterator largest = first;
    while (++first != last) 
        if (*largest < *first)
            largest = first;
    return largest;
}

template <typename ForwardIterator, typename Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp)
{
    if (first == last)
        return last;

    ForwardIterator largest = first;
    while (++first != last)
        if (comp(*largest, *first))
            largest = first;
    return largest;
}

//--------------------------------【min_elements() function】---------------------------------

template <typename ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last)
{
    if (first == last)
        return last;

    ForwardIterator smallest = first;
    while (++first != last) 
        if (*first < *smallest)
            smallest = first;
    return smallest;
}

template <typename ForwardIterator, typename Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp)
{
    if (first == last)
        return last;

    ForwardIterator smallest = first;
    while (++first != last)
        if (comp(*first, *smallest))
            smallest = first;
    return smallest;
}

//-------------------------------------【find() function】------------------------------------

template <typename InputIterator, typename T>
InputIterator find(InputIterator first, InputIterator last, const T& val)
{
    while (first != last && !(*first == val)) 
        ++first;
    return first;
}

//----------------------------------【find_if() function】------------------------------------

template <typename InputIterator, typename UnaryPredicate>
InputIterator find_if(InputIterator first, InputIterator last, UnaryPredicate pred)
{
    while (first != last && !pred(*first))
        ++first;
    return first;
}

//--------------------------------【find_if_not() function】----------------------------------

template <typename InputIterator, typename UnaryPredicate>
InputIterator find_if_not(InputIterator first, InputIterator last, UnaryPredicate pred)
{
    while (first != last && pred(*first))
        ++first;
    return first;
}

//------------------------------【find_first_of() function】----------------------------------

template <typename InputIterator, typename ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2)
{
    while (first1 != last1) {
        for (ForwardIterator iter = first2; iter != last2; ++iter)
            if (*iter == *first1) 
                return first1;
        ++first1;
    }
    return last1;
}

template <typename InputIterator, typename ForwardIterator, typename BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2, 
                            BinaryPredicate pred)
{
    while (first1 != last1) {
        for (ForwardIterator iter = first2; iter != last2; ++iter)
            if (pred(*iter, *first1))
                return first1;
            ++first1;
    }    
    return last1;
}

//---------------------------------【find_end() function】------------------------------------

template <typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                          ForwardIterator2 first2, ForwardIterator2 last2)
{
    if (first2 == last2)
        return last1;

    ForwardIterator1 result = last1;
    while (first1 != last1) {
        ForwardIterator1 iter1 = first1;
        ForwardIterator2 iter2 = first2;
        while (*iter1 == *iter2) {
            ++iter1;
            ++iter2;
            if (iter2 == last2) {
                result = first1;
                break;
            }
            if (iter1 == last1) {
                return result;                
            }
        }
        ++first1;
    }
    return result;
}

template <typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                          ForwardIterator2 first2, ForwardIterator2 last2,
                          BinaryPredicate pred)
{
    if (first2 == last2)
        return last1;

    ForwardIterator1 result = last1;
    while (first1 != last1) {
        ForwardIterator1 iter1 = first1;
        ForwardIterator2 iter2 = first2;
        while (pred(*iter1, *iter2)) {
            ++iter1;
            ++iter2;
            if (iter2 == last2) {
                result = first1;
                break;
            }
            if (iter1 == last1) {
                return result;
            }
        }
        ++first1;
    }
    return result;
}

//----------------------------------【search() function】------------------------------------

template <typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                        ForwardIterator2 first2, ForwardIterator2 last2)
{
    if (first2 == last2)
        return first1;

    while (first1 != last1) {
        ForwardIterator1 iter1 = first1;
        ForwardIterator2 iter2 = first2;
        while (*iter1 == *iter2) {
            ++iter1;
            ++iter2;
            if (iter2 == last2) 
                return first1;
            if (iter1 == last1)
                return last1;
        }
        ++first1;
    }
    return last1;
}

template <typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                        ForwardIterator2 first2, ForwardIterator2 last2,
                        BinaryPredicate pred)
{
    if (first2 == last2)
        return first1;

    while (first1 != last1) {
        ForwardIterator1 iter1 = first1;
        ForwardIterator2 iter2 = first2;
        while (pred(*iter1, *iter2)) {
            ++iter1;
            ++iter2;
            if (iter2 == last2) 
                return first1;
            if (iter1 == last1)
                return last1;
        }
        ++first1;
    }
    return last1;
}

//----------------------------------【search_n() function】-----------------------------------

template <typename ForwardIterator, typename Size, typename T>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size count, const T& val)
{
    if (count <= 0)
        return first;

    ForwardIterator limit = first;
    Zyx::advance(limit, Zyx::distance(first, last) - count + 1);
    while (first != limit) {
        ForwardIterator iter = first;
        Size i = 0;
        while (*iter == val) {
            ++i;
            ++iter;
            if (i == count) 
                return first;
        }
        ++first;
    }
    return last;
}

template <typename ForwardIterator, typename Size, typename T, typename BinaryPredicate>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last, 
                         Size count, const T& val, BinaryPredicate pred)
{
    if (count <= 0)
        return first;

    ForwardIterator limit = first;
    Zyx::advance(limit, Zyx::distance(first, last) - count + 1);
    while (first != limit) {
        ForwardIterator iter = first;
        Size i = 0;
        while (pred(*iter, val)) {
            ++i;
            ++iter;
            if (i == count) 
                return first;
        }
        ++first;
    }
    return last;	
}

//-------------------------------------【copy() function】------------------------------------

template <typename InputIterator, typename OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
    while (first != last) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

//---------------------------------【copy_backward() function】-------------------------------

template <typename BidirectionalIterator1, typename BidirectionalIterator2>
BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, 
                                     BidirectionalIterator1 last,
                                     BidirectionalIterator2 result)
{
    while (last != first)
        *(--result) = *(--last);
    return result;
}

//-------------------------------------【fill() function】------------------------------------

template <typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T& val)
{
    while (first != last) {
        *first = val;
        ++first;
    }
}

//-------------------------------------【fill_n() function】----------------------------------

template <typename OutputIterator, typename Size, typename T>
OutputIterator fill_n(OutputIterator first, Size n, const T& val)
{
    while (n > 0) {
        *first = val;
        ++first;
        --n;
    }
    return first;
}

//----------------------------------【for_each() function】-----------------------------------

template <typename InputIterator, typename Function>
Function for_each(InputIterator first, InputIterator last, Function fn)
{
    while (first != last) {
        fn(*first);
        ++first;
    }
    return fn;
}

//-------------------------------【perlocate_down() function】--------------------------------

template <typename RandomAccessIterator>
void perlocate_down(RandomAccessIterator first, RandomAccessIterator last)
{
	__perlocate_down_aux(first, last, difference_type(first), value_type(first));
}

template <typename RandomAccessIterator, typename Distance, typename T>
void __perlocate_down_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
	T value = *(last - 1);
	*(last - 1) = *first;
	__perlocate_down(first, Distance(0), Distance((last - first) - 1), value);
}

template <typename RandomAccessIterator, typename Distance, typename T>
void __perlocate_down(RandomAccessIterator first, Distance holeIndex, Distance len, T value)
{
	Distance child = 2 * holeIndex + 1;
	while (child < len) {
		if (child + 1 < len && *(first + child) < *(first + child + 1))
			++child;

		if (*(first + child) > value)
			*(first + holeIndex) = *(first + child);
		else
			break;

		holeIndex = child;
		child = 2 * holeIndex + 1;
	}
	*(first + holeIndex) = value;
}

//---------------------------------【push_heap() function】-----------------------------------

template <typename RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	__push_heap_aux(first, last, difference_type(first), value_type(first));
}

template <typename RandomAccessIterator, typename Distance, typename T>
void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
	__push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
}

// perlocate up
template <typename RandomAccessIterator, typename Distance, typename T>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value)
{
	Distance parent = (holeIndex - 1) / 2;
	while (holeIndex > topIndex && *(first + parent) < value) {
		*(first + holeIndex) = *(first + parent);
		holeIndex = parent;
		parent = (holeIndex - 1) / 2;
	}
	*(first + holeIndex) = value;
}

//---------------------------------【pop_heap() function】------------------------------------

template <typename RandomAccessIterator>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	__pop_heap_aux(first, last, value_type(first));
}

template <typename RandomAccessIterator, typename T>
void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*)
{
	__pop_heap(first, last - 1, last - 1, T(*(last - 1)), difference_type(first));
}

template <typename RandomAccessIterator, typename T, typename Distance>
void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, 
		  		RandomAccessIterator result, T value, Distance*)
{
	*result = *first;
	__adjust_heap(first, Distance(0), Distance(last - first), value);
}

// perlocate down
template <typename RandomAccessIterator, typename Distance, typename T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value)
{
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2;
	while (secondChild < len) {
		if (*(first + secondChild) < *(first + secondChild - 1))
			--secondChild;
		*(first + holeIndex) = *(first + secondChild);
		holeIndex = secondChild;
		secondChild = 2 * holeIndex + 2;
	}
	if (secondChild == len) {
		*(first + holeIndex) = *(first + (secondChild - 1));
		holeIndex = secondChild - 1;
	}
	//*(first + holeIndex) = value;
	__push_heap(first, holeIndex, topIndex, value);
}

//---------------------------------【sort_heap() function】-----------------------------------

template <typename RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	while (last - first > 1)
		pop_heap(first, last--);
}

//---------------------------------【make_heap() function】-----------------------------------

template <typename RandomAccessIterator>
void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	__make_heap(first, last, value_type(first), difference_type(first));
}

template <typename RandomAccessIterator, typename T, typename Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*)
{
	if (last - first < 2)
		return;

	Distance len = last - first;
	Distance parent = (len - 2) / 2;
	while (parent >= 0) {
		__adjust_heap(first, parent, len, T(*(first + parent)));
		--parent;
	}
}

}

#endif 