#include<hgl/filesystem/Filename.h>
#include<hgl/type/StringList.h>
#include"ShaderLib.h"

namespace shader_lib
{
    namespace
    {
        UTF8StringList varying_list;
        MapObject<UTF8String,VaryingConfig> varying_config_list;
    }//namespace

    const UTF8StringList &GetVaryingList()
    {
        return varying_list;
    }

    bool CheckVarying(const UTF8String &name)
    {
        return varying_config_list.KeyExist(name);
    }
        
    bool CheckVarying(const UTF8StringList &vary_list,InfoOutput *info_output)
    {
        const int count=vary_list.GetCount();

        if(count<=0)return(true);

        for(int i=0;i<count;i++)
        {
            const auto &vary=vary_list.GetString(i);

            if(!CheckVarying(vary))
            {
                if(info_output)
                    info_output->colorWriteln("red","can't find Varing: "+vary);

                return(false);
            }
        }

        return(true);
    }

    VaryingConfig *GetVarying(const UTF8String &name)
    {
        return varying_config_list[name];
    }

    namespace
    {
        Varying *ParseVarying(const UTF8String &str)
        {

        }
    }//namespace

    bool LoadVarying(const hgl::OSString &filename)
    {
        UTF8StringList sl;

        int line_count=LoadStringListFromTextFile(sl,filename);

        if(line_count<=0)return(false);

        VaryingConfig *vc=new VaryingConfig;

        for(int i=0;i<line_count;i++)
        {
            Varying *var;

            var=ParseVarying(sl[i]);
            if(var)vc->Add(var);
        }

        if(vc->GetCount()<=0)
        {
            delete vc;
            return(false);
        }

        {
            const UTF8String mfn=ToUTF8String(filesystem::ClipFileMainname(filename));

            varying_list.Add(mfn);

            varying_config_list.Add(mfn,vc);

            return(true);
        }
    }
}//namespace shader_lib
