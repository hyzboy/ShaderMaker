#pragma once
#include<hgl/type/StringList.h>
#include"RenderPlatformConfig.h"

using namespace hgl;

class ShaderMaker
{
    RenderPlatformConfig rp_cfg;

    UTF8StringList shader_source;

protected:

    void MakeCustomCode();

public:

    ShaderMaker(const RenderPlatformConfig &);
    ~ShaderMaker();
    
    bool Save(const OSString &filename);
};//class ShaderMaker
