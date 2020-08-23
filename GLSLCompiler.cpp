#include"GLSLCompiler.h"
#include<hgl/platform/ExternalModule.h>

namespace glsl_compiler
{
    using namespace hgl;

    struct GLSLCompilerInterface
    {
        bool        (*Init)();
        void        (*Close)();

        uint32_t    (*GetType)(const char *ext_name);
        SPVData *   (*Compile)(const uint32_t type,const char *source);

        void        (*Free)(SPVData *);
    };

    ExternalModule *gsi_module=nullptr;

    static GLSLCompilerInterface *gsi=nullptr;

    typedef GLSLCompilerInterface *(*GetInterfaceFUNC)();

    bool Init()
    {
        if(gsi)return(true);

        constexpr os_char filename[]=OS_TEXT("GLSLCompiler") HGL_PLUGIN_EXTNAME;

        gsi_module=LoadExternalModule(filename);

        if(!gsi_module)return(false);

        GetInterfaceFUNC get_func;

        get_func=GetInterfaceFUNC(gsi_module->GetFunc("GetInterface"));

        if(get_func)
        {
            gsi=get_func();
            if(gsi)
            {
                if(gsi->Init())
                    return(true);
            }
        }

        delete gsi_module;
        gsi_module=nullptr;
        return(false);
    }

    void Close()
    {
        if(gsi)
        {
            gsi->Close();
            gsi=nullptr;
        }

        if(gsi_module)
        {
            delete gsi_module;
            gsi_module=nullptr;
        }
    }

    uint32_t    GetType (const char *ext_name)
    {
        if(gsi)
            return gsi->GetType(ext_name);

        return 0;
    }

    SPVData *   Compile (const uint32_t type,const char *source)
    {
        if(gsi)
            return gsi->Compile(type,source);

        return nullptr;
    }

    void        Free    (SPVData *spv_data)
    {
        if(gsi)
            gsi->Free(spv_data);
    }
}//namespace glsl_compiler