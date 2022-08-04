#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/StringList.h>
#include"ShaderLib.h"

namespace shader_lib
{
    struct MaterialShader
    {
        ShaderType shader_type;

        OSString origin_filename;
        UTF8String short_name;

        UTF8StringList includes;
        UTF8StringList in,out;
        GeometryAttribute geom;
    };

    namespace
    {
        void ParseInclude(UTF8StringList &inc_list,const UTF8String &str)
        {
            int left=str.FindChars("\"<");

            if(left==-1)
                return;

            int right=str.FindChars(left+1,"\">");

            if(right==-1)
                return;

            inc_list.Add(str.SubString(left+1,right-left-1));
            return;
        }

        void ParseIn(UTF8StringList &in_list,const UTF8String &str)
        {
            char *p=str.c_str()+4;

            SplitToStringListBySpace
        }
    }//namespace

    bool LoadMaterialShader(const ShaderType &type,const OSString &filename,InfoOutput *info_output)
    {
        UTF8StringList sl;

        MaterialShader *ms=new MaterialShader;

        ms->shader_type=type;
        ms->origin_filename=filename;
        ms->short_name=ToUTF8String(filesystem::ClipFileMainname(filename));

        int line_count=LoadStringListFromTextFile(sl,filename);

        if(line_count<=0)
            return(false);

        for(int i=0;i<line_count;i++)
        {
            const UTF8String str=sl[i].Trim();

            if(str.GetFirstChar()=='#')
            {
                if(str.Comp("include")==0)
                {
                    ParseInclude(ms->includes,str);

                    continue;
                }
                else if(str.Comp("in ")==0)
                {
                    ParseIn(ms->in,str);
                }
            }
        }
    }
}//namespace shader_lib
