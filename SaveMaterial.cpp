#include"ShaderLib.h"
#include"GLSLCompiler.h"
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/graph/VertexAttrib.h>

namespace shader_lib
{
    using namespace hgl;
    using namespace hgl::io;
    using namespace hgl::graph;

    class MaterialOutput
    {
        MemoryOutputStream mos;
        DataOutputStream *dos;

        XMLMaterial *xml;
        InfoOutput *info_output;

        MSRList msr_list;

    public:

        MaterialOutput(XMLMaterial *xm,InfoOutput *ifo)
        {
            xml=xm;
            info_output=ifo;

            dos=new LEDataOutputStream(&mos);

            info_output->colorWriteln("red","Start of Output material.");
        }

        ~MaterialOutput()
        {
            info_output->colorWriteln("red","Finished of Output material.");
            delete dos;
        }

        void WriteFileHeader()
        {
            constexpr char MATERIAL_FILE_HEADER[]=u8"Material\x1A";
            constexpr uint MATERIAL_FILE_HEADER_LENGTH=sizeof(MATERIAL_FILE_HEADER)-1;
            
            dos->Write(MATERIAL_FILE_HEADER,MATERIAL_FILE_HEADER_LENGTH);
            dos->WriteUint8(2);                                                      //version

            dos->WriteUint32(xml->shader_stage_bits);
        }

        void StatShaderResource(const ShaderType flag,const glsl_compiler::ShaderResourceData *srd,const char *hint)
        {
            if(srd->count<=0)return;

            std::cout<<srd->count<<" "<<hint<<std::endl;

            const ShaderResource *sr=srd->items;

            MaterialShaderResource *msr;

            for(size_t i=0;i<srd->count;i++)
            {
                if(msr_list.Get(sr->name,msr))
                {
                    msr->shader_stage_flag|=flag;
                }
                else
                {
                    msr=new MaterialShaderResource(flag,sr);

                    msr_list.Add(msr->name,msr);

                    info_output->colorWriteln("black","    layout(set=%d,binding=%d) uniform %s",sr->set,sr->binding,sr->name);
                }

                ++sr;
            }
        }

        void StatShaderResource(const ShaderType flag,const glsl_compiler::ShaderFullResourceData &resource)
        {
            StatShaderResource(flag,&(resource[(size_t)DescriptorType::COMBINED_IMAGE_SAMPLER]),"combined_image_sampler");
            StatShaderResource(flag,&(resource[(size_t)DescriptorType::UNIFORM_BUFFER]),"uniform_buffer");
            StatShaderResource(flag,&(resource[(size_t)DescriptorType::STORAGE_BUFFER]),"storage_buffer");
        }
        
        void OutputShaderStage(const glsl_compiler::ShaderStageData &ssd,const char *hint)
        {
            dos->WriteUint8(uint8(ssd.count));

            if(ssd.count<=0)return;

            MemoryOutputStream mmos;
            AutoDelete<DataOutputStream> mdos=new LEDataOutputStream(&mmos);

            const glsl_compiler::ShaderStage *ss=ssd.items;
            VertexAttribType vat;
            const char *type_string;

            info_output->colorWriteln("blue",UTF8String::valueOf(ssd.count)+U8_TEXT(" Stage ")+UTF8String(hint));

            for(size_t i=0;i<ssd.count;i++)
            {
                mdos->WriteUint8(ss->location);
                mdos->WriteUint8(ss->basetype);
                mdos->WriteUint8(ss->vec_size);
                mdos->WriteAnsiTinyString(ss->name);

                vat.basetype=VertexAttribBaseType(ss->basetype);
                vat.vec_size=ss->vec_size;

                type_string=GetVertexAttribName(&vat);

                info_output->colorWriteln("black","    layout(location=%d) %s %s %s",ss->location,hint,type_string,ss->name);
                ++ss;
            }

            dos->WriteUint32(mmos.Tell());
            dos->Write(mmos.GetData(),mmos.Tell());
        }

        void WriteSPV(const ShaderType flag,const glsl_compiler::SPVData *spv)
        {
            dos->WriteUint32(flag);
            dos->WriteUint32(spv->spv_length);
            dos->Write(spv->spv_data,spv->spv_length);

            OutputShaderStage(spv->input,"in");
            OutputShaderStage(spv->output,"out");

            StatShaderResource(flag,spv->resource);
        }

        void WriteSPV()
        {
            const uint count=xml->shader_map.GetCount();
            auto **sp=xml->shader_map.GetDataList();

            for(uint i=0;i<count;i++)
            {
                WriteSPV((*sp)->left,
                         (*sp)->right->spv_data);

                ++sp;
            }
        }

        void WriteDescriptorSet()
        {
            
        }

        bool OutputToFile(const OSString &filename)
        {
            mos.ClearData();

            WriteFileHeader();
            WriteSPV();


        }
    };//class MaterialOutput    
    
    void OutputShaderResource(const ShaderResourceData *srd_arrays,const DescriptorType type,DataOutputStream *dos,const char *hint)
    {
        const ShaderResourceData *srd=srd_arrays+(size_t)type;

        if(srd->count<=0)return;

        dos->WriteUint32((uint32_t)type);

        MemoryOutputStream mos;
        AutoDelete<DataOutputStream> mdos=new LEDataOutputStream(&mos);

        std::cout<<srd->count<<" "<<hint<<std::endl;

        mdos->WriteUint8(uint8(srd->count));

        const ShaderResource *sr=srd->items;

        for(size_t i=0;i<srd->count;i++)
        {
            mdos->WriteUint8(sr->set);
            mdos->WriteUint8(sr->binding);
            mdos->WriteAnsiTinyString(sr->name);

            std::cout<<"    layout(set="<<int(sr->set)<<",binding="<<int(sr->binding)<<") uniform "<<sr->name<<std::endl;
            ++sr;
        }

        dos->WriteUint32(mos.Tell());
        dos->Write(mos.GetData(),mos.Tell());
    }

    void SaveSPV(io::DataOutputStream *global_dos,const ShaderType flag,glsl_compiler::SPVData *spv)
    {
        io::MemoryOutputStream mos;

        glsl_compiler::SaveSPV(&mos,spv,flag);

        global_dos->WriteUint32(mos.Tell());
        global_dos->Write(mos.GetData(),mos.Tell());
    }

    void SaveShaderDescriptorSet(io::DataOutputStream *dos,MaterialStat *stat,InfoOutput *info_output)
    {
    }

    bool SaveMaterial(const OSString &filename,XMLMaterial *xm,InfoOutput *info_output)
    {
        if(!xm)return(false);


        io::MemoryOutputStream mos;
        io::LEDataOutputStream dos(&mos);


        SaveShaderDescriptorSet(&dos,&xm->mtl_stat,info_output);

        const uint count=xm->shader_map.GetCount();
        auto **sp=xm->shader_map.GetDataList();
        for(uint i=0;i<count;i++)
        {
            SaveSPV(&dos,
                (*sp)->left,
                (*sp)->right->spv_data);

            ++sp;
        }

        if(filesystem::SaveMemoryToFile(filename,mos.GetData(),mos.Tell())==mos.Tell())
        {
            info_output->colorWriteln("green",OS_TEXT("Save material file \"<b>")+filename+OS_TEXT("</b>\" OK! total ")+OSString::valueOf(mos.Tell())+OS_TEXT(" bytes."));
            return(true);
        }
        else
        {
            info_output->colorWriteln("red",OS_TEXT("Save material file \"")+filename+OS_TEXT("\" failed!"));
            return(false);
        }
    }
}//namespace shader_lib
