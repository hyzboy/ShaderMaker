#include"ShaderLibParse.h"

namespace shader_lib
{
    ShaderLibRootElementCreater::ShaderLibRootElementCreater():xml::ElementCreater("root")
    {
        folder=new FolderElementCreater();
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
