#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H
#include "Graph.h"
#include "Exception.h"
#include "DynamicArray.h"
namespace DTLib
{
template <int N, typename V, typename E>
class MatrixGraph : public Graph<V, E>
{
protected:
    V* m_vertexes[N];
    E* m_edges[N][N];
    int m_ecount;

public:
    MatrixGraph()
    {
        for(int i=0; i<vCount(); i++)
        {
            m_vertexes[i] = NULL;
            for(int j=0; j<vCount(); j++)
            {
                m_edges[i][j] = NULL;
            }
        }
        m_ecount = 0;
    }
    V getVertex(int i)                  // O(1)
    {
        V ret;
        if(!getVertex(i, ret))
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }
        return ret;
    }
    bool getVertex(int i, V& value)     // O(1)
    {
        bool ret = ((0 <= i) && (i < vCount()));

        if( ret )
        {
            if( m_vertexes[i] != NULL )
            {
                value = *(m_vertexes[i]);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this vertex ...");
            }
        }
        return ret;
    }
    bool setVertex(int i, const V& value)   // O(1)
    {
        bool ret = ((0 <= i) && (i < vCount()));
        if( ret )
        {
            V* data = m_vertexes[i];
            if( data == NULL )
            {
                data = new V();
            }

            if( data != NULL )
            {
                *data = value;
                // 当赋值出现异常的时候，图对象的状态就不合法了，因此通过临时变量
                // data在不发生异常的时候向下执行：(为了异常安全)
                m_vertexes[i] = data;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to store new vertex value ...");
            }
        }
        return ret;
    }
    SharedPointer<Array<int>> getAdjacent(int i)    // O(n)
    {
        DynamicArray<int>* ret = NULL;
        if((0 <= i) && (i < vCount()))
        {
            int n = 0;
            for(int j=0; j<vCount(); j++)
            {
                if( m_edges[i][j] != NULL )
                {
                    n++;
                }
            }
            ret = new DynamicArray<int>(n);
            if( ret != NULL )
            {
                for( int j=0, k=0; j<vCount(); j++)
                {
                    if( m_edges[i][j] != NULL )
                    {
                        ret->set(k++, j);
                    }
                }
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create ret object ...");
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }
        return ret;
    }
    E getEdge(int i, int j)                            // O(1)
    {
        E ret;
        if( !getEdge(i, j, ret) )
        {
            THROW_EXCEPTION(InvalidParameterException, "Index <i, j> is invalid ...");
        }
        return ret;
    }
    bool getEdge(int i, int j, E& value)                // O(1)
    {
        bool ret = ((0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()) );
        if( ret )
        {
            if( m_edges[i][j] != NULL )
            {
                value = *(m_edges[i][j]);
            }
            else
            {
                THROW_EXCEPTION(InvalidOperationException, "No value assigned to this edge ...");
            }
        }
        return ret;
    }
    bool setEdge(int i, int j, const E& value)          // O(1)
    {
        bool ret = ((0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()) );
        if( ret )
        {
            E* ne = m_edges[i][j];
            if( ne == NULL )
            {
                ne = new E();
                if(ne != NULL)
                {
                    *ne = value;
                    m_edges[i][j] = ne;
                    m_ecount++;
                }
                else
                {
                    THROW_EXCEPTION(NoEnoughMemoryException, "No memory to store new vertex value ...");
                }
            }
            else
            {
                *ne = value;
            }
        }
        return ret;
    }
    bool removeEdge(int i, int j)                       // O(1)
    {
        bool ret = ((0 <= i) && (i < vCount()) &&
                    (0 <= j) && (j < vCount()) );
        if( ret )
        {
            // 为了异常安全，设置临时变量替代
            E* toDel = m_edges[i][j];

            m_edges[i][j] = NULL;
            if( toDel != NULL )
            {
                m_ecount--;
                delete toDel;
            }
        }
        return ret;
    }
    int vCount()                                        // O(1)
    {
        return N;
    }
    int eCount()                                        // O(1)
    {
        return m_ecount;
    }
    int OD(int i)                                       // O(n)
    {
        int ret = 0;
        if((0 <= i) && (i < vCount()))
        {
            for(int j=0; j<vCount(); j++)
            {
                if( m_edges[i][j] != NULL )
                {
                    ret++;
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }
        return ret;
    }
    int ID(int i)                                       // O(n)
    {
        int ret = 0;
        if((0 <= i) && (i < vCount()))
        {
            for(int j=0; j<vCount(); j++)
            {
                if( m_edges[j][i] != NULL )
                {
                    ret++;
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }
        return ret;
    }
    bool isAdjacent(int i, int j)
    {
        return (0 <= i) && (i < vCount()) && (0 <= j) && (j < vCount()) && (m_edges[i][j] != NULL);
    }
    ~MatrixGraph()                                       // O(n)
    {
        for(int i=0; i<vCount(); i++)
        {
            for(int j=0; j<vCount(); j++)
            {
               delete m_edges[i][j];
            }
            delete m_vertexes[i];
        }
    }
};
}
#endif // MATRIXGRAPH_H
