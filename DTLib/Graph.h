#ifndef GRAPH_H
#define GRAPH_H
#include "Object.h"
#include "SharedPointer.h"
#include "Array.h"
#include "DynamicArray.h"
#include "LinkQueue.h"
#include "LinkStack.h"
#include "Sort.h"
namespace DTLib
{
template<typename E>
struct Edge : public Object
{
    int b;
    int e;
    E data;
    Edge(int i = -1, int j = -1)
    {
        b = i;
        e = j;
    }
    Edge(int i, int j, const E& value)
    {
        b = i;
        e = j;
        data = value;
    }

    bool operator == (const Edge<E>& obj)
    {
        return (b == obj.b)&&(e == obj.e);
    }
    bool operator != (const Edge<E>& obj)
    {
        return !(*this == obj);
    }
    bool operator < (const Edge<E>& obj)
    {
        return (data < obj.data);
    }
    bool operator > (const Edge<E>& obj)
    {
        return (data > obj.data);
    }
};
template <typename V, typename E>
class Graph : public Object
{
protected:
    template <typename T>
    DynamicArray<T>* toArray(LinkQueue<T>& queue)
    {
        DynamicArray<T>* ret = new DynamicArray<T>(queue.length());

        if( ret )
        {
            for(int i=0; i<ret->length(); i++, queue.remove())
            {
                ret->set(i, queue.front());
            }
        }
        else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create ret object ...");
        }

        return ret;
    }
    int find (Array<int>& p, int i)
    {
        while (p[i] != -1)
        {
            i = p[i];
        }
        return i;
    }
    SharedPointer<Array<Edge<E>>> getUndirectedEdges()
    {
        DynamicArray< Edge<E> >* ret = NULL;
        if(asUndirected())
        {
            LinkQueue<Edge<E>> queue;
            for(int i=0; i<vCount(); i++)
            {
                for(int j=i; j<vCount(); j++)
                {
                    if(isAdjacent(i, j))
                    {
                        queue.add(Edge<E>(i, j, getEdge(i, j)));
                    }
                }
            }
            ret = toArray(queue);
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "This function is for undirected graph ...");
        }
        return ret;
    }
public:
    virtual V getVertex(int i) = 0;//��ö����ֵ
    virtual bool getVertex(int i, V& value) = 0;
    virtual bool setVertex(int i, const V& value) = 0;//���ö���ֵ
    virtual SharedPointer<Array<int>> getAdjacent(int i) = 0;//��ȡ�ڽӶ���
    virtual E getEdge(int i, int j) = 0;//��ñߵ�ֵ
    virtual bool getEdge(int i, int j, E& value) = 0;
    virtual bool setEdge(int i, int j, const E& value) = 0;//���ñߵ�ֵ
    virtual bool removeEdge(int i, int j) = 0;//ɾ����
    virtual int vCount() = 0;//��ȡ������
    virtual int eCount() = 0;//��ȡ����
    virtual int OD(int i) = 0;//����
    virtual int ID(int i) = 0;//���

    virtual bool isAdjacent(int i, int j) = 0;  //�ж����������Ƿ��ڽ�
    virtual int TD(int i)
    {
        return ID(i) + OD(i);
    }

    bool asUndirected()
    {
        bool ret = true;
        for(int i=0; i<vCount(); i++)
        {
            for(int j=0; j<vCount(); j++)
            {
                if( isAdjacent(i, j) )
                {
                    ret = ret && isAdjacent(j, i) && (getEdge(i, j) == getEdge(j, i));
                }
            }
        }
        return ret;
    }
    SharedPointer< Array<int> > BFS(int i)
    {
        DynamicArray<int>* ret = NULL;

        if((0 <= i)&&(i < vCount()))
        {
            LinkQueue<int> queue;
            LinkQueue<int> rettemp;
            DynamicArray<bool> visited(vCount());

            for(int k=0; k<visited.length(); k++)
            {
                visited[k] = false;
            }

            queue.add(i);
            while( queue.length() > 0 )
            {
                int v = queue.front();

                queue.remove();

                if( !visited[v] )
                {
                    SharedPointer<Array<int>> aj = getAdjacent(v);

                    for(int j=0; j<aj->length(); j++)
                    {
                        queue.add((*aj)[j]);
                    }

                    rettemp.add(v);

                    visited[v] = true;
                }
            }
            ret = toArray(rettemp);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }
        return ret;
    }
    SharedPointer<Array<int>> DFS(int i)
    {
        DynamicArray<int>* ret = NULL;
        if((0 <= i)&&(i < vCount()))
        {
            LinkStack<int> stack;
            LinkQueue<int> rettemp;
            DynamicArray<bool> visited(vCount());

            for(int k=0; k<visited.length(); k++)
            {
                visited[k] = false;
            }
            stack.push(i);
            while ( stack.size() > 0 )
            {
                int v = stack.top();
                stack.pop();
                if( !visited[v] )
                {
                    SharedPointer<Array<int>> aj = getAdjacent(v);
                    for(int j=aj->length() - 1; j >= 0; j--)
                    {
                        stack.push((*aj)[j]);
                    }

                    rettemp.add(v);
                    visited[v] = true;
                }
            }
            ret = toArray(rettemp);
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Index i is invalid ...");
        }
        return ret;
    }
    SharedPointer<Array<Edge<E>>> prim(const E& LIMIT, const bool MINIMUM = true)
    {
        LinkQueue< Edge<E> > ret;
        if(asUndirected())
        {
            DynamicArray<int> adjVex(vCount());
            DynamicArray<bool> mark(vCount());
            DynamicArray<E> cost(vCount());

            SharedPointer< Array<int> > aj = NULL;

            bool end = false;

            int v = 0;

            for(int i=0; i<vCount(); i++)
            {
                adjVex[i] = -1;
                mark[i] = false;
                cost[i] = LIMIT;
            }
            mark[v] = true;
            aj = getAdjacent(v);

            for(int j=0; j<aj->length(); j++)
            {
                cost[(*aj)[j]] = getEdge(v, (*aj)[j]);
                adjVex[(*aj)[j]] = v;
            }
            for(int i=0; (i<vCount()) && !end; i++)
            {
                E m = LIMIT;
                int k = -1;
                for(int j=0; j<vCount(); j++)
                {
                    if(!mark[j] && (MINIMUM ? (cost[j] < m) : (cost[j] > m)))
                    {
                        m = cost[j];
                        k = j;
                    }
                }
                end = (k == -1);

                if( !end )
                {
                    ret.add(Edge<E>(adjVex[k], k, getEdge(adjVex[k], k)));
                    mark[k] = true;
                    aj = getAdjacent(k);
                    for(int j=0; j<aj->length(); j++)
                    {
                        if( !mark[(*aj)[j]] && (MINIMUM ? (getEdge(k, (*aj)[j]) < cost[(*aj)[j]]) : getEdge(k, (*aj)[j]) > cost[(*aj)[j]]))
                        {
                            cost[(*aj)[j]] = getEdge(k, (*aj)[j]);
                            adjVex[(*aj)[j]] = k;
                        }
                    }
                }
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidOperationException, "Prim operation is for undirected graph only ...");
        }
        if( ret.length() != (vCount() - 1))
        {
            THROW_EXCEPTION(InvalidOperationException, "No enough edge for prim operation ...");
        }
        return toArray(ret);
    }
    SharedPointer<Array<Edge<E>>> Kruskal(const bool MINIMUM = true)
    {
        DynamicArray<int> preflag(vCount());
        LinkQueue<Edge<E>> ret;
        SharedPointer<Array<Edge<E>>> edges = getUndirectedEdges();

        for(int i=0; i<preflag.length(); i++)
        {
            preflag[i] = -1;
        }
        Sort::Shell(*edges, MINIMUM);
        for(int i=0; (i < edges->length())&&(ret.length() < (vCount() - 1)); i++)
        {
            int begin = find(preflag, (*edges)[i].b);
            int end = find(preflag, (*edges)[i].e);

            if( begin != end )
            {
                preflag[end] = begin;
                ret.add((*edges)[i]);
            }
        }
        if( ret.length() != (vCount() - 1))
        {
            THROW_EXCEPTION(InvalidOperationException, "No enough edges for Kruskal operation ...");
        }
        return toArray(ret);
    }

    SharedPointer<Array<int>> Dijkstra(int i, int j, const E& LIMIT)        // O(n*n)
    {
        LinkQueue<int> ret;

        if((0 <= i)&&(i < vCount())&&(0 <= j)&&(j < vCount()))
        {
            //dist[k] ��ʾ i --> k �����·��Ȩֵ
            DynamicArray<E> dist(vCount());     //  ��¼Ȩֵ
            DynamicArray<int> path(vCount());   //  ��¼·����ǰ������
            DynamicArray<bool> mark(vCount());  //  ��ǵ�ǰ�����Ƿ񱻼���

            for(int k=0; k<vCount(); k++)
            {
                path[k] = -1;
                mark[k] = false;
                dist[k] = isAdjacent(i, k) ? (path[k] = i, getEdge(i, k)) : LIMIT;
            }

            mark[i] = true;
            for(int k=0; k<vCount(); k++)
            {
                E m = LIMIT;

                int u = -1;
                // ���ȴ� k ��ʼѡȡһ��Ȩֵ��С�ıߣ��������¼����Ϊ u
                for(int w=0; w<vCount(); w++)
                {
                    if( !mark[w] && (dist[w] < m) )
                    {
                        m = dist[w];
                        u = w;
                    }
                }
                // ����Ҳ���������ѭ��
                if( u == -1 )
                {
                    break;
                }
                // �� u ������Ķ��� ���Ϊ true�����뵽true�����У�����ʱ��� k --> u ���·���Ѿ��ҳ����ˣ�
                mark[u] = true;

                // �� ���Ϊfalse�ļ����У��ҵ����·����������ǰ������u���뵽path��
                for(int w=0; w<vCount(); w++)
                {
                    if(!mark[w] && isAdjacent(u, w) && (dist[u] + getEdge(u, w) < dist[w]))
                    {
                        dist[w] = dist[u] + getEdge(u, w);
                        path[w] = u;
                    }
                }
            }
            // �Ӻ���ǰ�Ҷ��㣬ѹ��ջ��
            LinkStack<int> s;
            s.push(j);
            for(int k=path[j]; k!=-1; k=path[k])
            {
                s.push(k);
            }
            while (s.size() > 0)
            {
                ret.add(s.top());
                s.pop();
            }
            if(ret.length() < 2)
            {
                THROW_EXCEPTION(ArithmeticException, "There is no path form i to j ...");
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Invalid <i, j> is invalid ...");
        }
        return toArray(ret);
    }
    SharedPointer<Array<int>> Floyd(int x, int y, const E& LIMIT) // O(n*n*n)
    {
        LinkQueue<int> ret;

        if((0 <= x)&&(x < vCount())&&(0 <= y)&&(y < vCount()))
        {
            DynamicArray< DynamicArray<E> > dist(vCount());
            DynamicArray< DynamicArray<int> > path(vCount());

            for(int k=0; k<vCount(); k++)
            {
                dist[k].resize(vCount());
                path[k].resize(vCount());
            }

            for(int i=0; i<vCount(); i++)
            {
                for(int j=0; j<vCount(); j++)
                {
                    path[i][j] = -1;
                    dist[i][j] = isAdjacent(i, j) ? (path[i][j] = j, getEdge(i, j)) : LIMIT;
                }
            }

            for(int k=0; k<vCount(); k++)
            {
                for(int i=0; i<vCount(); i++)
                {
                    for(int j=0; j<vCount(); j++)
                    {
                        if(dist[i][j] > (dist[i][k] + dist[k][j]))
                        {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            path[i][j] = path[i][k];
                        }
                    }
                }
            }
            while((x != -1)&&(x != y))
            {
                ret.add(x);
                x = path[x][y];
            }
            if(x != -1)
            {
                ret.add(x);
            }
            if(ret.length() < 2)
            {
                THROW_EXCEPTION(ArithmeticException, "There is no path form i to j ...");
            }
        }
        else
        {
            THROW_EXCEPTION(InvalidParameterException, "Invalid <i, j> is invalid ...");
        }
        return toArray(ret);
    }
};

}

#endif // GRAPH_H
