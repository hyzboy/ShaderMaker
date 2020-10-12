#pragma once

#include<QWidget>
#include<QListWidgetItem>
#include<QTextEdit>
#include<QSyntaxHighlighter>

class TPStruct:public QWidget
{
    Q_OBJECT

private:

    void OnStructClicked(QListWidgetItem *item);

private:

    QTextEdit *sl_preview;
    QSyntaxHighlighter *highlighter;

public:

    TPStruct();
    ~TPStruct();
};
