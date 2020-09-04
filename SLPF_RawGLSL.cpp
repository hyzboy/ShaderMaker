#include<hgl/type/StringList.h>

namespace shader_lib
{
    using namespace hgl;

    bool LoadRawGLSL(const OSString &filename)
    {
        UTF8StringList file;

        if(LoadStringListFromTextFile(file,filename)<=0)
        {
            os_out<<"      RawGLSL Load failed: "<<filename.c_str()<<std::endl;
            return(false);
        }

        os_out<<"      RawGLSL Load successed: "<<filename.c_str()<<std::endl;

        const int count=file.GetCount();

        for(int i=0;i<count;i++)
        {
            const UTF8String &line=file.GetString(i);

            if(line.IsEmpty())
                std::cout<<std::endl;
            else
                std::cout<<"        "<<line.c_str()<<std::endl;
        }

        return(true);
    }
}//namespace shader_lib
