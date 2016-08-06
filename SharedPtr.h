#ifndef ZYX_SHARED_PTR
#define ZYX_SHARED_PTR

namespace Zyx {

template <typename T>
class SharedPtr
{
public:
    explicit SharedPtr(T* p = nullptr) : ptr(p), count(p ? new size_t(1) : nullptr) { }

    SharedPtr(const SharedPtr& sp) : ptr(sp.ptr), count(sp.count) 
    {
        if (count != nullptr) 
            ++*count; 
    }

    SharedPtr& operator=(SharedPtr sp)
    {
        swap(sp);
        return *this;
    }

    ~SharedPtr() { reset(); }

public:
    void swap(SharedPtr& sp)
    {
        T* tp = ptr;
        ptr = sp.ptr;
        sp.ptr = tp;

        size_t* tc = count;
        count = sp.count;
        sp.count = tc;
    }

    void reset()
    {
        if (count != nullptr) {
        	if (--*count == 0) {
                delete ptr;
                delete count;
        	}
        	ptr = nullptr;
        	count = nullptr;
        }
    }

    void reset(T* p)
    {
        if (ptr != p) {
        	reset();
        	ptr = p;
        	count = new size_t(1);
        }
    }

    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    size_t use_count() const { return count ? *count : 0; }
    bool unique() const { return count != nullptr && *count == 1; }
    operator bool() const { return ptr != nullptr; }

private:
    T* ptr;
    size_t* count;
};

template <typename T>
inline void swap(SharedPtr<T>& sp1, SharedPtr<T>& sp2)
{
    sp1.swap(sp2);
}

}

#endif