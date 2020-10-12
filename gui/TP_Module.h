#pragma once

#include<QWidget>
#include<QListWidgetItem>
#include<QTextEdit>
#include<QSyntaxHighlighter>

class TPModule:public QWidget
{
    Q_OBJECT

private:

    void OnModuleClicked(QListWidgetItem *item);

private:

    QListWidget *depend_raw_list;
    QListWidget *depend_struct_list;

    QListWidget *CreateDependGroup(QLayout *,const QString &);

    QTextEdit *sl_preview;
    QSyntaxHighlighter *highlighter;

public:

    TPModule();
    ~TPModule();
};
