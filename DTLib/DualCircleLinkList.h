#ifndef DUALCIRCLELINKLIST_H
#define DUALCIRCLELINKLIST_H
#include "DualLinkList.h"

namespace DTLib
{
template <typename T>
class DualCircleLinkList : public DualLinkList<T>
{
protected:
    typedef typename DualLinkList<T>::Node Node;
    int mod(int i) const
    {
        return (this->m_length == 0) ? 0 : (i%(this->m_length+1));
    }
    Node* last() const
    {
        return this->position(this->m_length - 1)->next;
    }
    void last_to_first()
    {
        Node* lastnode = last();

        if( lastnode != NULL )
        {
            this->m_header.next->pre = lastnode;
        }
        lastnode->next = this->m_header.next;
    }
public:
    bool insert(const T &e)
    {
        return insert(this->m_length, e);
    }
    bool insert(int i, const T &e)
    {
        bool ret = true;
        i = mod(i);
        ret = DualLinkList<T>::insert(i, e);
        if( ret && (i == 0) )
        {
            last_to_first();
        }
        return ret;
    }
    bool remove(int i)
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
                    last_to_first();
                    if( this->m_current == toDel )
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
        else
        {
            ret = DualLinkList<T>::remove(i);
        }

        return ret;
    }
    bool set(int i, const T &e)                         // O(n)
    {
        return DualLinkList<T>::set(mod(i), e);
    }

    bool get(int i, T &e) const                         // O(n)
    {
        return DualLinkList<T>::get(mod(i), e);
    }
    T get(int i) const                                  // O(n)
    {
        return DualLinkList<T>::get(mod(i));
    }
    int find(const T& e) const                          // O(n)
    {
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
        return DualLinkList<T>::move(mod(i), step);
    }
    bool end()                                              // O(1)
    {
        return (this->m_length == 0)||(this->m_current == NULL);
    }
    ~DualCircleLinkList()                                   // O(n)
    {
        clear();
    }
};
}
#endif // DUALCIRCLELINKLIST_H
