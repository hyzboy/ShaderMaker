#include"XMLTextEdit.h"
#include"ConfigData.h"
#include<QSyntaxHighlighter>
#include<QRegularExpression>
#include<QTextBlock>
#include<QPainter>
#include<QRegularExpression>

class XMLHighlighter:public QSyntaxHighlighter
{
public:

    XMLHighlighter(QTextDocument *parent):QSyntaxHighlighter(parent)
    {
        HighlightingRule rule;

        keywordFormat.setForeground(Qt::darkBlue);
        keywordFormat.setFontWeight(QFont::Bold);

        const QString keywordPatterns[] = 
        {
            QStringLiteral("\\bmain\\b"),
        };

        for (const QString &pattern : keywordPatterns)
        {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        
        //! [0] //! [1]
        }
        //! [1]

        //! [4]
        stringFormat.setForeground(QColor(255,106,0));
        rule.pattern = QRegularExpression(QStringLiteral("\"\\S*\""));
        rule.format = stringFormat;
        highlightingRules.append(rule);
        //! [4]


        // tag format
        xml_tagFormat.setForeground(Qt::darkBlue);
        xml_tagFormat.setFontWeight(QFont::Bold);
        rule.pattern = QRegularExpression(QStringLiteral("(<[a-zA-Z:]+\\b|<\\?[a-zA-Z:]+\\b|\\?>|>|/>|</[a-zA-Z:]+>)"));
        rule.format = xml_tagFormat;
        highlightingRules.append(rule);

        xml_commentFormat.setForeground(Qt::lightGray);
        xml_commentFormat.setFontItalic(true);

        xml_commentStartExpression = QRegularExpression(QStringLiteral("<!--"));
        xml_commentEndExpression = QRegularExpression(QStringLiteral("-->"));
    }
    //! [6]

    void commentBlock(const QString &text,const QRegularExpression start,QRegularExpression end,QTextCharFormat format)
    {
        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = text.indexOf(start);

        //! [9] //! [10]
        while (startIndex >= 0) 
        {
            //! [10] //! [11]
            QRegularExpressionMatch match = end.match(text, startIndex);
            int endIndex = match.capturedStart();
            int commentLength = 0;

            if (endIndex == -1) 
            {
                setCurrentBlockState(1);
                commentLength= text.length() - startIndex;
            }
            else
            {
                commentLength= endIndex - startIndex
                             + match.capturedLength();
            }

            setFormat(startIndex, commentLength, format);
            startIndex = text.indexOf(start, startIndex + commentLength);
        }
    }

    //! [7]
    void highlightBlock(const QString &text) override
    {
        for (const HighlightingRule &rule : qAsConst(highlightingRules)) 
        {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);

            while (matchIterator.hasNext()) 
            {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
        //! [7] //! [8]
        setCurrentBlockState(0);
        //! [8]
    }

private:

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat,
                    stringFormat,
        
                    xml_tagFormat,
                    xml_commentFormat;

    QRegularExpression  xml_commentStartExpression,
                        xml_commentEndExpression;
};//class GLSLHighlighter:public QSyntaxHighlighter

XMLTextEdit::XMLTextEdit(QWidget *parent):QTextEdit(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setFrameShape(QFrame::StyledPanel);
    setReadOnly(true);
    setLineWrapMode(QTextEdit::NoWrap);
    setTabStopDistance(4);
    setFont(GetCodeFont());

    highlighter=new XMLHighlighter(document());
}

void XMLTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Tab)
    {
        insertPlainText("    ");
        return;
    }
    
    QTextEdit::keyPressEvent(e);
}
