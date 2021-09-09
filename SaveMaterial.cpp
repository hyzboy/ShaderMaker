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

    private:

        void WriteFileHeader()
        {
            constexpr char MATERIAL_FILE_HEADER[]=u8"Material\x1A";
            constexpr uint MATERIAL_FILE_HEADER_LENGTH=sizeof(MATERIAL_FILE_HEADER)-1;
            
            dos->Write(MATERIAL_FILE_HEADER,MATERIAL_FILE_HEADER_LENGTH);
            dos->WriteUint8(2);                                                      //version

            dos->WriteUint32(xml->shader_stage_bits);
        }

        void StatShaderResource(const ShaderType flag,const glsl_compiler::ShaderResourceData *srd,const uint type,const char *hint)
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
                    msr=new MaterialShaderResource(flag,(DescriptorType)type,sr);

                    msr_list.Add(msr->name,msr);

                    info_output->colorWriteln("gray","    layout(set=%d,binding=%d) uniform %s",sr->set,sr->binding,sr->name);
                }

                ++sr;
            }
        }

        void StatShaderResource(const ShaderType flag,const glsl_compiler::ShaderFullResourceData &resource)
        {
            ENUM_CLASS_FOR(DescriptorType,int,i)
            {
                StatShaderResource(flag,&resource[i],i,descriptor_type_name[i]);
            }
        }

        void WriteMSR(MaterialShaderResource *msr)
        {   
            dos->WriteUint8((uint8)(msr->desc_type));
            dos->WriteAnsiTinyString(msr->name);
            dos->WriteUint8(msr->set);
            dos->WriteUint8(msr->binding);
            dos->WriteUint32(msr->shader_stage_flag);
        }
        
        void OutputShaderStage(DataOutputStream *mdos,const glsl_compiler::ShaderStageData &ssd,const char *hint)
        {
            mdos->WriteUint8(uint8(ssd.count));

            if(ssd.count<=0)return;

            const glsl_compiler::ShaderStage *ss=ssd.items;
            VertexAttribType vat;
            const char *type_string;
            int location;

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

                location=ss->location;

                info_output->colorWriteln("black","    layout(location=%d) %s %s %s",location,hint,type_string,ss->name);
                ++ss;
            }
        }

        void WriteSPV(const ShaderType flag,const glsl_compiler::SPVData *spv)
        {
            MemoryOutputStream mmos;
            AutoDelete<DataOutputStream> mdos=new LEDataOutputStream(&mmos);

            mdos->WriteUint32(flag);
            mdos->WriteUint32(spv->spv_length);
            mdos->Write(spv->spv_data,spv->spv_length);

            OutputShaderStage(mdos,spv->input,"in");
            OutputShaderStage(mdos,spv->output,"out");

            StatShaderResource(flag,spv->resource);     //注意：因为各种资源可能会有VS/GS/FS共用的情况，所以这里只有统计，并不输出

            dos->WriteUint32(mmos.Tell());
            dos->Write(mmos.GetData(),mmos.Tell());
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

        void OutputShaderStageName(AnsiString &str,const uint32_t &type)
        {
            uint32_t index=0;
            uint32_t bit=1;

            str.Clear();
            bool first=true;

            while(bit<=ssbCallable)
            {
                if(type&bit)
                {
                    if(!first)
                    {
                        str.Strcat(",");
                    }
                    else
                    {
                        first=false;
                    }

                    str.Strcat(shader_stage_name_list[index]);
                }

                bit<<=1;
                ++index;
            }
        }

        void WriteDescriptorSet()
        {
            const uint count=msr_list.GetCount();
            auto **it=msr_list.GetDataList();
            AnsiString flags;

            dos->WriteUint8(count);

            info_output->colorWriteln("blue","%d descriptor sets",count);
            
            for(uint i=0;i<count;i++)
            {
                MaterialShaderResource *msr=(*it)->right;

                OutputShaderStageName(flags,msr->shader_stage_flag);

                info_output->colorWriteln("black","    layout(set=%d,binding=%d) uniform %s; //%s",msr->set,msr->binding,msr->name,flags.c_str());

                WriteMSR(msr);

                ++it;
            }
        }

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

        bool OutputToFile(const OSString &filename)
        {
            mos.ClearData();

            WriteFileHeader();
            WriteSPV();

            WriteDescriptorSet();

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
    };//class MaterialOutput    

    bool SaveMaterial(const OSString &filename,XMLMaterial *xm,InfoOutput *info_output)
    {
        if(!xm)return(false);

        MaterialOutput mo(xm,info_output);

        return mo.OutputToFile(filename);
    }
    
    //void OutputShaderResource(const ShaderResourceData *srd_arrays,const DescriptorType type,DataOutputStream *dos,const char *hint)
    //{
    //    const ShaderResourceData *srd=srd_arrays+(size_t)type;

    //    if(srd->count<=0)return;

    //    dos->WriteUint32((uint32_t)type);

    //    MemoryOutputStream mos;
    //    AutoDelete<DataOutputStream> mdos=new LEDataOutputStream(&mos);

    //    std::cout<<srd->count<<" "<<hint<<std::endl;

    //    mdos->WriteUint8(uint8(srd->count));

    //    const ShaderResource *sr=srd->items;

    //    for(size_t i=0;i<srd->count;i++)
    //    {
    //        mdos->WriteUint8(sr->set);
    //        mdos->WriteUint8(sr->binding);
    //        mdos->WriteAnsiTinyString(sr->name);

    //        std::cout<<"    layout(set="<<int(sr->set)<<",binding="<<int(sr->binding)<<") uniform "<<sr->name<<std::endl;
    //        ++sr;
    //    }

    //    dos->WriteUint32(mos.Tell());
    //    dos->Write(mos.GetData(),mos.Tell());
    //}

    //void SaveSPV(io::DataOutputStream *global_dos,const ShaderType flag,glsl_compiler::SPVData *spv)
    //{
    //    io::MemoryOutputStream mos;

    //    glsl_compiler::SaveSPV(&mos,spv,flag);

    //    global_dos->WriteUint32(mos.Tell());
    //    global_dos->Write(mos.GetData(),mos.Tell());
    //}

    //void SaveShaderDescriptorSet(io::DataOutputStream *dos,MaterialStat *stat,InfoOutput *info_output)
    //{
    //}

    //bool SaveMaterial(const OSString &filename,XMLMaterial *xm,InfoOutput *info_output)
    //{
    //    if(!xm)return(false);


    //    io::MemoryOutputStream mos;
    //    io::LEDataOutputStream dos(&mos);


    //    SaveShaderDescriptorSet(&dos,&xm->mtl_stat,info_output);

    //    const uint count=xm->shader_map.GetCount();
    //    auto **sp=xm->shader_map.GetDataList();
    //    for(uint i=0;i<count;i++)
    //    {
    //        SaveSPV(&dos,
    //            (*sp)->left,
    //            (*sp)->right->spv_data);

    //        ++sp;
    //    }

    //    if(filesystem::SaveMemoryToFile(filename,mos.GetData(),mos.Tell())==mos.Tell())
    //    {
    //        info_output->colorWriteln("green",OS_TEXT("Save material file \"<b>")+filename+OS_TEXT("</b>\" OK! total ")+OSString::valueOf(mos.Tell())+OS_TEXT(" bytes."));
    //        return(true);
    //    }
    //    else
    //    {
    //        info_output->colorWriteln("red",OS_TEXT("Save material file \"")+filename+OS_TEXT("\" failed!"));
    //        return(false);
    //    }
    //}
}//namespace shader_lib
