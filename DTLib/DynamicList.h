#ifndef DYNAMICLIST_H
#define DYNAMICLIST_H

#include "SeqList.h"
#include "Exception.h"
namespace DTLib
{
template <typename T>
class DynamicList : public SeqList<T>
{
protected:
    int m_capacity;                 // ˳��洢�ռ�Ĵ�С
public:
    DynamicList(int capacity)      // ����ռ�
    {
        this->m_array = new T[capacity];
        if(this->m_array != NULL)
        {
            this->m_capacity = capacity;
            this->m_length = 0;
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create DynamicList object ...");
        }
    }
    int capacity() const
    {
        return m_capacity;
    }

    void resize(int capacity)       // ���ÿռ�Ĵ�С
    {
        if(capacity != m_capacity)
        {
            T* array = new T[capacity];

            if( array != NULL )
            {
                int length = (this->m_length < capacity) ? (this->m_length) : (capacity);
                for(int i=0; i<length; i++)
                {
                    array[i] = this->m_array[i];
                }

                T* temp = this->m_array;
                this->m_array = array;
                this->m_capacity = capacity;
                this->m_length = length;

                delete[] temp;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to resize DynamicList object ...");
            }
        }
    }
    ~DynamicList()                // �黹�ռ�
    {
        delete[] this->m_array;
    }
};
}
#endif // DYNAMICLIST_H
