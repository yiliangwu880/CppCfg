
#pragma once

#include <string>

class Any
{
public:
    //默认构造函数
    Any() : m_tpIndex(std::type_index(typeid(void))) {}
    Any(const Any& other) : m_ptr(other.clone()), m_tpIndex(other.m_tpIndex) {}
    Any(Any&& other) : m_ptr(std::move(other.m_ptr)), m_tpIndex(std::move(other.m_tpIndex)) {}

    //通用的右值构造
    template<class T, class = typename std::enable_if<!std::is_same<std::decay<T>::type, Any>::value, T>::type>
    Any(T&& t)
        : m_ptr(new Derived<std::decay<T>::type>(std::forward<T>(t)))
        , m_tpIndex(typeid(std::decay<T>::type)) {}

    //判断是否为空
    bool isNull()
    {
        return !bool(m_ptr);
    }

    //是否可以类型转换
    template<class T>
    bool is()
    {
        return m_tpIndex == type_index(typeid(T));
    }

    //类型转换
    template<class T>
    T& cast()
    {
        if (!is<T>())
        {
            cout << "can not cast " << typeid(T).name() << " to "
                << m_tpIndex.name() << endl;
            throw bad_cast();
        }
        auto ptr = dynamic_cast<Derived<T>*>(m_ptr.get());
        return ptr->m_value;
    }

    Any& operator=(const Any& other)
    {
        if (m_ptr == other.m_ptr)
        {
            return *this;
        }
        m_ptr = other.clone();
        m_tpIndex = other.m_tpIndex;
        return *this;
    }

private:
    struct Base;
    using BasePtr = std::unique_ptr<Base>;

    //非模板擦除类型
    struct Base
    {
        virtual BasePtr clone() const = 0;
    };

    template<typename T>
    struct Derived : public Base
    {
        template<typename...Args>
        Derived(Args&&...args) : m_value(std::forward<Args>(args)...)
        {
        }
        BasePtr clone() const
        {
            return BasePtr(new Derived(m_value));
        }

        T m_value;
    };

    //拷贝使用
    BasePtr clone() const
    {
        if (m_ptr)
        {
            return m_ptr->clone();
        }
        return nullptr;
    }

    BasePtr         m_ptr;      //具体数据
    std::type_index m_tpIndex;  //数据类型
};
————————————————
版权声明：本文为CSDN博主「allenxguo」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/gx864102252/article/details/79560686