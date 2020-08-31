#include"ShaderLibParse.h"

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
                
    }
}//namespace shader_lib
