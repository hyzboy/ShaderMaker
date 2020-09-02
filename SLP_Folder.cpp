#include"ShaderLibParse.h"
#include<hgl/filesystem/FileSystem.h>
#include<iostream>

namespace shader_lib
{
    bool FolderElementCreater::Start()
    {
        path.Clear();
        return(true);
    }

    void FolderElementCreater::Attr(const u8char *flag,const u8char *info)
    {
        if(stricmp(flag,"path")==0)path=ToOSString(info);else
        if(stricmp(flag,"type")==0)
        {
            if(stricmp(info,"module")==0)type=FolderType::Module;else
            if(stricmp(info,"varying")==0)type=FolderType::Varying;else
            {
                path.Clear();
            }
        }
    }

    void FolderElementCreater::End()
    {
        constexpr os_char *typename_string[]={OS_TEXT("module"),OS_TEXT("varying")};

        const OSString fullname=filesystem::MergeFilename(uplevel_path,path);

        os_out<<OS_TEXT("sub folder[")<<typename_string[size_t(type)]<<OS_TEXT("]: ")<<fullname.c_str()<<std::endl;
    }
}//namespace shader_lib
