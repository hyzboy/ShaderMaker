#include"glsl2spv.h"
#include<glslang/Public/ShaderLang.h>
#include<iostream>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/type/Map.h>
#include"VKShaderParse.h"

using namespace hgl;
using namespace hgl::graph;

VK_NAMESPACE_USING

void OutputShaderConfig(ShaderParse *sp)
{
    
}

void OutputVertexShaderConfig(ShaderParse *sp)
{
    Map<UTF8String,VkVertexInputAttributeDescription *> stage_input_locations;

    const SPVResVector &stage_inputs=sp->GetStageInputs();

    uint attr_count=stage_inputs.size();
    
    VkVertexInputBindingDescription *binding_list=new VkVertexInputBindingDescription[attr_count];
    VkVertexInputAttributeDescription *attribute_list=new VkVertexInputAttributeDescription[attr_count];

    VkVertexInputBindingDescription *bind=binding_list;
    VkVertexInputAttributeDescription *attr=attribute_list;

    uint32_t binding_index=0;

    for(const spirv_cross::Resource &si:stage_inputs)
    {
        const VkFormat                  format  =sp->GetFormat(si);             //注意这个格式有可能会解析不出来(比如各种压缩格式)
        const UTF8String &              name    =sp->GetName(si);
        
        bind->binding   =binding_index;                 //binding对应在vkCmdBindVertexBuffer中设置的缓冲区的序列号，所以这个数字必须从0开始，而且紧密排列。
                                                        //在VertexInput类中，buf_list需要严格按照本此binding为序列号排列
        bind->stride    =GetStrideByFormat(format);
        bind->inputRate =VK_VERTEX_INPUT_RATE_VERTEX;

        //binding对应的是第几个数据输入流
        //实际使用一个binding可以绑定多个attrib
        //比如在一个流中传递{pos,color}这样两个数据，就需要两个attrib
        //但在我们的设计中，仅支持一个流传递一个attrib

        attr->binding   =binding_index;
        attr->location  =sp->GetLocation(si);                                        //此值对应shader中的layout(location=
        attr->format    =format;
        attr->offset    =0;

        stage_input_locations.Add(name,attr);

        ++attr;
        ++bind;
        ++binding_index;
    }
}

bool CompileShader(const OSString &filename)
{
    int64 size;

    AutoDeleteArray<char> source=(char *)filesystem::LoadFileToMemory(filename,size,true);

    if(!source)
        return(false);

    const OSString ext_name=filesystem::ClipFileExtName(filename,false);

    SPIRVData spirv;

    if(CompileShaderToSPV(source,ext_name,spirv))
    {
        const OSString spv_filename=filename+OS_TEXT(".spv");
        const uint64 spv_size=spirv.size()*sizeof(uint32);

        if(filesystem::SaveMemoryToFile(spv_filename,spirv.data(),spv_size)!=spv_size)
        {
            std::cerr<<"save to file error!"<<std::endl;
            return(false);
        }
    }

    ShaderParse sp(spirv.data(),spirv.size()*sizeof(uint32));

    if (ext_name.CaseComp(OS_TEXT("vert")) == 0)
        OutputVertexShaderConfig(&sp);

    OutputShaderConfig(&sp);
    return(true);
}

int os_main(int argc,os_char **argv)
{
    os_out<<OS_TEXT("ShaderCompiler 1.0 (glslang version:")<<GLSLANG_MINOR_VERSION<<OS_TEXT(")")<<std::endl;
    os_out<<OS_TEXT("Copyright (C) www.hyzgame.com")<<std::endl;
    os_out<<std::endl;

    if(argc<2)
    {
        os_out<<OS_TEXT("Example: ShaderCompiler [shader filename]")<<std::endl;
        return(0);
    }

    InitShaderCompiler();

    CompileShader(argv[1]);

    CloseShaderCompiler();

    return 0;
}
