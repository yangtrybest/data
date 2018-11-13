#ifndef TREENODE_H
#define TREENODE_H
#include "Object.h"
namespace DTLib
{
template <typename T>
class TreeNode : public Object
{
protected:
    bool m_flag;
    TreeNode(const TreeNode<T>&);
    TreeNode<T>& operator = (const TreeNode<T>&);
    void* operator new(unsigned int size) throw()
    {
        return Object::operator new(size);
    }
public:
    T value;
    TreeNode<T>* parent;            // 指向父结点的指针，在工程中很有用
    TreeNode()
    {
        parent = NULL;
        m_flag = false;
    }
    bool flag()
    {
        return m_flag;
    }
    virtual ~TreeNode() = 0;
};
template <typename T>
TreeNode<T>::~TreeNode()
{

}
}
#endif // TREENODE_H
