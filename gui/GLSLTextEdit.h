#pragma once

#include<QTextEdit>
#include<QSyntaxHighlighter>

class GLSLTextEdit:public QTextEdit
{
protected:

    QSyntaxHighlighter *highlighter;

private:

    void keyPressEvent(QKeyEvent *) override;

public:

    GLSLTextEdit(QWidget *parent=nullptr);
    ~GLSLTextEdit()=default;
};//