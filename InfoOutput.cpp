#include"InfoOutput.h"
#include<stdio.h>
#include<stdarg.h>
#include<wchar.h>
#include<hgl/type/StrChar.h>

void InfoOutput::write(const char *str)
{
    if(!str)return;
    if(!*str)return;

    write(str,hgl::strlen(str));
}

void InfoOutput::writeln(const char *str)
{
    write(str);
    write("\n",1);
}

void InfoOutput::write(const wchar_t *str)
{
    if(!str)return;
    if(!*str)return;

    write(str,hgl::strlen(str));
}

void InfoOutput::writeln(const wchar_t *str)
{
    write(str);
    write(L"\n",1);
}
    
void InfoOutput::print(const char *format,...)
{
    va_list args;
    va_start(args,format);

    char buffer[1024];

    int len=vsprintf_s(buffer,format,args);

    if(len>0)
        write(buffer,len);
}

void InfoOutput::print(const wchar_t *format,...)
{
    va_list args;
    va_start(args,format);

    wchar_t buffer[1024];

    int len=vswprintf_s(buffer,format,args);

    if(len>0)
        write(buffer,len);
}
    
InfoOutput &InfoOutput::operator<<(int num)             {os_char buf[64];write(hgl::itos(buf,64,num));return *this;}
InfoOutput &InfoOutput::operator<<(unsigned int num)    {os_char buf[64];write(hgl::utos(buf,64,num));return *this;}
InfoOutput &InfoOutput::operator<<(__int64 num)         {os_char buf[64];write(hgl::itos(buf,64,num));return *this;}
InfoOutput &InfoOutput::operator<<(unsigned __int64 num){os_char buf[64];write(hgl::utos(buf,64,num));return *this;}
InfoOutput &InfoOutput::operator<<(float num)           {os_char buf[64];write(hgl::ftos(buf,64,num));return *this;}
InfoOutput &InfoOutput::operator<<(double num)          {os_char buf[64];write(hgl::ftos(buf,64,num));return *this;}

class StdInfoOutput:public InfoOutput
{
public:

    void write(const char *str,int len) override
    {
        if(!str||!*str)return;

        std::cout<<str;
    }

    void write(const wchar_t *str) override
    {
        if(!str||!*str)return;

        std::wcout<<str;
    }
};//class StdInfoOutput

static StdInfoOutput std_info_output;

InfoOutput *CreateStdInfoOutput()
{
    return(new StdInfoOutput);
}

InfoOutput *GetDefaultInfoOutput()
{
    return &std_info_output;
}