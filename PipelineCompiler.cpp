#include<hgl/type/String.h>
#include<hgl/graph/vulkan/VKPipelineData.h>
#include<hgl/filesystem/FileSystem.h>
#include<iostream>

VK_NAMESPACE::VKPipelineData *LoadPipeline(const hgl::OSString &filename);

int os_main(int argc,os_char **argv)
{
    std::cout<<"PipelineCompiler v1.0 (offical web: "<<HGL_OFFICAL_WEB<<")"<<std::endl<<std::endl;

    if(argc<2)
    {
        std::cout<<"example: PipelineCompiler [pipeline filename]"<<std::endl<<std::endl;
        return 0;
    }

    const hgl::OSString xml_filename=argv[1];

    os_out<<OS_TEXT("pipeline filename: ")<<xml_filename.c_str()<<std::endl;

    VK_NAMESPACE::VKPipelineData *pd=LoadPipeline(xml_filename);
    
    if(pd)
    {
        const hgl::OSString filename=hgl::filesystem::TrimFileExtName(xml_filename)+OS_TEXT("pipeline");

        os_out<<OS_TEXT("save pipeline file: ")<<filename.c_str()<<std::endl;

        if(VK_NAMESPACE::SaveToFile(filename,pd))
            std::cout<<"save ok!"<<std::endl;
        else
            std::cerr<<"save failed!"<<std::endl;
    }
    else
    {
        std::cerr<<"load pipeline xml file failed!"<<std::endl;
    }

    return 0;
}
