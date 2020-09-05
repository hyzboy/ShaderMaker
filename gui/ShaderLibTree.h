#include<QWidget>
#include<QTreeWidget>

class ShaderLibTree:public QWidget
{
    QTreeWidget *sl_tree;

public:

    ShaderLibTree(QWidget *);
    ~ShaderLibTree();
};//