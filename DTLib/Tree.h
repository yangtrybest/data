#ifndef TREE_H
#define TREE_H
#include "Object.h"
#include "SharedPointer.h"
#include "TreeNode.h"
namespace DTLib
{
template <typename T>
class Tree : public Object
{
protected:
    TreeNode<T>* m_root;
    Tree(const Tree<T>&);
    Tree<T>& operator = (const Tree<T>&);
public:
    Tree() { m_root = NULL; }
    virtual bool insert(TreeNode<T>* node) = 0;
    virtual bool insert(const T& value, TreeNode<T>* parent) = 0;

    // 删除树中的一个结点：
    // 比较合理的方式：删除操作删除一个结点，将D结点代表的子树 删除，但保留这颗子树
    // 并将其返回，这样便于对颗子树中的数据进行操作！！所以使用的是 智能指针来返回！
    virtual SharedPointer< Tree<T> > remove(const T& value) = 0;
    virtual SharedPointer< Tree<T> > remove(TreeNode<T>* node) = 0;

    virtual TreeNode<T>* find(const T& value) const = 0;
    virtual TreeNode<T>* find(TreeNode<T>* node) const = 0;
    virtual TreeNode<T>* root() const = 0;
    virtual int degree() const = 0;
    virtual int count() const = 0;
    virtual int height() const = 0;
    virtual void clear() = 0;
    // 层次遍历
    virtual bool begin() = 0;
    virtual bool end() = 0;
    virtual bool next() = 0;
    virtual T current() = 0;
};
}
#endif // TREE_H
