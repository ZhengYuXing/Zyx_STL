#ifndef ZYX_NON_COPYABLE
#define ZYX_NON_COPYABLE

namespace Zyx {

class NonCopyable
{
public:
    NonCopyable() { }
    ~NonCopyable() { }
    
private:
    NonCopyable(const NonCopyable&);
    NonCopyable& operator=(const NonCopyable&);
};

// in C++11:
// class NonCopyable
// {
// public:
//     NonCopyable() = default;
//     ~NonCopyable() = default;
//     NonCopyable(const NonCopyable&) = delete;
//     NonCopyable& operator=(const NonCopyable&) = delete;
// };

}

#endif