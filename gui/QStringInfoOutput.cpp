#include"QStringInfoOutput.h"

class QStringInfoOutput:public InfoOutput
{
    QString *qstr;
                
public:

    QStringInfoOutput(QString *qs)
    {
        qstr=qs;
    }

    void write(const char *str,int len) override
    {
        if(!str||!*str)return;

        qstr->append(QString::fromUtf8(str,len));
    }

    void write(const wchar_t *str,int len) override
    {
        if(!str||!*str)return;

        qstr->append(QString::fromWCharArray(str,len));
    }

    void colorWrite(const char *color,const char *str) override
    {
        if(!str)return;

        if(!color)
            return InfoOutput::write(str);

        qstr->append(QString("<font color=\"")+QString(color)+QString("\">")+QString::fromUtf8(str)+QString("</font>"));
    }

    void colorWriteln(const char *color,const char *str) override
    {
        if(!str)return;

        if(!color)
            return InfoOutput::write(str);

        qstr->append(QString("<font color=\"")+QString(color)+QString("\">")+QString::fromUtf8(str)+QString("</font>\n"));
    }
};//class QStringInfoOutput

InfoOutput *CreateQStringInfoOutput(QString *str)
{
    return(new QStringInfoOutput(str));
}