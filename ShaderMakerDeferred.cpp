#include"ShaderMaker.h"
#include"ShaderConfigParse.h"

class SM_Deferred:public ShaderMaker
{
public:

    using ShaderMaker::ShaderMaker;
    ~SM_Deferred()
    {
    }
    
};//class SM_Deferred:public ShaderMaker

ShaderMaker *CreateDeferredShaderMaker(RenderPlatformConfig &rpc)
{
    ShaderConfigParse *shader_config = LoadShaderConfig(rpc.config_filename);

    if (!shader_config)
        return(nullptr);

    return(new SM_Deferred(rpc,));
}