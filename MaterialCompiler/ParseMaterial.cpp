#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/LoadStringList.h>
#include"ShaderLibrary/ShaderLib.h"
#include"GLSLCompiler/GLSLCompiler.h"

namespace shader_lib
{
    namespace
    {
        bool SplitShaderDefineString(ShaderType &type,UTF8String &filename,const UTF8String &str)
        {
            if(str.Length()<=5)
                return(false);

            int pos=str.FindChars(U8_TEXT("\t ="));     //寻找\t、空格、=

            if(pos<4)
                return(false);

            char front[5];

            hgl::strcpy(front,4,str.c_str());
            front[4]=0;

            type=glsl_compiler::GetType(front);

            if(type==0)
                return(false);
            
            filename=str.SubString(pos+1);
            return(true);
        }
    }//namespace

    bool LoadMaterial(const OSString &filename,InfoOutput *info_output)
    {
        UTF8StringList sl;

        int line_count=LoadStringListFromTextFile(sl,filename);

        if(line_count<=0)
            return 0;

        uint shader_count=0;

        uint32_t shader_stage_bits=0;

        ShaderType type;
        UTF8String shader_fn;

        for(int i=0;i<line_count;i++)
        {
            if(SplitShaderDefineString(type,shader_fn,sl[i]))
            {
                shader_stage_bits|=type;
            }
        }

        return(true);
    }
}//namespace shader_lib
