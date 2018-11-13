#ifndef CIRCLELIST_H
#define CIRCLELIST_H
#include "LinkList.h"

namespace DTLib
{
template <typename T>
class CircleList : public LinkList<T>
{
protected:
    typedef typename LinkList<T>::Node Node;
    int mod(int i) const                            // O(1)
    {
        return (this->m_length == 0) ? 0 : (i%(this->m_length + 1));
    }
    Node* last() const                              // O(n)
    {
        return this->position(this->m_length-1)->next;
    }

    void last_to_first()                            // O(n)
    {
        last()->next = this->m_header.next;
    }
public:
//    CircleList();
    bool insert(const T &e)
    {
        return insert(this->m_length, e);
    }
    bool insert(int i, const T &e)                  // O(n)
    {
        bool ret = true;
        i = mod(i);
        ret = LinkList<T>::insert(i, e);

        if( ret && (i == 0))
        {
            last_to_first();
        }
        return ret;
    }
    bool remove(int i)                                  // O(n)
    {
        bool ret = true;
        i = mod(i);

        if( i == 0 )
        {
            Node* toDel = this->m_header.next;

            if( toDel != NULL )
            {
                this->m_header.next = toDel->next;
                this->m_length--;

                if(this->m_length > 0)
                {
                    last_to_first();                    // O(n)
                    if(this->m_current == toDel)
                    {
                        this->m_current = toDel->next;
                    }
                }
                else
                {
                    this->m_header.next = NULL;
                    this->m_current = NULL;
                }

                this->destroy(toDel);
            }
            else
            {
                ret = false;
            }
        }
        else                                            // O(n)
        {
            ret = LinkList<T>::remove(i);
        }

        return ret;
    }
    bool set(int i, const T &e)                         // O(n)
    {
        return LinkList<T>::set(mod(i), e);
    }

    bool get(int i, T &e) const                         // O(n)
    {
        return LinkList<T>::get(mod(i), e);
    }
    T get(int i) const                                  // O(n)
    {
        return LinkList<T>::get(mod(i));
    }
    int find(const T& e) const                          // O(n)
    {
        // 不使用父类的版本，是因为父类版本出现异常的时候，会破坏
        // 循环链表的结构；使用父类版本的代码如下：
//        int ret = -1;
//        last()->next = NULL;
//        ret = LinkList<T>::find(e);
//        last_to_first();
//        return ret;
        int ret = -1;
        Node* slider = this->m_header.next;
        for(int i=0; i<this->m_length; i++)
        {
            if( slider->value == e)
            {
                ret = i;
                break;
            }
            slider = slider->next;
        }
        return ret;
    }

    void clear()                                            // O(n)
    {
        while( this->m_length > 1)      // O(n)
        {
            remove(1);         // O(1) // remove(1)是为了效率，详细看remove()函数的实现
        }
        if( this->m_length == 1)        // O(1)
        {
            Node* toDel = this->m_header.next;

            this->m_header.next = NULL;
            this->m_length = 0;
            this->m_current = NULL;
            this->destroy(toDel);
        }
    }
    bool move(int i, int step = 1)                          // O(n)
    {
        return LinkList<T>::move(mod(i), step);
    }
    bool end()                                              // O(1)
    {
        return (this->m_length == 0)||(this->m_current == NULL);
    }

    ~CircleList()                                           // O(n)
    {
        clear();
    }
};
}
#endif // CIRCLE_H
