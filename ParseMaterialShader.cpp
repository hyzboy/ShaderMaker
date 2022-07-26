#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/StringList.h>
#include"ShaderLib.h"

namespace shader_lib
{
    bool LoadMaterialShader(const ShaderType &type,const OSString &filename,InfoOutput *info_output)
    {
        UTF8StringList sl;

        int line_count=LoadStringListFromTextFile(sl,filename);

        if(line_count<=0)
            return(false);


    }
}//namespace shader_lib
