#include"ShaderLibParse.h"
#include<iostream>

namespace shader_lib
{
    void FilesElementerCreater::CharData(const u8char *str,const int str_length)
    {
        int len;
        u8char *trim_str=trim(str,len);

        std::cout<<"file: "<<str<<std::endl;
    }
}//namespace shader_lib
