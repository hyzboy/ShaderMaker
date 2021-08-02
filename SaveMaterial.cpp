#include"ShaderLib.h"
#include"GLSLCompiler.h"
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/filesystem/FileSystem.h>

namespace shader_lib
{
    void SaveSPV(io::DataOutputStream *global_dos,const ShaderType flag,glsl_compiler::SPVData *spv)
    {
        io::MemoryOutputStream mos;

        glsl_compiler::SaveSPV2Shader(&mos,spv,flag,false);

        global_dos->WriteUint32(mos.Tell());
        global_dos->Write(mos.GetData(),mos.Tell());
    }

    void SaveShaderDescriptorSet(io::DataOutputStream *dos,MaterialStat *stat,InfoOutput *info_output)
    {
    }

    bool SaveMaterial(const OSString &filename,XMLMaterial *xm,InfoOutput *info_output)
    {
        if(!xm)return(false);

        constexpr char MATERIAL_FILE_HEADER[]=u8"Material\x1A";
        constexpr uint MATERIAL_FILE_HEADER_LENGTH=sizeof(MATERIAL_FILE_HEADER)-1;

        io::MemoryOutputStream mos;
        io::LEDataOutputStream dos(&mos);

        dos.Write(MATERIAL_FILE_HEADER,MATERIAL_FILE_HEADER_LENGTH);
        dos.WriteUint8(2);                                                      //version

        dos.WriteUint32(xm->shader_stage_bits);

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
