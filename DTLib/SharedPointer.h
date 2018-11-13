#ifndef SHAREDPOINTER_H
#define SHAREDPOINTER_H
#include "Pointer.h"
#include <cstdlib>
#include "Exception.h"
namespace DTLib
{
template <typename T>
class SharedPointer : public Pointer<T>
{
protected:
    int *m_ref;  // 计数变量也是在堆空间申请的，且生命周期和堆空间对象是一样的；

    void assign(const SharedPointer<T>& obj)
    {
        this->m_pointer = obj.m_pointer;
        this->m_ref = obj.m_ref;
        if( this->m_ref )
        {
            (*this->m_ref)++;
        }
    }
public:
    SharedPointer(T* p = NULL) : m_ref(NULL)
    {
        if( p )
        {
            this->m_ref = reinterpret_cast<int*>(std::malloc(sizeof(int)));
            if( this->m_ref )
            {
                *(this->m_ref) = 1;
                this->m_pointer = p;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No enough memory to create SharedPointer object ...");
            }
        }
    }
    SharedPointer(const SharedPointer<T>& obj) : Pointer<T>(NULL)
    {
        assign(obj);
    }
    SharedPointer<T>& operator = (const SharedPointer<T>& obj)
    {
        if( this != &obj )
        {
            clear();
            assign(obj);
        }

        return *this;
    }

    void clear()
    {
        T* toDel = this->m_pointer;
        int* ref = this->m_ref;

        this->m_pointer = NULL;
        this->m_ref = NULL;

        if( ref )
        {
            (*ref)--;
            if( *ref == 0 )
            {
                free(ref);
                delete toDel;
            }
        }
    }

    ~SharedPointer()
    {
        clear();
    }
};

template <typename T>
bool operator == (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return (l.get() == r.get());
}
template <typename T>
bool operator != (const SharedPointer<T>& l, const SharedPointer<T>& r)
{
    return !(l == r);
}

}


#endif // SHAREDPOINTER_H
