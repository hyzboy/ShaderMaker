#include"ShaderLib.h"
#include"ShaderModule.h"
#include"ShaderLibParse.h"
#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>

namespace shader_lib
{
    using namespace hgl;

    constexpr os_char shader_libs_filename[]=OS_TEXT("shader_libs.xml");

    bool Init(const OSString &path)
    {
        const OSString filename=filesystem::MergeFilename(path,OSString(shader_libs_filename));

        if(!filesystem::FileExist(filename))
            return(false);

        ShaderLibRootElementCreater root_ec(path);
        xml::ElementParseCreater epc(&root_ec);
        xml::XMLParse xml(&epc);

        xml.Start();

        return xml::XMLParseFile(&xml,filename);
    }

    void Clear()
    {
    }
}//namespace shader_lib
