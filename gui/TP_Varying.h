#pragma once

#include<QWidget>
#include<QListWidgetItem>
#include<QTextEdit>
#include<QSyntaxHighlighter>

class TPVarying:public QWidget
{
    Q_OBJECT

private:

    void OnVaryingClicked(QListWidgetItem *item);

private:

    QTextEdit *sl_preview;
    QSyntaxHighlighter *highlighter;

public:

    TPVarying();
    ~TPVarying();
};
