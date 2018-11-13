#ifndef LINKLIST_H
#define LINKLIST_H
#include "List.h"
#include "Exception.h"
namespace DTLib
{
template <typename T>
class LinkList : public List<T>
{
protected:
    struct Node : public Object
    {
        T value;
        Node* next;
    };
    mutable struct : public Object
    {
        char reserved[sizeof(T)];
        Node* next;
    } m_header;
    int m_length;
    int m_step;
    Node* m_current;

    Node* position(int i) const             // O(n)
    {
        Node* ret = reinterpret_cast<Node*>(&m_header);
        for(int k = 0; k < i; k++)
        {
            ret = ret->next;
        }
        return ret;
    }

    virtual Node* create()
    {
        return new Node();
    }
    virtual void destroy(Node* p)
    {
        delete p;
    }
public:
    LinkList()
    {
        m_header.next = NULL;
        m_length = 0;
        m_step = 1;
        m_current = NULL;
    }
    bool insert(const T& e)
    {
        return insert(m_length, e);
    }
    bool insert(int i, const T& e)          // O(n)
    {
        bool ret = ((0 <= i)&&(i <= m_length));
        if( ret )
        {
            Node* node = create();
            if( node != NULL )
            {
                Node* current = position(i);
                node->value = e;
                node->next = current->next;
                current->next = node;

                m_length++;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to insert new element ...");
            }
        }
        return ret;
    }
    bool remove(int i)                      // O(n)
    {
        bool ret = ((0 <= i)&&(i < m_length));
        if( ret )
        {
            Node* current = position(i);
            Node* toDel = current->next;
            if( m_current == toDel )
            {
                m_current = toDel->next;
            }

            current->next = toDel->next;

            m_length--;

            destroy(toDel);
        }
        return ret;
    }
    bool set(int i, const T& e)             // O(n)
    {
        bool ret = ((0 <= i)&&(i < m_length));
        if( ret )
        {
            position(i)->next->value = e;
        }
        return ret;
    }
    virtual T get(int i) const                      // O(n)
    {
        T ret;

        if( get(i, ret) )
        {
            return ret;
        }
        else
        {
            THROW_EXCEPTION(IndexOutOfBoundsException, "Parameter i is invalid ...");
        }

    }
    bool get(int i, T& e) const             // O(n)
    {
        bool ret = ((0 <= i)&&(i < m_length));
        if( ret )
        {
            e = position(i)->next->value;
        }
        return ret;
    }

    int find(const T &e) const              // O(n)
    {
        int ret = -1;
        int i = 0;
        Node* node = m_header.next;
        while( node )
        {
            // 泛指类型值的比较，对于类类型，需要重载比较操作符
            // 因此重载版本可能会抛出异常
            if(node->value == e)
            {
                ret = i;
                break;
            }
            else
            {
                node = node->next;
                i++;
            }
        }
        return ret;
    }
    void clear()                        // O(n)
    {
        while( m_header.next )
        {
            Node* toDel = m_header.next;
            m_header.next = toDel->next;
            m_length--;
            destroy(toDel);
        }
    }
    int length() const                  // O(1)
    {
        return m_length;
    }
    virtual bool move(int i, int step = 1)
    {
        bool ret = (0 <= i)&&(i < m_length) && (step > 0);
        if( ret )
        {
            m_current = position(i)->next;
            m_step = step;
        }
        return ret;
    }
    virtual bool end()
    {
        return (m_current == NULL);
        //return m_current.isNull();
    }
    virtual T current()
    {
        if( !end() )
        {
            return m_current->value;
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "No value at current position ...");
        }
    }
    // 移动一个步长 m_step
    virtual bool next()
    {
        int i = 0;
        while( (i < m_step)&& !end() )
        {
            m_current = m_current->next;
            i++;
        }

        return (i == m_step);
    }

    ~LinkList()                         // O(n)
    {
        clear();
    }
};
}


#endif // LINKLIST_H
