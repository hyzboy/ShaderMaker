#include"ShaderLib.h"
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/Map.h>

using namespace hgl;
using namespace hgl::filesystem;

namespace shader_lib
{
    namespace
    {
        MapObject<int,UTF8StringList> shader_lib;
    }//namespace

    int Load(const OSString &filename)
    {
        if(!FileExist(filename))
            return(-1);

        UTF8StringList *sl=new UTF8StringList;

        if(LoadStringListFromTextFile(*sl,filename)<=0)
        {
            delete sl;
            return(-2);
        }

        const int index=shader_lib.GetCount();

        shader_lib.Add(index,sl);

        return index;
    }

    void Clear()
    {
        shader_lib.DeleteAll();
    }

    bool AddTo(UTF8StringList &sl_out, const int index)
    {
        UTF8StringList *sl=shader_lib[index];

        if(!sl)return(false);

        sl_out.Add(*sl);

        return(true);
    }
}//namespace shader_lib
