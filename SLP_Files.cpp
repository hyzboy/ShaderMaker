#include"ShaderLibParse.h"
#include<iostream>
#include<hgl/filesystem/FileSystem.h>

namespace shader_lib
{
    void FilesElementerCreater::Attr(const u8char *flag,const u8char *info)
    {
        if(hgl::stricmp(flag,U8_TEXT("type"))==0)
        {
            if(hgl::stricmp(info,U8_TEXT("glsl"))==0)type=FileType::GLSL;else
            if(hgl::stricmp(info,U8_TEXT("varying"))==0)type=FileType::Varying;
        }
    }

    bool FilesElementerCreater::Start()
    {
        constexpr os_char *filetype_name[]={OS_TEXT("GLSL"),OS_TEXT("Varying")};

        if(!RangeCheck<FileType>(type))
        {
            std::cout<<"  Files type error!"<<std::endl;
            return(false);
        }
        else
        {
            os_out<<OS_TEXT("  Files type is [")<<filetype_name[size_t(type)]<<OS_TEXT("]")<<std::endl;
        }

        return(true);
    }

    void FilesElementerCreater::CharData(const u8char *str,const int str_length)
    {
        int len=str_length;
        const u8char *trim_str=trim(str,len);

        if(trim_str)
        {
            const OSString filename=to_oschar(trim_str,len);
            const OSString fullname=filesystem::MergeFilename(pathname,filename);
            const OSString ext_name=filesystem::ClipFileExtName(filename,false);

            if(ext_name.CaseComp(OS_TEXT("xml"))==0)
            {
                os_out<<OS_TEXT("    XML file: ")<<fullname.c_str()<<std::endl;
            }
            else
            {
                os_out<<OS_TEXT("        file: ")<<fullname.c_str()<<std::endl;
            }

            file_list.Add(fullname);
        }
    }
}//namespace shader_lib
