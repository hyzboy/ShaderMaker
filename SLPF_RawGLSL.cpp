#include<hgl/type/StringList.h>
#include<hgl/type/Map.h>
#include<hgl/filesystem/FileSystem.h>

namespace shader_lib
{
    using namespace hgl;

    namespace
    {
        MapObject<UTF8String,UTF8StringList> raw_shader_list;
    }

    bool CheckRawModule(const UTF8String &name)
    {
        return raw_shader_list.KeyExist(name);
    }
    
    bool CheckRawModule(const UTF8StringList &raw_list)
    {        
        const int count=raw_list.GetCount();

        if(count<=0)return(true);

        for(int i=0;i<count;i++)
            if(!CheckRawModule(raw_list.GetString(i)))return(false);

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

        raw_shader_list.Add(to_u8(fn),file);

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
