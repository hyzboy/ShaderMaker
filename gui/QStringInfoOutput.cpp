#include"QStringInfoOutput.h"
#include<hgl/type/SplitString.h>

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
        {
            qstr->append(QString("<p>"));
            qstr->append(QString::fromUtf8(str->c_str()));
            qstr->append(QString("</p>"));
        }
    }

    void write(const hgl::WideStringList &sl)
    {
        for(const hgl::WideString *str:sl)
        {
            qstr->append(QString("<p>"));
            qstr->append(QString::fromWCharArray(str->c_str()));
            qstr->append(QString("</p>"));
        }
    }

    template<typename T>
    void format_string(bool append_enter,const char *color,const T *format,va_list args)
    {
        if(!format)return;
        
        T buffer[4096];

        int len=hgl::vsprintf<T>(buffer,4096,format,args);
        
        hgl::StringList<T> sl;

        int count=hgl::SplitToStringListByEnter(sl,buffer,len);

        if(!color)
        {
            if(count==1)
            {
                write(buffer,len);

                if(append_enter)
                    writeEnter<T>();
            }
            else
            {
                return write(sl);
            }
        }

        qstr->append(QString("<font color=\"")+QString(color)+QString("\">"));
        write(sl);
        qstr->append(QString("</font>"));
    }

    void colorWrite(const char *color,const char *format,...) override
    {
        va_list args;
        va_start(args,format);

        format_string<char>(false,color,format,args);
        va_end(args);
    }

    void colorWrite(const char *color,const wchar_t *format,...) override
    {
        va_list args;
        va_start(args,format);

        format_string<wchar_t>(false,color,format,args);
        va_end(args);
    }

    void colorWriteln(const char *color,const char *format,...) override
    {
        va_list args;
        va_start(args,format);

        format_string<char>(true,color,format,args);
        va_end(args);
    }

    void colorWriteln(const char *color,const wchar_t *format,...) override
    {
        va_list args;
        va_start(args,format);

        format_string<wchar_t>(true,color,format,args);
        va_end(args);
    }
};//class QStringInfoOutput

InfoOutput *CreateQStringInfoOutput(QString *str)
{
    return(new QStringInfoOutput(str));
}