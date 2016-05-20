#ifndef ZYX_FUNCTIONAL
#define ZYX_FUNCTIONAL 

namespace Zyx {

template <typename Arg, typename Result>
struct unary_function
{
	typedef Arg argument_type;
	typedef Result result_type;
};

template <typename Arg1, typename Arg2, typename Result>
struct binary_function
{
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};

template <typename T>
struct negate : public unary_function<T, T>
{
	T operator()(const T& x) const { return -x; }
};

template <typename T>
struct equal_to : public binary_function<T, T, T>
{
	bool operator()(const T& x, const T& y) { return x == y; }
};

template <typename T>
struct not_equal_to : public binary_function<T, T, T>
{
	bool operator()(const T& x, const T& y) { return x != y; }
};

template <typename T>
struct less : public binary_function<T, T, T>
{
	bool operator()(const T& x, const T& y) { return x < y; }
};

template <typename T>
struct less_equal : public binary_function<T, T, T>
{
	bool operator()(const T& x, const T& y) { return x <= y; }
};

template <typename T>
struct greater : public binary_function<T, T, T>
{
	bool operator()(const T& x, const T& y) { return x > y; }
};

template <typename T>
struct greater_equal : public binary_function<T, T, T>
{
	bool operator()(const T& x, const T& y) { return x >= y; }
};

}

#endif