#include"GLSLCompiler.h"
#include<hgl/platform/ExternalModule.h>

using namespace hgl;
using namespace hgl::io;

namespace glsl_compiler
{
    enum class ShaderLanguageType
    {
        GLSL=0,
        HLSL,

        MAX=0xff
    };//enum class ShaderType

    struct CompileInfo
    {
        ShaderLanguageType shader_type = ShaderLanguageType::GLSL;
        const char *entrypoint; 
        uint32_t includes_count;
        const char **includes;
    };

    struct GLSLCompilerInterface
    {
        bool        (*Init)();
        void        (*Close)();

        uint32_t    (*GetType)(const char *ext_name);
        SPVData *   (*Compile)(const uint32_t stage,const char *source, const CompileInfo *compile_info);
        SPVData *   (*CompileFromPath)(const uint32_t stage,const char *path, const CompileInfo *compile_info);

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

    ShaderType GetType (const char *ext_name)
    {
        if(gsi)
            return gsi->GetType(ext_name);

        return 0;
    }

    SPVData *   Compile (const uint32_t type,const char *source)
    {
        if(gsi)
            return gsi->Compile(type,source,nullptr);

        return nullptr;
    }

    void        Free    (SPVData *spv_data)
    {
        if(gsi)
            gsi->Free(spv_data);
    }

    SPVData *CompileShaderToSPV(const uint8 *source,const uint32_t flag)
    {
        ByteOrderMask bom=CheckBOM(source);

        if(bom==ByteOrderMask::UTF8)
            source+=3;
        else
        if(bom!=ByteOrderMask::NONE)
        {
            std::cerr<<"GLSLCompiler does not support BOMHeader outside of UTF8"<<std::endl;
            return(nullptr);
        }

        glsl_compiler::SPVData *spv=glsl_compiler::Compile(flag,(char *)source);

        if(!spv)return(nullptr);

        const bool result=spv->result;

        if(!result)
        {
            std::cerr<<"glsl compile failed."<<std::endl;
            std::cerr<<"info: "<<spv->log<<std::endl;
            std::cerr<<"debug info: "<<spv->debug_log<<std::endl;

            glsl_compiler::Free(spv);
            return(nullptr);
        }

        std::cout<<"Compile successed! spv length "<<spv->spv_length<<" bytes."<<std::endl;

        return spv;
    }
}//namespace glsl_compiler
