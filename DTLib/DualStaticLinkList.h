#ifndef DUALSTATICLINKLIST_H
#define DUALSTATICLINKLIST_H
#include "DualLinkList.h"
#include "Exception.h"
namespace DTLib
{
template <typename T, int N>
class DualStaticLinkList : public DualLinkList<T>
{
protected:
    typedef typename DualLinkList<T>::Node Node;
    struct SNode : public Node
    {
        void* operator new(unsigned int size, void* p)
        {
            (void)size;
            return p;
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
                ret = reinterpret_cast<SNode*>(m_space) + i;
                ret = new(ret) SNode();
                m_used[i] = 1;
                break;
            }
        }
        return ret;
    }
    void destory(Node* p)
    {
        SNode* space = reinterpret_cast<SNode*>(m_space);
        SNode* pn = dynamic_cast<SNode*>(p);
        for(int i=0; i<N; i++)
        {
            if( pn == (space + i))
            {
                m_used[i] = 0;
                pn->~SNode();
                break;
            }
        }
    }
public:
    DualStaticLinkList()
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
    ~DualStaticLinkList()
    {
        this->clear();
    }

};

}
#endif // DUALSTATICLINKLIST_H
