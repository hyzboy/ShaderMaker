#include"glsl2spv.h"
#include"RenderPlatformConfig.h"
#include"ShaderConfigParse.h"
#include"ShaderLib.h"
#include"ShaderMaker.h"

#include<hgl/type/StdString.h>
#include<hgl/type/StringList.h>
#include<hgl/util/cmd/CmdParse.h>
#include<hgl/log/LogInfo.h>
#include<iostream>
#include<iomanip>

using namespace hgl;

ShaderConfigParse *shader_config=nullptr;

int os_main(int argc,os_char **argv)
{
    os_out<<OS_TEXT("ShaderMaker 1.0")<<std::endl;
    os_out<<OS_TEXT("Copyright (C) www.hyzgame.com")<<std::endl;
    os_out<<std::endl;

    if(argc<2)
    {
        os_out<<OS_TEXT("Example: ShaderMaker output_name 1.gbuffer [/mobile]")<<std::endl;
        return(0);
    }

    util::CmdParse cp(argc,argv);

    RenderPlatformConfig rpc;

    rpc.name            =to_u8(argv[1]);
    rpc.config_filename =argv[2];
    rpc.mobile          =(cp.Find(OS_TEXT("/mobile")) > 0);

    ShaderConfigParse *scp=LoadShaderConfig(rpc.config_filename);

    ShaderMaker *sm=new ShaderMaker(rpc);

    MakeGBufferFragmentShader(argv[1]);

    MakeCompositionVertexShader(argv[1]);
    MakeCompositionFragmentShader(argv[1]);

    delete sm;

    return(0);
}
