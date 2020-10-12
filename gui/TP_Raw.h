#pragma once

#include<QWidget>
#include<QListWidgetItem>
#include<QTextEdit>
#include<QSyntaxHighlighter>

class TPRawGLSL:public QWidget
{
    Q_OBJECT

private:

    void OnRawGLSLClicked(QListWidgetItem *item);

private:

    QTextEdit *sl_preview;
    QSyntaxHighlighter *highlighter;

public:

    TPRawGLSL();
    ~TPRawGLSL();
};
