#include"TP_ShaderFile.h"
#include<QMenu>

void TPShaderFile::CreatePopupMenu()
{
    popup_menu=new QMenu(this);

    popup_menu->addAction(tr("&New shader"),this,&TPShaderFile::OnNew);
    popup_menu->addAction(tr("New &Folder"),this,&TPShaderFile::OnNewFolder);
    popup_menu->addSeparator();
    popup_menu->addAction(tr("&Rename"),this,&TPShaderFile::OnRename);
    popup_menu->addAction(tr("&Delete"),this,&TPShaderFile::OnDelete);
}

void TPShaderFile::OnPopupMenu(const QPoint &pos)
{
    popup_menu->popup(file_tree_widget->viewport()->mapToGlobal(pos));
}

void TPShaderFile::OnNew()
{
}

void TPShaderFile::OnNewFolder()
{
}

void TPShaderFile::OnRename()
{
}

void TPShaderFile::OnDelete()
{
}