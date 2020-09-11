#include"ShaderLibParse.h"
#include"ShaderLib.h"
#include<hgl/filesystem/FileSystem.h>
#include<iostream>

namespace shader_lib
{
    void FolderElementCreater::CharData(const u8char *str,const int str_length)
    {
        int len=str_length;
        const u8char *trim_str=trim(str,len);

        if(len<=0)return;

        const OSString path=to_oschar(trim_str,len);
        const OSString fullname=filesystem::MergeFilename(uplevel_path,path);

        os_out<<OS_TEXT("sub folder: ")<<fullname.c_str()<<std::endl;

        LoadFromFolder(fullname);
    }
}//namespace shader_lib
