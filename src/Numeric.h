#ifndef ZYX_NUMERIC
#define ZYX_NUMERIC 

namespace Zyx {

//---------------------------------【accumulate() function】-----------------------------------

template <typename InputIterator, typename T>
T accumulate(InputIterator first, InputIterator last, T init)
{
    while (first != last) {
        init = init + *first;
        ++first;
    }
    return init;
}

template <typename InputIterator, typename T, typename BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op)
{
    while (first != last) {
        init = binary_op(init, *first);
        ++first;
    }
    return init;
}

//-------------------------------【inner_product() function】----------------------------------

template <typename InputIterator1, typename InputIterator2, typename T>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init)
{
    for (; first1 != last1; ++first1, ++first2)
        init = init + (*first1) * (*first2);
    return init;
}

template <typename InputIterator1, typename InputIterator2, typename T,
          typename BinaryOperation1, typename BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init,
                BinaryOperation1 binary_op1, BinaryOperation2 binary_op2)
{
    for (; first1 != last1; ++first1, ++first2)
        init = binary_op1(init, binary_op2(*first1, *first2));
    return init;    
}

//--------------------------------【partial_sum() function】-----------------------------------

template <typename InputIterator, typename OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result)
{
    if (first != last) {
        typename iterator_traits<InputIterator>::value_type val = *first;
        *result = *first;
        while (++first != last) {
            val = val + *first;
            *++result = val;
        }
        ++result;
    }
    return result;
}

template <typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last, 
                           OutputIterator result, BinaryOperation binary_op)
{
    if (first != last) {
        typename iterator_traits<InputIterator>::value_type val = *first;
        *result = *first;
        while (++first != last) {
            val = binary_op(val, *first);
            *++result = val;
        }
        ++result;
    }
    return result;
}

//-----------------------------【adjacent_difference() function】------------------------------

template <typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result)
{
    if (first != last) {
        typename iterator_traits<InputIterator>::value_type prev = *first;
        *result = *first;
        while (++first != last) {
            typename iterator_traits<InputIterator>::value_type tmp = *first;
            *++result = tmp - prev;
            prev = tmp;
        }
        ++result;
    }
    return result;
}

template <typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result, BinaryOperation binary_op)
{
    if (first != last) {
        typename iterator_traits<InputIterator>::value_type prev = *first;
        *result = *first;
        while (++first != last) {
            typename iterator_traits<InputIterator>::value_type tmp = *first;
            *++result = binary_op(tmp, prev);
            prev = tmp;
        }
        ++result;
    }
    return result;
}

//-----------------------------------【power() function】--------------------------------------

template <typename T, typename Integer>
T power(T x, Integer n)
{
    return power(x, n, multiplies<T>());
}

template <typename T, typename Integer, typename MonoidOperation>
T power(T x, Integer n, MonoidOperation op)
{
    // if (n == 0) {
    //     return identity_element(op);
    // } else {
    //     while (n & 1 == 0) {
    //         n >>= 1;
    //         x = op(x, x);
    //     }
    //     T result = x;
    //     n >>= 1;
    //     while (n != 0) {
    //         x = op(x, x);
    //         if (n & 1 != 0)
    //             result = op(result, x);
    //         n >>= 1;
    //     }
    //     return result;
    // }

    T result = identity_element(op);
    if (n < 0) {
        n = -n;
        x = 1 / x;
    }
    while (n > 0) {
        if (n & 1)
            result = op(result, x);
        x = op(x, x);
        n >>= 1;
    }
    return result;
}

//-----------------------------------【iota() function】---------------------------------------

template <typename ForwardIterator, typename T>
void iota(ForwardIterator first, ForwardIterator last, T val)
{
    while (first != last) {
        *first = val;
        ++first;
        ++val;
    }
}

}

#endif