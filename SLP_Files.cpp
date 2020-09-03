#include"ShaderLibParse.h"
#include<iostream>

namespace shader_lib
{
    void FilesElementerCreater::Attr(const u8char *flag,const u8char *info)
    {
        if(hgl::stricmp(flag,U8_TEXT("type"))==0)
        {
            if(hgl::stricmp(flag,U8_TEXT("glsl"))==0)type=FileType::GLSL;else
            if(hgl::stricmp(flag,U8_TEXT("varying"))==0)type=FileType::Varying;
        }
    }

    void FilesElementerCreater::CharData(const u8char *str,const int str_length)
    {
        int len=str_length;
        const u8char *trim_str=trim(str,len);

        if(trim_str)
        {
            const OSString filename=to_oschar(trim_str,len);

            os_out<<OS_TEXT("file: ")<<filename.c_str()<<std::endl;

            file_list.Add(filename);
        }
    }
}//namespace shader_lib
