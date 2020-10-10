#include<QSyntaxHighlighter>
#include<QRegularExpression>

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
        classFormat.setFontWeight(QFont::Bold);
        classFormat.setForeground(Qt::darkMagenta);
        rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
        rule.format = classFormat;
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
        quotationFormat.setForeground(Qt::darkGreen);
        rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
        rule.format = quotationFormat;
        highlightingRules.append(rule);
        //! [4]

        //! [5]
        functionFormat.setFontItalic(true);
        functionFormat.setForeground(Qt::blue);
        rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
        rule.format = functionFormat;
        highlightingRules.append(rule);
        //! [5]

        //! [6]
        commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
        commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
    }
    //! [6]

    //! [7]
    void highlightBlock(const QString &text)
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
        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = text.indexOf(commentStartExpression);

        //! [9] //! [10]
        while (startIndex >= 0) {
            //! [10] //! [11]
            QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
            int endIndex = match.capturedStart();
            int commentLength = 0;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex
                    + match.capturedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
        }
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

    QTextCharFormat integerFormat;
    QTextCharFormat floatFormat;
    QTextCharFormat symbolFormat;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};//class GLSLHighlighter:public QSyntaxHighlighter

QSyntaxHighlighter *CreateGLSLSyntaxHighlighter(QTextDocument *document)
{
    return(new GLSLHighlighter(document));
}