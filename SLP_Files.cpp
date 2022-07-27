#include"ShaderLibParse.h"
#include<iostream>
#include<hgl/filesystem/FileSystem.h>

namespace shader_lib
{
    //bool LoadVarying(const OSString &);
    bool LoadXmlGLSL(const OSString &);
    bool LoadRawGLSL(const OSString &);

    const u8char *filetype_name[]=
    {
        U8_TEXT("GLSL"),
        U8_TEXT("Varying"),
    };

    bool FilesElementerCreater::Start()
    {
        const UTF8String type_str=(*this)[u8"type"];

        type=(FileType)string_serial_from_list<u8char>(filetype_name,type_str.c_str());

        if(!RangeCheck<FileType>(type))
        {
            std::cout<<"  Files type error!"<<std::endl;
            type=FileType::GLSL;
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

        if(!trim_str)
            return;

        {
            const OSString filename=to_oschar(trim_str,len);
            const OSString fullname=filesystem::MergeFilename(pathname,filename);
            const OSString ext_name=filesystem::ClipFileExtName(filename,false);

            if(ext_name.CaseComp(OS_TEXT("xml"))==0)
            {
                os_out<<OS_TEXT("    XML file: ")<<fullname.c_str()<<std::endl;

                if(type==FileType::Varying)
                {
                 //   LoadVarying(fullname);
                }
                else
                if(type==FileType::GLSL)
                    LoadXmlGLSL(fullname);
            }
            else
            if(ext_name.CaseComp(OS_TEXT("glsl"))==0)
            {                
                os_out<<OS_TEXT("   GLSL file: ")<<fullname.c_str()<<std::endl;

                LoadRawGLSL(fullname);
            }
            else
            {
                os_out<<OS_TEXT("        file: ")<<fullname.c_str()<<std::endl;
            }

            file_list.Add(fullname);
        }
    }
}//namespace shader_lib
