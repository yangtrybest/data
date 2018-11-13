#ifndef STATICLINKLIST_H
#define STATICLINKLIST_H
#include "LinkList.h"
#include "Exception.h"
namespace DTLib
{
template <typename T, int N>
class StaticLinkList : public LinkList<T>
{
protected:
    typedef typename LinkList<T>::Node Node;
    struct SNode : public Node
    {
        void* operator new(unsigned int size, void* loc)
        {
            (void)size;
            return loc;
        }
    };

    unsigned char m_space[sizeof(SNode) * N];
    int m_used[N];

    Node* create()
    {
        SNode* ret = NULL;
        for(int i=0; i<N; i++)
        {
            if( !m_used[i] )
            {
                // ֻ��ָ����Ҫʹ�õĵ�ַ�� m_space ����
                ret = reinterpret_cast<SNode*>(m_space) + i;
                // ��������Ƭ�ռ� ���ù��캯����
                ret = new(ret) SNode();
                m_used[i] = 1;
                break;
            }
        }
        return ret;
    }

    void destroy(Node *p)
    {
        SNode* pn = dynamic_cast<SNode*>(p);
        SNode* space = reinterpret_cast<SNode*>(m_space);
        for(int i=0; i<N; i++)
        {
            if( pn == (space + i) )
            {
                m_used[i] = 0;
                pn->~SNode();
                break;
            }
        }
    }
public:
    StaticLinkList()
    {
        for(int i=0; i<N; i++)
        {
            m_used[i] = 0;
        }
    }
    int capacity() const
    {
        return N;
    }
    ~StaticLinkList()
    {
        this->clear();
    }

};

}
#endif // STATICLINKLIST_H
