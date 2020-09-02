#include"ShaderLibParse.h"

namespace shader_lib
{
    ShaderLibRootElementCreater::ShaderLibRootElementCreater(const OSString &cp):xml::ElementCreater("root")
    {
        cur_path=cp;

        folder=new FolderElementCreater(cur_path);
        files=new FilesElementerCreater();

        Registry(folder);
        Registry(files);
    }

    ShaderLibRootElementCreater::~ShaderLibRootElementCreater()
    {
        SAFE_CLEAR(files);
        SAFE_CLEAR(folder);
    }
}//namespace shader_lib
