#include"InfoOutput.h"
#include<stdio.h>
#include<stdarg.h>
#include<wchar.h>
#include<hgl/type/StrChar.h>

#if HGL_OS == HGL_OS_Windows
#include<Windows.h>
#endif//

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
    va_end(args);

    if(len>0)
        write(buffer,len);
}

void InfoOutput::print(const wchar_t *format,...)
{
    va_list args;
    va_start(args,format);

    wchar_t buffer[1024];

    int len=vswprintf_s(buffer,format,args);
    va_end(args);

    if(len>0)
        write(buffer,len);
}

void InfoOutput::colorWrite(const char *,const char *format,...)
{
    va_list args;
    va_start(args,format);

    print(format,args);
    va_end(args);
}

void InfoOutput::colorWrite(const char *,const wchar_t *format,...)
{
    va_list args;
    va_start(args,format);

    print(format,args);
    va_end(args);
}

void InfoOutput::colorWriteln(const char *,const char *format,...)
{
    va_list args;
    va_start(args,format);

    print(format,args);
    va_end(args);
    writeEnter<char>();
}

void InfoOutput::colorWriteln(const char *,const wchar_t *format,...)
{
    va_list args;
    va_start(args,format);

    print(format,args);
    va_end(args);
    writeEnter<wchar_t>();
}

InfoOutput &InfoOutput::operator<<(int num)             {os_char buf[64];write(hgl::itos(buf,64,num));return *this;}
InfoOutput &InfoOutput::operator<<(unsigned int num)    {os_char buf[64];write(hgl::utos(buf,64,num));return *this;}
InfoOutput &InfoOutput::operator<<(__int64 num)         {os_char buf[64];write(hgl::itos(buf,64,num));return *this;}
InfoOutput &InfoOutput::operator<<(unsigned __int64 num){os_char buf[64];write(hgl::utos(buf,64,num));return *this;}
InfoOutput &InfoOutput::operator<<(float num)           {os_char buf[64];write(hgl::ftos(buf,64,num));return *this;}
InfoOutput &InfoOutput::operator<<(double num)          {os_char buf[64];write(hgl::ftos(buf,64,num));return *this;}

class StdInfoOutput:public InfoOutput
{
    #if HGL_OS == HGL_OS_Windows
    void *console_handle;
    DWORD result;
    #endif//

public:

    StdInfoOutput()
    {
    #if HGL_OS == HGL_OS_Windows
        console_handle=GetStdHandle(STD_OUTPUT_HANDLE);
    #endif//
    }

    void write(const char *str,int len) override
    {
        if(!str||!*str)return;

        #if HGL_OS == HGL_OS_Windows
            WriteConsoleA(console_handle,str,len,&result,nullptr);
        #else
            std::cout<<str;
        #endif//
    }

    void write(const wchar_t *str,int len) override
    {
        if(!str||!*str)return;

        #if HGL_OS == HGL_OS_Windows
            WriteConsoleW(console_handle,str,len,&result,nullptr);
        #else
            std::wcout<<str;
        #endif//
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
