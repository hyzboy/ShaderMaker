#pragma once

#include<hgl/type/BaseString.h>

using namespace hgl;

struct RenderPlatformConfig
{
    UTF8String  name;
    OSString    config_filename;
    bool        mobile;

public:

    RenderPlatformConfig()
    {
        mobile=false;
    }

    void operator = (const RenderPlatformConfig &rpc)
    {
        name            =rpc.name;
        config_filename =rpc.config_filename;
        mobile          =rpc.mobile;
    }
};//struct RenderPlatformConfig
