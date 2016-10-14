#ifndef ZYX_ANY
#define ZYX_ANY

#include <typeinfo>

namespace Zyx {

class Any
{
public:
    class PlaceHolder
    {
    public:
        virtual ~PlaceHolder() { }
        virtual const std::type_info& type() const = 0;
        virtual PlaceHolder* clone() const = 0;
    };

    template <typename ValueType>
    class Holder : public PlaceHolder
    {
    public:
        Holder(const ValueType& value) : held(value) { }
        // Holder(ValueType&& value) : held(static_cast<ValueType&&>(value)) { }
        virtual const std::type_info& type() const { return typeid(ValueType); }
        virtual PlaceHolder* clone() const { return new Holder(held); }

    private:
        Holder& operator=(const Holder&);

    public:
        ValueType held;
    };

private:
    // template <typename ValueType>
    // friend ValueType any_cast(const Any& operand);

    template <typename ValueType>
    friend ValueType* any_cast(Any*);

public:
    Any() : content(nullptr) 
    {
    }

    template <typename ValueType>
    Any(const ValueType& value) : content(new Holder<ValueType>(value)) 
    { 
    }

    Any(const Any& other) : content(other.content ? other.content->clone() : nullptr) 
    {
    }

    // Any(Any&& other) : content(other.content)
    // {
    //     other.content = nullptr;
    // }

    template <typename ValueType>
    Any& operator=(const ValueType& rhs)
    {
        Any(rhs).swap(*this);
        return *this;
    }

    Any& operator=(const Any& rhs)
    {
        Any(rhs).swap(*this);
        return *this;
    }

    // Any& operator=(Any&& rhs)
    // {
    //     rhs.swap(*this);
    //     Any().swap(rhs);
    //     return *this;
    // }

    ~Any()
    {
        delete content;
    }

public:
    bool empty() const 
    {
        return !content; 
    }

    void clear() 
    { 
        Any().swap(*this); 
    }

    Any& swap(Any& rhs)
    {
        Zyx::swap(content, rhs.content);
        return *this;
    }

    const std::type_info& type() const 
    {
        return content ? content->type() : typeid(void);
    }
    
private:
    PlaceHolder* content;
};

inline void swap(Any& lhs, Any& rhs)
{
    lhs.swap(rhs);
}

class bad_any_cast : public std::bad_cast
{
public:
    virtual const char* what() const
    {
        return "Zyx::bad_any_cast: "
               "failed conversion using Zyx::bad_any_cast";
    }
};

template <typename ValueType>
ValueType* any_cast(Any* operand)
{
    return operand && operand->type() == typeid(ValueType) 
        ? &static_cast<Any::Holder<ValueType>*>(operand->content)->held 
        : nullptr;
}

template <typename ValueType>
inline const ValueType* any_cast(const Any* operand)
{
    return any_cast<ValueType>(const_cast<Any*>(operand));
}

template <typename ValueType>
ValueType any_cast(const Any& operand)	
{
    const ValueType* result = any_cast<ValueType>(&operand);
    if (!result)
        throw bad_any_cast();
    return *result;
}

// template <typename ValueType>
// ValueType any_cast(const Any& operand)
// {
//     assert(operand.type() == typeid(ValueType));
//     return static_cast<Any::Holder<ValueType>*>(operand.content)->held;
// }

}

#endif