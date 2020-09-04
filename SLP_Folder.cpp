#include"ShaderLibParse.h"
#include"ShaderLib.h"
#include<hgl/filesystem/FileSystem.h>
#include<iostream>

namespace shader_lib
{
    bool FolderElementCreater::Init()
    {
        path.Clear();
        return(true);
    }

    void FolderElementCreater::Attr(const u8char *flag,const u8char *info)
    {
        if(stricmp(flag,"path")==0)path=ToOSString(info);
    }

    void FolderElementCreater::End()
    {
        const OSString fullname=filesystem::MergeFilename(uplevel_path,path);

        os_out<<OS_TEXT("sub folder: ")<<fullname.c_str()<<std::endl;

        LoadFromFolder(fullname);
    }
}//namespace shader_lib
