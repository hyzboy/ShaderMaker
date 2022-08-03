#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/StringList.h>
#include"ShaderLib.h"

namespace shader_lib
{
    class MaterialShader
    {
        ShaderType shader_type;

        OSString origin_filename;
        UTF8String short_name;

        UTF8StringList includes;
        UTF8StringList in,out;
        GeometryAttribute geom;
    };

    bool LoadMaterialShader(const ShaderType &type,const OSString &filename,InfoOutput *info_output)
    {
        UTF8StringList sl;

        int line_count=LoadStringListFromTextFile(sl,filename);

        if(line_count<=0)
            return(false);

        for(int i=0;i<line_count;i++)
        {
            
        }
    }
}//namespace shader_lib
