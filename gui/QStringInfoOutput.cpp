#include"QStringInfoOutput.h"
#include<hgl/type/StringList.h>

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

    void write(const hgl::UTF8StringList &sl)
    {
        for(const hgl::UTF8String *str:sl)
            qstr->append(QString("<p>")+QString::fromUtf8(str->c_str())+QString("</p>"));
    }

    void write(const hgl::WideStringList &sl)
    {
        for(const hgl::WideString *str:sl)
            qstr->append(QString("<p>")+QString::fromWCharArray(str->c_str())+QString("</p>"));
    }

    void colorWrite(const char *color,const char *str) override
    {
        if(!str)return;

        hgl::UTF8StringList sl;

        int count=hgl::SplitToStringListByEnter(sl,hgl::UTF8String(str));

        if(!color)
        {
            if(count==1)
                return InfoOutput::write(str);
            else
                return write(sl);
        }

        qstr->append(QString("<font color=\"")+QString(color)+QString("\">"));
        write(sl);
        qstr->append(QString("</font>"));
    }

    void colorWrite(const char *color,const wchar_t *str) override
    {
        if(!str)return;
        
        hgl::WideStringList sl;

        int count=hgl::SplitToStringListByEnter(sl,hgl::WideString(str));

        if(!color)
        {
            if(count==1)
                return InfoOutput::write(str);
            else
                return write(sl);
        }

        qstr->append(QString("<font color=\"")+QString(color)+QString("\">"));
        write(sl);
        qstr->append(QString("</font>"));
    }

    void colorWriteln(const char *color,const char *str) override
    {
        if(!str)return;        

        hgl::UTF8StringList sl;

        int count=hgl::SplitToStringListByEnter(sl,hgl::UTF8String(str));

        if(!color)
        {
            if(count==1)
                return InfoOutput::writeln(str);
            else
                return write(sl);
        }

        qstr->append(QString("<font color=\"")+QString(color)+QString("\">"));
        write(sl);
        qstr->append(QString("</font>"));
    }

    void colorWriteln(const char *color,const wchar_t *str) override
    {
        if(!str)return;
        
        hgl::WideStringList sl;

        int count=hgl::SplitToStringListByEnter(sl,hgl::WideString(str));

        if(!color)
        {
            if(count==1)
                return InfoOutput::writeln(str);
            else
                return write(sl);
        }

        qstr->append(QString("<font color=\"")+QString(color)+QString("\">"));
        write(sl);
        qstr->append(QString("</font>"));
    }
};//class QStringInfoOutput

InfoOutput *CreateQStringInfoOutput(QString *str)
{
    return(new QStringInfoOutput(str));
}