#ifndef ZYX_HEAP
#define ZYX_HEAP 

#include "Iterator.h"

namespace Zyx 
{

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
    while (child < len) 
    {
        if (child + 1 < len && *(first + child) < *(first + child + 1))
        {
            ++child;
        }

        if (*(first + child) > value)
        {
            *(first + holeIndex) = *(first + child);
        }
        else
        {
            break;
        }

        holeIndex = child;
        child = 2 * holeIndex + 1;
    }
    *(first + holeIndex) = value;
}

//---------------------------------【push_heap() function】-----------------------------------
//                          Complexity : At most log(N) comparisons

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
    while (holeIndex > topIndex && *(first + parent) < value) 
    {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template <typename RandomAccessIterator, typename Compare>
void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    __push_heap(first, comp, last - first - 1, 0, *(last - 1));
}

template <typename RandomAccessIterator, typename Compare, typename Distance, typename T>
void __push_heap(RandomAccessIterator first, Compare comp, 
                 Distance holeIndex, Distance topIndex, T value)
{
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) 
    {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

//---------------------------------【pop_heap() function】------------------------------------
//                        Complexity : At most 2×log(N) comparisons

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

    while (secondChild < len) 
    {
        if (*(first + secondChild) < *(first + secondChild - 1))
        {
            --secondChild;
        }

        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * holeIndex + 2;
    }

    if (secondChild == len) 
    {
        *(first + holeIndex) = *(first + secondChild - 1);
        holeIndex = secondChild - 1;
    }

    __push_heap(first, holeIndex, topIndex, value);
}

template <typename RandomAccessIterator, typename Compare>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    typename iterator_traits<RandomAccessIterator>::value_type value = *(last - 1);
    *(last - 1) = *first;
    __adjust_heap(first, comp, 0, last - first - 1, value);

    // typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
    // typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;

    // difference_type holeIndex = 0;
    // difference_type topIndex = holeIndex;
    // difference_type secondChild = 2 * holeIndex + 2;
    // difference_type len = last - first - 1;

    // while (secondChild < len) 
    // {
    //     if (comp(*(first + secondChild), *(first + secondChild - 1)))
    //     {
    //         --secondChild;
    //     }

    //     *(first + holeIndex) = *(first + secondChild);
    //     holeIndex = secondChild;
    //     secondChild = 2 * holeIndex + 2;
    // }

    // if (secondChild == len) 
    // {
    //     *(first + holeIndex) = *(first + secondChild - 1);
    //     holeIndex = secondChild - 1;
    // }

    // difference_type parent = (holeIndex - 1) / 2;
    // while (holeIndex > topIndex && comp(*(first + parent), value)) 
    // {
    //     *(first + holeIndex) = *(first + parent);
    //     holeIndex = parent;
    //     parent = (holeIndex - 1) / 2;
    // }
    // *(first + holeIndex) = value;
}

template <typename RandomAccessIterator, typename Compare, typename Distance, typename T>
void __adjust_heap(RandomAccessIterator first, Compare comp,
                   Distance holeIndex, Distance len, T value)
{
    Distance child = 2 * holeIndex + 1;
    while (child < len) 
    {
        if (child + 1 < len && comp(*(first + child), *(first + child + 1)))
        {
            ++child;
        }

        if (comp(value, *(first + child))) 
        {
            *(first + holeIndex) = *(first + child);
            holeIndex = child;
            child = 2 * holeIndex + 1;
        } 
        else 
        {
            break;
        }
    }
    *(first + holeIndex) = value;
}

//---------------------------------【make_heap() function】-----------------------------------
//                           Complexity : At most 3*N comparisons
    
template <typename RandomAccessIterator>
void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __make_heap(first, last, value_type(first), difference_type(first));
}

template <typename RandomAccessIterator, typename T, typename Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*)
{
    if (last - first < 2)
    {
        return;
    }

    Distance len = last - first;
    Distance parent = (len - 2) / 2;
    while (parent >= 0) 
    {
        __adjust_heap(first, parent, len, T(*(first + parent)));
        --parent;
    }
}

template <typename RandomAccessIterator, typename Compare>
void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    if (last - first < 2)
    {
        return;
    }

    typedef typename iterator_traits<RandomAccessIterator>::difference_type difference_type;
    difference_type len = last - first;
    difference_type parent = (len - 2) / 2;
    while (parent >= 0) 
    {
        __adjust_heap(first, comp, parent, len, *(first + parent));
        --parent;
    }
}

//---------------------------------【sort_heap() function】-----------------------------------
//                         Complexity : At most N×log(N) comparisons

template <typename RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    while (last - first > 1)
    {
        pop_heap(first, last--);
    }
}

template <typename RandomAccessIterator, typename Compare>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    while (last - first > 1)
    {
        pop_heap(first, last--, comp);
    }
}

}

#endif