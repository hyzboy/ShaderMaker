#pragma once

class InfoOutput
{
public:

    virtual ~InfoOutput()=default;

    virtual void write(const char *,const int)=0;
    virtual void write(const wchar_t *,const int)=0;

            template<typename T>
            void writeEnter()
            {
                constexpr T ch=(T)'\n';
                return write(&ch,1);
            }

            void write(const char *);
            void writeln(const char *);
            void write(const wchar_t *);
            void writeln(const wchar_t *);

            void print(const char *,...);
            void print(const wchar_t *,...);

    virtual void colorWrite(const char *,const char *str,...){print(str);}
    virtual void colorWrite(const char *,const wchar_t *str,...){print(str);}
    virtual void colorWriteln(const char *,const char *str,...){print(str);writeEnter<char>();}
    virtual void colorWriteln(const char *,const wchar_t *str,...){print(str);writeEnter<wchar_t>();}

    InfoOutput &operator<<(int);
    InfoOutput &operator<<(unsigned int);
    InfoOutput &operator<<(__int64);
    InfoOutput &operator<<(unsigned __int64);
    InfoOutput &operator<<(float);
    InfoOutput &operator<<(double);
    InfoOutput &operator<<(const char *str){write(str);return *this;}
    InfoOutput &operator<<(const wchar_t *str){write(str);return *this;}
};//class InfoOutput

InfoOutput *GetDefaultInfoOutput();
InfoOutput *CreateStdInfoOutput();
