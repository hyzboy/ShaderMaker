#pragma once

#include<QWidget>
#include<QTreeWidget>
#include"GLSLTextEdit.h"

enum class ShaderFileColumn
{
    Name=0,
    Filename
};

class TPShaderFile:public QWidget
{
    Q_OBJECT

private:
    
    QTreeWidget *file_tree_widget;
    QTreeWidgetItem *current_item;

    void UpdateFileTree();
    void OnFileChanged(QTreeWidgetItem *,int);

    QMenu *popup_menu;

    void CreatePopupMenu();
    void OnPopupMenu(const QPoint &);

    void OnNew();
    void OnNewFolder();
    void OnRename();
    void OnDelete();

private:

    QTabWidget *editor_tab_widget;

    void OnTabCloseRequested(int index);

public:

    TPShaderFile();
};
