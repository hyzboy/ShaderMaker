#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/SplitString.h>
#include<hgl/type/LoadStringList.h>
#include"ShaderLibrary/ShaderLib.h"

namespace shader_lib
{
    struct MaterialShader
    {
        ShaderType shader_type;

        OSString origin_filename;
        UTF8String short_name;

        UTF8StringList includes;
        UTF8StringList in,out;
        UTF8StringList const_values;
        ObjectList<Uniform> ubo;
        ObjectList<Uniform> unifroms;

        GeometryAttribute geom;

        UTF8StringList codes;
    };

    namespace
    {
        void ParseInclude(UTF8StringList &inc_list,const UTF8String &str)
        {
            //example: #include<abc.glsl>

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
            //example:  #in     vec2 TexCoord
            //          #out    vec2 TexCoord

            char *p=str.c_str()+4;

            UTF8StringList ins;

            int count=SplitToStringListByCodes(ins,str);

            for(int i=0;i<count;i++)
            {
                if(in_list.Find(ins[i])!=-1)
                    in_list.Add(ins[i]);
            }
        }

        Uniform *ParseUniform(const UTF8String &str)
        {
            //example: #ubo CameraInfo g_camera;
            //example: uniform sampler2D BaseColor;

            UTF8StringList sl;

            SplitToStringListByCodes(sl,str);

            if(sl.GetCount()<3)return(nullptr);

            return(new Uniform(sl[1],sl[2]));
        }

        const u8char *gs_in_list[]=
        {
            "points",
            "lines",
            "line_adjacency",
            "triangles",
            "triangles_adjacency"
        };

        constexpr uint gs_in_count=sizeof(gs_in_list)/sizeof(const u8char *);

        const u8char *gs_out_list[]=
        {
            "points",
            "line_strip",
            "triangle_strip"
        };

        constexpr uint gs_out_count=sizeof(gs_out_list)/sizeof(const u8char *);

        bool ParseGeometry(GeometryAttribute &ga,const UTF8String &str)
        {
            //example: #geometry points,triangle_strip,4

            UTF8StringList sl;

            SplitToStringListByChar(sl,str.c_str()+10,str.Length()-10,U8_TEXT(','));

            if(!sl[2].ToUint(ga.max_vertices))
                return(false);

            if(ga.max_vertices==0)
                return(false);

            ga.in=sl[0];
            ga.out=sl[1];

            if(find_str_array(gs_in_count,gs_in_list,ga.in.c_str())==-1)
                return(false);

            if(find_str_array(gs_out_count,gs_out_list,ga.out.c_str())==-1)
                return(false);

            return(true);
        }

        const u8char *shader_value_type_name_list[]=
        {
            "bool",
            "float",
            "int",
            "uint",
            "double",

            "bvec", "vec", "ivec", "uvec", "dvec",
            "bvec2","vec2","ivec2","uvec2","dvec2",
            "bvec3","vec3","ivec3","uvec3","dvec3",
            "bvec4","vec4","ivec4","uvec4","dvec4"
        };

        constexpr uint shader_value_type_name_count=sizeof(shader_value_type_name_list)/sizeof(const u8char *);

        bool ParseConst(UTF8StringList &const_values,const UTF8String &str)
        {
            //example: const float alpha=0;

            UTF8StringList sl;
            
            SplitToStringListByChars(sl,str,UTF8String(" =;"));

            if(sl[0]!="const")return(false);
            
            if(find_str_array(shader_value_type_name_count,shader_value_type_name_list,sl[1].c_str())==-1)
                return(false);

            const_values.Add(str.SubString(6));

            return(true);
        }
    }//namespace

    MaterialShader *LoadMaterialShader(const ShaderType &type,const OSString &filename,InfoOutput *info_output)
    {
        UTF8StringList sl;

        MaterialShader *ms=new MaterialShader;

        ms->shader_type=type;
        ms->origin_filename=filename;
        ms->short_name=ToUTF8String(filesystem::ClipFileMainname(filename));

        int line_count=LoadStringListFromTextFile(sl,filename);

        if(line_count<=0)
            return(nullptr);

        for(int i=0;i<line_count;i++)
        {
            const UTF8String str=sl[i].Trim();

            if(str.GetFirstChar()=='#')
            {
                if(str.Comp("#include ")==0)
                {
                    ParseInclude(ms->includes,str);

                    continue;
                }
                else if(str.Comp("#in ")==0)
                {
                    ParseIn(ms->in,str);
                }
                else if(str.Comp("#out ")==0)
                {
                    ParseIn(ms->out,str);
                }
                else if(str.Comp("#ubo ")==0)
                {
                    Uniform *u=ParseUniform(str);

                    if(u)
                        ms->ubo.Add(u);
                }
                else if(str.Comp("#geometry ")==0)
                {
                    ParseGeometry(ms->geom,str);
                }
            }
            else if(str.Comp("uniform ")==0)
            {
                Uniform *u=ParseUniform(str);

                if(u)
                    ms->unifroms.Add(u);
            }
            else if(str.Comp("const ")==0)
            {
                ParseConst(ms->const_values,str);
            }
            else
            {
                ms->codes.Add(sl[i]);
            }
        }

        return ms;
    }
}//namespace shader_lib
