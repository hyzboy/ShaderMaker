#pragma once

#include<QWidget>
#include<QTreeWidget>
#include"GLSLTextEdit.h"
#include"WI_EditorTreeWidgetItem.h"

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
    EditorTreeWidgetItem *current_item;

    void UpdateFileTree();
    void OnItemChange(QTreeWidgetItem *,int);
    void OnFileOpenEditor(QTreeWidgetItem *,int);

    QMenu *popup_menu_file,*popup_menu_folder;

    void CreatePopupMenu();
    void OnPopupMenu(const QPoint &);

    void OnNewGLSL();

    void OnNewFolder();
    void OnRename();
    void OnDelete();

private:

    QTabWidget *editor_tab_widget;

    void OnTabCloseRequested(int index);

public:

    TPShaderFile();
};
