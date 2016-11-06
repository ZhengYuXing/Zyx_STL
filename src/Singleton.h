#ifndef ZYX_SINGLETON
#define ZYX_SINGLETON

namespace Zyx {

template <typename T>
class Singleton
{
public:
    typedef T object_type;

public:
    static object_type& instance()
    {
        static object_type obj;
        create_object.do_nothing();
        return obj;
    }

private:
    struct object_creator
    {
        object_creator()
        {
            Singleton<T>::instance();
        }

        inline void do_nothing() const
        {        	
        }
    };

private:    
    static object_creator create_object;
};

template <typename T> typename Singleton<T>::object_creator Singleton<T>::create_object;

}

#endif