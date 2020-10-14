#include"GLSLTextEdit.h"
#include<QSyntaxHighlighter>
#include<QRegularExpression>
#include<QTextBlock>
#include<QPainter>
#include<QRegExp>

class GLSLHighlighter:public QSyntaxHighlighter
{
public:

    GLSLHighlighter(QTextDocument *parent):QSyntaxHighlighter(parent)
    {
        HighlightingRule rule;

        keywordFormat.setForeground(Qt::darkBlue);
        keywordFormat.setFontWeight(QFont::Bold);

        const QString keywordPatterns[] = 
        {
            QStringLiteral("\\bgl_Position\\b"),
            QStringLiteral("\\bgl_FragCoord\\b"),

            QStringLiteral("\\bmain\\b"),
            QStringLiteral("\\breturn\\b"),
            QStringLiteral("\\bconst\\b"),

            QStringLiteral("\\bvoid\\b"),
            QStringLiteral("\\bfloat\\b"),  QStringLiteral("\\bvec2\\b"),   QStringLiteral("\\bvec3\\b"),       QStringLiteral("\\bvec4\\b"),
            QStringLiteral("\\bint\\b"),    QStringLiteral("\\bivec2\\b"),  QStringLiteral("\\bivec3\\b"),      QStringLiteral("\\bivec4\\b"),
            QStringLiteral("\\buint\\b"),   QStringLiteral("\\buvec2\\b"),  QStringLiteral("\\buvec3\\b"),      QStringLiteral("\\buvec4\\b"),
            QStringLiteral("\\bbool\\b"),   QStringLiteral("\\bbec2\\b"),   QStringLiteral("\\bbec3\\b"),       QStringLiteral("\\bbec4\\b"),
            QStringLiteral("\\bdouble\\b"), QStringLiteral("\\bdvec2\\b"),  QStringLiteral("\\bdvec3\\b"),      QStringLiteral("\\bdvec4\\b"),

            QStringLiteral("\\bmat2\\b"),   QStringLiteral("\\bmat3\\b"),   QStringLiteral("\\bmat4\\b"),
            QStringLiteral("\\bmat2x2\\b"), QStringLiteral("\\bmat3x2\\b"), QStringLiteral("\\bmat4x2\\b"),
            QStringLiteral("\\bmat2x3\\b"), QStringLiteral("\\bmat3x3\\b"), QStringLiteral("\\bmat4x3\\b"),
            QStringLiteral("\\bmat2x4\\b"), QStringLiteral("\\bmat3x4\\b"), QStringLiteral("\\bmat4x4\\b"),

            QStringLiteral("\\bdmat2\\b"),  QStringLiteral("\\bdmat3\\b"),  QStringLiteral("\\bdmat4\\b"),
            QStringLiteral("\\bdmat2x2\\b"),QStringLiteral("\\bdmat3x2\\b"),QStringLiteral("\\bdmat4x2\\b"),
            QStringLiteral("\\bdmat2x3\\b"),QStringLiteral("\\bdmat3x3\\b"),QStringLiteral("\\bdmat4x3\\b"),
            QStringLiteral("\\bdmat2x4\\b"),QStringLiteral("\\bdmat3x4\\b"),QStringLiteral("\\bdmat4x4\\b"),

            QStringLiteral("\\bimage1D\\b"),          QStringLiteral("\\biimage1D\\b"),          QStringLiteral("\\buimage1D\\b"),
            QStringLiteral("\\bimage2D\\b"),          QStringLiteral("\\biimage2D\\b"),          QStringLiteral("\\buimage2D\\b"),
            QStringLiteral("\\bimage3D\\b"),          QStringLiteral("\\biimage3D\\b"),          QStringLiteral("\\buimage3D\\b"),
            QStringLiteral("\\bimageCube\\b"),        QStringLiteral("\\biimageCube\\b"),        QStringLiteral("\\buimageCube\\b"),
            QStringLiteral("\\bimage2DRect\\b"),      QStringLiteral("\\biimage2DRect\\b"),      QStringLiteral("\\buimage2DRect\\b"),
            QStringLiteral("\\bimage1DArray\\b"),     QStringLiteral("\\biimage1DArray\\b"),     QStringLiteral("\\buimage1DArray\\b"),
            QStringLiteral("\\bimage2DArray\\b"),     QStringLiteral("\\biimage2DArray\\b"),     QStringLiteral("\\buimage2DArray\\b"),
            QStringLiteral("\\bimageCubeArray\\b"),   QStringLiteral("\\biimageCubeArray\\b"),   QStringLiteral("\\buimageCubeArray\\b"),
            QStringLiteral("\\bimageBuffer\\b"),      QStringLiteral("\\biimageBuffer\\b"),      QStringLiteral("\\buimageBuffer\\b"),
            QStringLiteral("\\bimage2DMS\\b"),        QStringLiteral("\\biimage2DMS\\b"),        QStringLiteral("\\buimage2DMS\\b"),
            QStringLiteral("\\bimage2DMSArray\\b"),   QStringLiteral("\\biimage2DMSArray\\b"),   QStringLiteral("\\buimage2DMSArray\\b"),

            QStringLiteral("\\bsampler1D\\b"),          QStringLiteral("\\bisampler1D\\b"),          QStringLiteral("\\busampler1D\\b"),
            QStringLiteral("\\bsampler2D\\b"),          QStringLiteral("\\bisampler2D\\b"),          QStringLiteral("\\busampler2D\\b"),
            QStringLiteral("\\bsampler3D\\b"),          QStringLiteral("\\bisampler3D\\b"),          QStringLiteral("\\busampler3D\\b"),
            QStringLiteral("\\bsamplerCube\\b"),        QStringLiteral("\\bisamplerCube\\b"),        QStringLiteral("\\busamplerCube\\b"),
            QStringLiteral("\\bsampler2DRect\\b"),      QStringLiteral("\\bisampler2DRect\\b"),      QStringLiteral("\\busampler2DRect\\b"),
            QStringLiteral("\\bsampler1DArray\\b"),     QStringLiteral("\\bisampler1DArray\\b"),     QStringLiteral("\\busampler1DArray\\b"),
            QStringLiteral("\\bsampler2DArray\\b"),     QStringLiteral("\\bisampler2DArray\\b"),     QStringLiteral("\\busampler2DArray\\b"),
            QStringLiteral("\\bsamplerCubeArray\\b"),   QStringLiteral("\\bisamplerCubeArray\\b"),   QStringLiteral("\\busamplerCubeArray\\b"),
            QStringLiteral("\\bsamplerBuffer\\b"),      QStringLiteral("\\bisamplerBuffer\\b"),      QStringLiteral("\\busamplerBuffer\\b"),
            QStringLiteral("\\bsampler2DMS\\b"),        QStringLiteral("\\bisampler2DMS\\b"),        QStringLiteral("\\busampler2DMS\\b"),
            QStringLiteral("\\bsampler2DMSArray\\b"),   QStringLiteral("\\bisampler2DMSArray\\b"),   QStringLiteral("\\busampler2DMSArray\\b"),

            QStringLiteral("\\bsampler1DShadow\\b"),
            QStringLiteral("\\bsampler2DShadow\\b"),
            QStringLiteral("\\bsamplerCubeShadow\\b"),
            QStringLiteral("\\bsampler2DRectShadow\\b"),
            QStringLiteral("\\bsampler1DArrayShadow\\b"),
            QStringLiteral("\\bsampler2DArrayShadow\\b"),
            QStringLiteral("\\bsamplerCubeArrayShadow\\b"),

            QStringLiteral("\\batomic_uint\\b"),

            QStringLiteral("\\bstruct\\b"),
            QStringLiteral("\\buniform\\b"),
            QStringLiteral("\\blayout\\b"),
            QStringLiteral("\\bbinding\\b"),
            QStringLiteral("\\blocation\\b"),
            QStringLiteral("\\bset\\b"),
            QStringLiteral("\\boffset\\b"),
            QStringLiteral("\\brow_major\\b"),
            QStringLiteral("\\bcol_major\\b"),

            QStringLiteral("\\bpush_constant\\b"),

            QStringLiteral("\\bin\\b"),     QStringLiteral("\\bout\\b"),        QStringLiteral("\\binout\\b"),

            QStringLiteral("\\bif\\b"),
            QStringLiteral("\\bfor\\b"),
            QStringLiteral("\\bwhile\\b"),
        };

        for (const QString &pattern : keywordPatterns)
        {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        
        //! [0] //! [1]
        }
        //! [1]

        sharpFormat.setForeground(Qt::darkGreen);
        rule.pattern=QRegularExpression(QStringLiteral("#\\w*"));
        rule.format=sharpFormat;
        highlightingRules.append(rule);

        integerFormat.setForeground(Qt::red);
        rule.pattern=QRegularExpression(QStringLiteral("\\b\\+?\\-?[0-9]+e?\\+?\\-?[0-9]*\\b"));
        rule.format=integerFormat;
        highlightingRules.append(rule);

        floatFormat.setForeground(Qt::red);
        rule.pattern=QRegularExpression(QStringLiteral("\\b\\+?\\-?[0-9]*\\.[0-9]+e?\\+?\\-?[0-9]*\\b"));
        rule.format=floatFormat;
        highlightingRules.append(rule);

        symbolFormat.setForeground(Qt::darkGreen);
        rule.pattern=QRegularExpression(QStringLiteral("\\+?\\-?\\*?/?\\{?\\}?\\(?\\)?,?=?;?"));
        rule.format=symbolFormat;
        highlightingRules.append(rule);

        //! [2]

        //! [3]
        singleLineCommentFormat.setForeground(Qt::gray);
        rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(Qt::gray);
        //! [3]

        //! [4]
        stringFormat.setForeground(QColor(255,106,0));
        rule.pattern = QRegularExpression(QStringLiteral("\"\\S*\""));
        rule.format = stringFormat;
        highlightingRules.append(rule);
        //! [4]

        //! [5]
        functionFormat.setForeground(Qt::blue);
        rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
        rule.format = functionFormat;
        highlightingRules.append(rule);
        //! [5]

        //! [6]
        commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
        commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));

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

        //! [9]
        commentBlock(text,commentStartExpression,commentEndExpression,multiLineCommentFormat);
//        commentBlock(text,xml_commentStartExpression,xml_commentEndExpression,xml_commentFormat);
    }

private:

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat sharpFormat;
    QTextCharFormat integerFormat;
    QTextCharFormat floatFormat;
    QTextCharFormat symbolFormat;

    QTextCharFormat keywordFormat;

    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat functionFormat;

    QTextCharFormat xml_tagFormat,
                    xml_commentFormat;

    QRegularExpression  xml_commentStartExpression,
                        xml_commentEndExpression;
};//class GLSLHighlighter:public QSyntaxHighlighter

GLSLTextEdit::GLSLTextEdit(QWidget *parent):QTextEdit(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setFrameShape(QFrame::StyledPanel);
    setReadOnly(true);
    setLineWrapMode(QTextEdit::NoWrap);
    setTabStopWidth(4);
    {
        QFont f=font();

        f.setFixedPitch(true);
        f.setStyleHint(QFont::Monospace);
        f.setFamily("Consolas");

        if(f.pointSize()<9)
            f.setPointSize(9);

        setFont(f);
    }

    highlighter=new GLSLHighlighter(document());
}

void GLSLTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==Qt::Key_Tab)
    {
        insertPlainText("    ");
        return;
    }
    
    QTextEdit::keyPressEvent(e);
}
