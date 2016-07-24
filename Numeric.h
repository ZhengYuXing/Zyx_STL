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

}

#endif