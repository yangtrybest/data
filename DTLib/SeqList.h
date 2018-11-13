#ifndef SEQLIST_H
#define SEQLIST_H
#include "List.h"
#include "Exception.h"
namespace DTLib
{
template <typename T>
class SeqList : public List<T>
{
protected:
    T* m_array;         //存储空间
    int m_length;       //当前线性表长度
public:
    bool insert(const T&e)
    {
        return insert(m_length, e);
    }
    bool insert(int i, const T& e)              // O(n)
    {
        bool ret = ((0 <= i)&&(i <= m_length));
        ret = ret && ((m_length + 1) <= capacity());
        if( ret )
        {
            for(int p = m_length - 1; p >= i; p--)
            {
                m_array[p + 1] = m_array[p];
            }
            m_array[i] = e;
            m_length++;
        }
        return ret;
    }

    bool remove(int i)                               // O(n)
    {
        bool ret = ((0 <= i)&&(i < m_length));
        if( ret )
        {
            m_array[i] = 0;
            for(int p = i; p < m_length - 1; p++)
            {
                m_array[p] = m_array[p+1];
            }
            m_length--;
        }
        return ret;
    }

    bool get(int i, T& e) const                      // O(1)
    {
        bool ret = ((0 <= i)&&(i < m_length));
        if( ret )
        {
            e = m_array[i];
        }

        return ret;
    }

    bool set(int i, const T &e)                          // O(1)
    {
         bool ret = ((0 <= i)&&(i < m_length));
         if( ret )
         {
             m_array[i] = e;
         }
         return ret;
    }

    int find(const T& e)const                            // O(n)
    {
        int ret = -1;
        for(int i=0; i< length(); i++)
        {
            if(m_array[i] == e)
            {
                ret = i;
                break;
            }
        }
        return ret;
    }
    void clear()                                         // O(1)
    {
        m_length = 0;   // 顺序表的清除操作就是将长度  置零
    }
    int length() const                               // O(1)
    {
        return m_length;
    }
    // 顺序存储结构的数组访问方式
    T& operator [](int i)
    {
        if(((0 <= i)&&(i < m_length)))
        {
            return m_array[i];
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid...");
        }
    }
    T operator [](int i) const
    {
        return (const_cast<SeqList<T>&>(*this))[i]; // 代码复用
    }
    // 顺序存储空间的容量
    virtual int capacity() const = 0;

};


}

#endif // SEQLIST_H
