#include"GLSLCompiler.h"
#include<hgl/platform/ExternalModule.h>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/Map.h>
#include<hgl/io/FileOutputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/graph/VertexAttrib.h>

using namespace hgl;
using namespace hgl::io;
using namespace hgl::graph;

namespace glsl_compiler
{
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

    void OutputShaderStage(const ShaderStageData &ssd,DataOutputStream *dos,const char *hint)
    {
        dos->WriteUint8(uint8(ssd.count));

        if(ssd.count<=0)return;

        MemoryOutputStream mos;
        AutoDelete<DataOutputStream> mdos=new LEDataOutputStream(&mos);

        const ShaderStage *ss=ssd.items;
        VertexAttribType vat;
        const char *type_string;

        for(size_t i=0;i<ssd.count;i++)
        {
            mdos->WriteUint8(ss->location);
            mdos->WriteUint8(ss->basetype);
            mdos->WriteUint8(ss->vec_size);
            mdos->WriteAnsiTinyString(ss->name);

            vat.basetype=VertexAttribBaseType(ss->basetype);
            vat.vec_size=ss->vec_size;

            type_string=GetVertexAttribName(&vat);

            std::cout<<"layout(location="<<int(ss->location)<<") "<<hint<<" "<<type_string<<" "<<ss->name<<std::endl;
            ++ss;
        }

        dos->WriteUint32(mos.Tell());
        dos->Write(mos.GetData(),mos.Tell());
    }

    void OutputShaderResource(const ShaderResourceData *srd_arrays,const uint32_t type,DataOutputStream *dos,const char *hint)
    {
        const ShaderResourceData *srd=srd_arrays+type;

        if(srd->count<=0)return;

        dos->WriteUint32((uint32_t)type);

        MemoryOutputStream mos;
        AutoDelete<DataOutputStream> mdos=new LEDataOutputStream(&mos);

        std::cout<<srd->count<<" "<<hint<<std::endl;

        mdos->WriteUint8(uint8(srd->count));

        const ShaderResource *sr=srd->items;

        for(size_t i=0;i<srd->count;i++)
        {
            mdos->WriteUint8(sr->set);          //append in version 1
            mdos->WriteUint8(sr->binding);
            mdos->WriteAnsiTinyString(sr->name);

            std::cout<<"layout(set="<<int(sr->set)<<",binding="<<int(sr->binding)<<") uniform "<<sr->name<<std::endl;
            ++sr;
        }

        dos->WriteUint32(mos.Tell());
        dos->Write(mos.GetData(),mos.Tell());
    }

    SPVData *CompileShaderToSPV(const uint8 *source,const uint32_t flag)
    {
        if(source[0]==0xEF
         &&source[1]==0xBB
         &&source[2]==0xBF)
           source+=3;

        glsl_compiler::SPVData *spv=glsl_compiler::Compile(flag,(char *)source);

        if(!spv)return(false);

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

    constexpr char SHADER_FILE_HEADER[]="Shader\x1A";
    constexpr uint SHADER_FILE_HEADER_BYTES=sizeof(SHADER_FILE_HEADER)-1;

    bool SaveSPV2Shader(MemoryOutputStream *mos,const SPVData *spv,const uint32_t flag,const bool include_file_header)
    {
        if(!mos)return(false);
        if(!spv)return(false);

        AutoDelete<DataOutputStream> dos=new LEDataOutputStream(mos);

        if(include_file_header)
            dos->Write(SHADER_FILE_HEADER,SHADER_FILE_HEADER_BYTES);

        dos->WriteUint8(1);     //version
        dos->WriteUint32(flag);
        dos->WriteUint32(spv->spv_length);
        dos->Write(spv->spv_data,spv->spv_length);

        OutputShaderStage(spv->input,dos,"in");
        OutputShaderStage(spv->output,dos,"out");
        
        OutputShaderResource(spv->resource,(uint32_t)Descriptor::COMBINED_IMAGE_SAMPLER,dos,"combined_image_sampler");
        OutputShaderResource(spv->resource,(uint32_t)Descriptor::UNIFORM_BUFFER,dos,"uniform_buffer");
        OutputShaderResource(spv->resource,(uint32_t)Descriptor::STORAGE_BUFFER,dos,"storage_buffer");

        return(true);
    }

    bool SaveSPV2Shader(const OSString &filename,const SPVData *spv,const uint32_t flag)
    {
        if(!spv)return(false);

        MemoryOutputStream mos;

        SaveSPV2Shader(&mos,spv,flag,true);

        if(filesystem::SaveMemoryToFile(filename,mos.GetData(),mos.Tell()))
        {
            os_out<<OS_TEXT("Save Shader file [")<<filename.c_str()<<OS_TEXT("] successed, total ")<<mos.Tell()<<OS_TEXT(" bytes.")<<std::endl;
            return(true);
        }
        else
        {
            os_err<<OS_TEXT("Save Shader file [")<<filename.c_str()<<OS_TEXT("] failed!")<<std::endl;
            return(false);
        }
    }

    bool CompileShader(const OSString &filename)
    {
        int64 size;

        AutoDeleteArray<uint8> source=(uint8 *)filesystem::LoadFileToMemory(filename,size,true);

        if(!source)
        {
            os_err<<OS_TEXT("Load Shader file [")<<filename.c_str()<<OS_TEXT("] failed!")<<std::endl;
            return(false);
        }
        else
        {
            os_out<<OS_TEXT("Load Shader file [")<<filename.c_str()<<OS_TEXT("] successed!")<<std::endl;
        }

        const UTF8String ext_name=to_u8(filesystem::ClipFileExtName(filename,false));

        const uint32_t flag=glsl_compiler::GetType(ext_name.c_str());

        SPVData *spv=CompileShaderToSPV(source,flag);

        if(!spv)
            return(false);

        return SaveSPV2Shader(filename+OS_TEXT(".shader"),spv,flag);
    }
}//}//namespace glsl_compiler
