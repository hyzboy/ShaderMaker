#pragma once

#include<QTextEdit>
#include<QSyntaxHighlighter>

class XMLTextEdit:public QTextEdit
{
protected:
    
    QSyntaxHighlighter *highlighter;

private:

    void keyPressEvent(QKeyEvent *) override;

public:

    XMLTextEdit(QWidget *parent=nullptr);
    ~XMLTextEdit()=default;
};//