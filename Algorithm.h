#ifndef ZYX_ALGORITHM
#define ZYX_ALGORITHM 

#include <cstdlib>
#include "Iterator.h"
#include "Utility.h"

namespace Zyx {

//------------------------------------【equal() function】------------------------------------

template <typename InputIterator1, typename InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
    for (; first1 != last1; ++first1, ++first2)
        if (!(*first1 == *first2))
            return false;
    return true;
}

//--------------------------【lexicographical_compare() function】----------------------------

template <typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, 
                             InputIterator2 first2, InputIterator2 last2)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2)
            return true;
        if (*first2 < *first1)
            return false;        
    }
    return first1 == last1 && first2 != last2;
}

template <typename InputIterator1, typename InputIterator2, typename Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, 
                             InputIterator2 first2, InputIterator2 last2, Compare comp)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (comp(*first1, *first2))
            return true;
        if (comp(*first2, *first1))
            return false;        
    }
    return first1 == last1 && first2 != last2;
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

//-------------------------------------【remove() function】----------------------------------

template <typename ForwardIterator, typename T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& val)
{
    ForwardIterator result = first;
    while (first != last) {
        if (!(*first == val)) {
            *result = *first;        // in c++11 : *result = move(*first)
            ++result;
        }
        ++first;
    }
    return result;
}

//-----------------------------------【remove_if() function】---------------------------------

template <typename ForwardIterator, typename UnaryPredicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, UnaryPredicate pred)
{
    ForwardIterator result = first;
    while (first != last) {
        if (!pred(*first)) {
            *result = *first;        // in c++11 : *result = move(*first)
            ++result;
        }
        ++first;
    }
    return result;
}

//----------------------------------【remove_copy() function】--------------------------------

template <typename InputIterator, typename OutputIterator, typename T>
OutputIterator remove_copy(InputIterator first, InputIterator last, 
                           OutputIterator result, const T& val)
{
    while (first != last) {
        if (!(*first == val)) {
            *result = *first;
            ++result;
        }
        ++first;
    }
    return result;
}

//--------------------------------【remove_copy_if() function】-------------------------------

template <typename InputIterator, typename OutputIterator, typename UnaryPredicate>
OutputIterator remove_copy_if(InputIterator first, InputIterator last,
                              OutputIterator result, UnaryPredicate pred)
{
    while (first != last) {
        if (!pred(*first)) {
            *result = *first;
            ++result;
        }
        ++first;
    }
    return result;
}

//---------------------------------【transform() function】-----------------------------------

template <typename InputIterator, typename OutputIterator, typename UnaryPredicate>
OutputIterator transform(InputIterator first, InputIterator last, 
                         OutputIterator result, UnaryPredicate op)
{
    for (; first != last; ++first, ++result)
        *result = op(*first);
    return result;
}

template <typename InputIterator1, typename InputIterator2, 
          typename OutputIterator, typename BinaryPredicate>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, 
                         OutputIterator result, BinaryPredicate binary_op)
{
    for (; first1 != last1; ++first1, ++first2, ++result)
        *result = binary_op(*first1, *first2);
    return result;
}

//-----------------------------------【unique() function】------------------------------------

template <typename ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last)
{
    if (first == last)
        return last;
    ForwardIterator result = first;
    while (++first != last) 
        if (!(*result == *first))
            *(++result) = *first;
    return ++result;
}

template <typename ForwardIterator, typename BinaryPredicate>
ForwardIterator unique(ForwardIterator first, ForwardIterator last, BinaryPredicate pred)
{
    if (first == last)
        return last;
    ForwardIterator result = first;
    while (++first != last) 
        if (!pred(*result, *first))
            *(++result) = *first;
    return ++result;
}

//--------------------------------【unique_copy() function】----------------------------------

template <typename InputIterator, typename OutputIterator>
OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result)
{
    if (first == last)
        return result;
    return __unique_copy(first, last, result, iterator_category(result));
}

template <typename InputIterator, typename ForwardIterator>
ForwardIterator __unique_copy(InputIterator first, InputIterator last, 
                              ForwardIterator result, forward_iterator_tag)
{
    *result = *first;
    while (++first != last) 
        if (!(*result == *first))
            *(++result) = *first;
    return ++result;
}

template <typename InputIterator, typename OutputIterator>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
                             OutputIterator result, output_iterator_tag)
{
    return __unique_copy(first, last, result, output_iterator_tag, value_type(first));
}

template <typename InputIterator, typename OutputIterator, typename T>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
                             OutputIterator result, output_iterator_tag, T*)
{
    T value = *first;
    *result = *first;
    while (++first != last) {
        if (!(value == *first)) {
            value = *first;
            *(++result) = value;
        }
    }
    return ++result;
}

template <typename InputIterator, typename OutputIterator, typename BinaryPredicate>
OutputIterator unique_copy(InputIterator first, InputIterator last, 
                           OutputIterator result, BinaryPredicate pred)
{
    if (first == last)
        return result;
    typename iterator_traits<InputIterator>::value_type value = *first;
    *result = value;
    while (++first != last) {
        if (!pred(value, *first)) {
            value = *first;
            *(++result) = value;
        }
    }
    return ++result;
}

//----------------------------------【reverse() function】------------------------------------

template <typename BidirectionalIterator>
void reverse(BidirectionalIterator first, BidirectionalIterator last)
{
    __reverse(first, last, iterator_category(first));
}

template <typename BidirectionalIterator>
void __reverse(BidirectionalIterator first, BidirectionalIterator last, 
               bidirectional_iterator_tag)
{
    while (first != last && first != --last) {
        iter_swap(first, last);
        ++first;
    }
}

template <typename RandomAccessIterator>
void __reverse(RandomAccessIterator first, RandomAccessIterator last, 
               random_access_iterator_tag)
{
    while (first < last) 
        iter_swap(first++, --last);
}

//--------------------------------【reverse_copy() function】---------------------------------

template <typename BidirectionalIterator, typename OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, 
                            OutputIterator result)
{
    while (first != last) {
        --last;
        *result = *last;
        ++result;
    }
    return result;
}

//---------------------------------【set_union() function】-----------------------------------

template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, 
                         InputIterator2 first2, InputIterator2 last2, 
                         OutputIterator result)
{
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
        } else if (*first2 < *first1) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
            ++first2;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

//------------------------------【set_intersection() function】-------------------------------

template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result)
{
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            ++first1;
        } else if (*first2 < *first1) {
            ++first2;
        } else {
            *result = *first1;
            ++first1;
            ++first2;
            ++result;
        }
    }
    return result;
}

//-------------------------------【set_difference() function】--------------------------------

template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              OutputIterator result)
{
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
            ++result;
        } else if (*first2 < *first1) {
            ++first2;
        } else {
            ++first1;
            ++first2;
        }
    }
    return copy(first1, last1, result);
}

//--------------------------【set_symmetric_difference() function】---------------------------

template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
                                        InputIterator2 first2, InputIterator2 last2,
                                        OutputIterator result)
{
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
            ++result;
        } else if (*first2 < *first1) {
            *result = *first2;
            ++first2;
            ++result;
        } else {
            ++first1;
            ++first2;
        }
    }
    return copy(first2, last2, copy(first1, last1, result));
}

//--------------------------------【lower_bound() function】----------------------------------

template <typename ForwardIterator, typename T>
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& val)
{
    return __lower_bound(first, last, val, difference_type(first), iterator_category(first));
}

template <typename ForwardIterator, typename T, typename Distance>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& val, 
                              Distance*, forward_iterator_tag)
{
    Distance len = distance(first, last);
    while (len > 0) {
        Distance half = len >> 1;
        ForwardIterator middle = first;
        advance(middle, half);
        if (*middle < val) {
            first = ++middle;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
}

template <typename RandomAccessIterator, typename T, typename Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, 
                                   const T& val, Distance*, random_access_iterator_tag)
{
    Distance len = last - first;
    while (len > 0) {
        Distance half = len >> 1;
        RandomAccessIterator middle = first + half;
        if (*middle < val) {
            first = middle + 1;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
}

template <typename ForwardIterator, typename T, typename Compare>
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& val, Compare comp)
{
    typedef iterator_traits<ForwardIterator>::difference_type difference_type; 
    difference_type len = distance(first, last);
    while (len > 0) {
        difference_type half = len / 2;
        ForwardIterator middle = first;
        advance(middle, half);
        if (comp(*middle, val)) {
            first = ++middle;
            len = len - half - 1;
        } else {
            len = half;
        }
    }
    return first;
}

//--------------------------------【upper_bound() function】----------------------------------

template <typename ForwardIterator, typename T>
ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& val)
{
    return __upper_bound(first, last, val, difference_type(first), iterator_category(first));
}

template <typename ForwardIterator, typename T, typename Distance>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T& val,
                              Distance*, forward_iterator_tag)
{
    Distance len = distance(first, last);
    while (len > 0) {
        Distance half = len >> 1;
        ForwardIterator middle = first;
        advance(middle, half);
        if (val < *middle) {
            len = half;
        } else {
            first = ++middle;
            len = len - half - 1;
        }
    }
    return first;
}

template <typename RandomAccessIterator, typename T, typename Distance>
RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, 
                                   const T& val, Distance*, random_access_iterator_tag)
{
    Distance len = last - first;
    while (len > 0) {
        Distance half = len >> 1;
        RandomAccessIterator middle = first + half;
        if (val < *middle) {
            len = half;
        } else {
            first = middle + 1;
            len = len - half - 1;
        }
    }
    return first;
}

template <typename ForwardIterator, typename T, typename Compare>
ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& val, Compare comp)
{
    typedef iterator_traits<ForwardIterator>::difference_type difference_type; 
    difference_type len = distance(first, last);
    while (len > 0) {
        difference_type half = len / 2;
        ForwardIterator middle = first;
        advance(middle, half);
        if (comp(val, *middle)) {
            len = half;            
        } else {
            first = ++middle;
            len = len - half - 1;
        }
    }
    return first;
}

//-------------------------------【binary_search() function】---------------------------------

template <typename ForwardIterator, typename T>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& val)
{
    ForwardIterator iter = lower_bound(first, last, val);
    return iter != last && !(val < *iter);
}

template <typename ForwardIterator, typename T, typename Compare>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& val, Compare comp)
{
    ForwardIterator iter = lower_bound(first, last, val);
    return iter != last && !comp(val, *iter);
}

//--------------------------------【equal_range() function】----------------------------------

template <typename ForwardIterator, typename T>
Pair<ForwardIterator, ForwardIterator> 
equal_range(ForwardIterator first, ForwardIterator last, const T& val)
{
    ForwardIterator iter = lower_bound(first, last, val);
    return make_pair(iter, upper_bound(iter, last, val));
}

template <typename ForwardIterator, typename T, typename Compare>
Pair<ForwardIterator, ForwardIterator> 
equal_range(ForwardIterator first, ForwardIterator last, const T& val, Compare comp)
{
    ForwardIterator iter = lower_bound(first, last, val, comp);
    return make_pair(iter, upper_bound(iter, last, val, comp));
}

//-------------------------------【next_permutation() function】------------------------------

template <typename BidirectionalIterator>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last)
{
    if (first == last)
        return false;

    BidirectionalIterator i = first;
    if (i == last)
        return false;

    i = last;
    --i;
    while (true) {
        BidirectionalIterator ii = i;
        --i;
        if (*i < *ii) {
            BidirectionalIterator j = last;
            while (!(*i < *--j))
                ;
            iter_swap(i, j);
            reverse(ii, last);
            return true;
        }
        if (i == first) {
            reverse(first, last);
            return false;
        }
    }

    /* another way:
    if (first == last)
        return false;

    BidirectionalIterator i = first;
    if (i == last)
        return false;

    i = last;
    --i;  
    BidirectionalIterator ii = i;
    --i;  
    while (i != first && !(*i < *ii)) {
        --i;
        --ii;
    }

    if (i == first && !(*i < *ii)) {
        reverse(first, last);
        return false;
    }

    BidirectionalIterator j = last;
    while (!(*i < *--j))
        ;
    iter_swap(i, j);
    reverse(ii, last);
    return true;
    */
}

//-------------------------------【prev_permutation() function】------------------------------

template <typename BidirectionalIterator>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last)
{
    if (first == last)
        return false;

    BidirectionalIterator i = first;
    ++i;
    if (i == last)
        return false;

    i = last;
    --i;
    while (true) {
        BidirectionalIterator ii = i;
        --i;        
        if (*ii < *i) {
            BidirectionalIterator j = last;
            while (!(*--j < *i))
                ;
            iter_swap(i, j);
            reverse(ii, last);
            return true;
        }
        if (i == first) {
            reverse(first, last);
            return false;
        }
    }
}

//--------------------------------【random_shuffle() function】-------------------------------

template <typename RandomAccessIterator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last)
{
    __random_shuffle(first, last, difference_type(first));
}

template <typename RandomAccessIterator, typename Distance>
void __random_shuffle(RandomAccessIterator first, RandomAccessIterator last, Distance*)
{
    if (first == last)
        return;
    for (RandomAccessIterator i = first + 1; i != last; ++i)
        iter_swap(i, first + Distance(rand() % ((i - first) + 1)));
}

template <typename RandomAccessIterator, typename RandomNumberGenerator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last, 
                    RandomNumberGenerator &rand)
{
    if (first == last)
        return;
    for (RandomAccessIterator i = first + 1; i != last; ++i)
        iter_swap(i, first + rand((i - first) + 1));
}

//---------------------------------【partitional() function】---------------------------------

template <typename BidirectionalIterator, typename Predicate>
BidirectionalIterator partition(BidirectionalIterator first, 
                                BidirectionalIterator last, Predicate pred)
{
    while (first != last) {
        while (pred(*first)) {
            ++first;
            if (first == last) return first;
        }
        do {
            --last;
            if (first == last) return first;
        } while (!pred(*last));
        iter_swap(first, last);
        ++first;
    }
    return first;

    // another way:
    // while (true) {
    //     while (true) {
    //         if (first == last)
    //             return first;
    //         else if (pred(*first))
    //             ++first;
    //         else
    //             break;
    //     }
    //     --last;
    //     while (true) {
    //         if (first == last)
    //             return first;
    //         else if (!pred(*last))
    //             --last;
    //         else
    //             break;
    //     }
    //     iter_swap(first, last);
    //     ++first;
    // }
}

//---------------------------------【partial_sort() function】--------------------------------

template <typename RandomAccessIterator>
void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, 
                  RandomAccessIterator last)
{
    __partial_sort(first, middle, last, value_type(first));
}

template <typename RandomAccessIterator, typename T>
void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
                    RandomAccessIterator last, T*)
{
    make_heap(first, middle);
    for (RandomAccessIterator iter = middle; iter < last; ++iter)
        if (*iter < *first)
            __pop_heap(first, middle, iter, T(*iter), difference_type(first));
    sort_heap(first, middle);
}

template <typename InputIterator, typename RandomAccessIterator>
RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last,
                                       RandomAccessIterator result_first,
                                       RandomAccessIterator result_last)
{
    
} 

//-------------------------------------【sort() function】------------------------------------

template <typename RandomAccessIterator>
void sort(RandomAccessIterator first, RandomAccessIterator last)
{
    if (first != last) {
        __introsort_loop(first, last, value_type(first), __lg(last - first) * 2);
        __final_insertion_sort(first, last);
    }
}

// another way
template <typename RandomAccessIterator>
void quick_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    if (first != last) {
        __quick_sort_loop(first, last);
        __final_insertion_sort(first, last);
    }
}

template <typename RandomAccessIterator>
void __quick_sort_loop(RandomAccessIterator first, RandomAccessIterator last)
{
    __quick_sort_loop_aux(first, last, value_type(first));
}

template <typename RandomAccessIterator, typename T>
void __quick_sort_loop_aux(RandomAccessIterator first, RandomAccessIterator last, T*)
{
    while (last - first > __stl_threshold) {
        RandomAccessIterator cut = __unguarded_partition(first, last, T(__median(*first, 
                                                         *(first + (last - first)/2), *(last - 1))));
        if (cut - first >= last - cut) {
            __quick_sort_loop(cut, last);
            last = cut;
        } else {
            __quick_sort_loop(first, cut);
            first = cut;
        }
    }
}

const int __stl_threshold = 16;

template <typename Size>
Size __lg(Size n)
{
    Size k;
    for (k = 0; n > 1; n >>= 1)
        ++k;
    return k;
}

template <typename T>
const T& __median(const T& a, const T& b, const T& c)
{
    if (a < b)
        if (b < c)
            return b;
        else if (a < c)
            return c;
        else
            return a;
    else if (a < c)
        return a;
    else if (b < c)
        return c;
    else
        return b;
}

template <typename RandomAccessIterator, typename T, typename Size>
void __introsort_loop(RandomAccessIterator first, RandomAccessIterator last, T*, Size depth_limit)
{
    while (last - first > __stl_threshold) {
        if (depth_limit == 0) {
            partial_sort(first, last, last);
            return;
        }
        --depth_limit;
        RandomAccessIterator cut = __unguarded_partition(first, last, T(__median(*first, 
                                                         *(first + (last - first)/2), *(last - 1))));
        __introsort_loop(cut, last, value_type(first), depth_limit);
        last = cut;
    }
}

template <typename RandomAccessIterator, typename T>
RandomAccessIterator __unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, T pivot)
{
    while (true) {
        while (*first < pivot) 
            ++first;
        --last;
        while (pivot < *last)
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

template <typename RandomAccessIterator>
void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    if (last - first > __stl_threshold) {
        __insertion_sort(first, first + __stl_threshold);
        __unguarded_insertion_sort(first + __stl_threshold, last);
    } else {
        __insertion_sort(first, last);
    }
}

template <typename RandomAccessIterator>
void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    if (first == last)
        return;
    for (RandomAccessIterator iter = first + 1; iter != last; ++iter)
        __linear_insert(first, iter, value_type(first));
}

template <typename RandomAccessIterator, typename T>
void __linear_insert(RandomAccessIterator first, RandomAccessIterator last, T*)
{
    T value = *last;
    if (value < *first) {
        copy_backward(first, last, last + 1);
        *first = value;
    } else {
        __unguarded_linear_insert(last, value);
    }
}

template <typename RandomAccessIterator, typename T>
void __unguarded_linear_insert(RandomAccessIterator last, T value)
{
    RandomAccessIterator next = last - 1;
    while (value < *next) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}

template <typename RandomAccessIterator>
void __unguarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    __unguarded_insertion_sort_aux(first, last, value_type(first));
}

template <typename RandomAccessIterator, typename T>
void __unguarded_insertion_sort_aux(RandomAccessIterator first, RandomAccessIterator last, T*)
{
    for (RandomAccessIterator iter = first; iter != last; ++iter)
        __unguarded_linear_insert(iter, T(*iter));
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