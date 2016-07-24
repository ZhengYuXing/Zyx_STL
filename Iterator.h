#ifndef ZYX_ITERATOR
#define ZYX_ITERATOR

namespace Zyx {
	
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <typename Category, typename T, typename Distance = ptrdiff_t,
		  typename Pointer = T*, typename Reference = T&>
struct iterator
{
    typedef Category     iterator_category;
    typedef T            value_type;
    typedef Distance     difference_type;
    typedef Pointer      pointer;
    typedef Reference    reference;
};

template <typename Iterator>
struct iterator_traits
{
    typedef typename Iterator::iterator_category    iterator_category;
    typedef typename Iterator::value_type           value_type;
    typedef typename Iterator::difference_type      difference_type;
    typedef typename Iterator::pointer	            pointer;
    typedef typename Iterator::reference            reference;
};

template <typename T>
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag    iterator_category;
    typedef T                             value_type;
    typedef ptrdiff_t                     difference_type;
    typedef T* 	                          pointer;
    typedef T&                            reference;
};

template <typename T>
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag    iterator_category;
    typedef T                             value_type;
    typedef ptrdiff_t                     difference_type;
    typedef const T*                      pointer;
    typedef const T&                      reference;
};

template <typename Iterator>
typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator& iter)
{
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}

template <typename Iterator>
typename iterator_traits<Iterator>::value_type*
value_type(const Iterator& iter)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template <typename Iterator>
typename iterator_traits<Iterator>::difference_type*
difference_type(const Iterator& iter)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

//---------------------------------【distance() function】------------------------------------

template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
    return __distance(first, last, iterator_category(first));
}

template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++n;
        ++first;
    }
    return n;
}

template <typename RandomAccessIterator>
typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
    return last - first;
}

//---------------------------------【advance() function】------------------------------------

template <typename InputIterator, typename Distance>
void advance(InputIterator& iter, Distance n)
{
    __advance(iter, n, iterator_category(iter));
}

template <typename InputIterator, typename Distance>
void __advance(InputIterator& iter, Distance n, input_iterator_tag)
{
    while (n--) ++iter;
}

template <typename BidirectionalIterator, typename Distance>
void __advance(BidirectionalIterator& iter, Distance n, bidirectional_iterator_tag)
{
    if (n >= 0)
        while (n--) ++iter;
    else
        while (n++) --iter;
}

template <typename RandomAccessIterator, typename Distance>
void __advance(RandomAccessIterator& iter, Distance n, random_access_iterator_tag)
{
    iter += n;
}

}

#endif