#include<hgl/type/LoadStringList.h>
#include<hgl/type/Map.h>
#include<hgl/filesystem/FileSystem.h>
#include<iostream>
#include"InfoOutput/InfoOutput.h"

namespace shader_lib
{
    using namespace hgl;

    namespace
    {
        UTF8StringList raw_shader_name_list;
        ObjectMap<UTF8String,UTF8StringList> raw_shader_list;
    }

    const UTF8StringList &GetRawModuleList()
    {
        return raw_shader_name_list;
    }

    bool CheckRawModule(const UTF8String &name)
    {
        return raw_shader_list.KeyExist(name);
    }
    
    bool CheckRawModule(const UTF8StringList &raw_list,InfoOutput *info_output)
    {        
        const int count=raw_list.GetCount();

        if(count<=0)return(true);

        for(const UTF8String *name:raw_list)
        {
            if(!CheckRawModule(*name))
            {
                info_output->colorWriteln("red",u8"can't find RAW Module: "+(*name));
                return(false);
            }
        }

        return(true);
    }

    UTF8StringList *GetRawModule(const UTF8String &name)
    {
        return raw_shader_list[name];
    }

    bool LoadRawGLSL(const OSString &filename)
    {
        UTF8StringList *file=new UTF8StringList;

        if(LoadStringListFromTextFile(*file,filename)<=0)
        {
            os_out<<"      RawGLSL Load failed: "<<filename.c_str()<<std::endl;
            return(false);
        }

        os_out<<"      RawGLSL Load successed: "<<filename.c_str()<<std::endl;

        const OSString fn=filesystem::ClipFileMainname(filename);

        const UTF8String raw_name=to_u8(fn);

        std::cout<<u8"Raw Module: "<<raw_name.c_str()<<std::endl;

        raw_shader_list.Add(raw_name,file);
        raw_shader_name_list.Add(raw_name);

        const int count=file->GetCount();

        for(int i=0;i<count;i++)
        {
            const UTF8String &line=file->GetString(i);

            if(line.IsEmpty())
                std::cout<<std::endl;
            else
                std::cout<<"        "<<line.c_str()<<std::endl;
        }

        return(true);
    }
}//namespace shader_lib
