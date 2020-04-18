#pragma once
#include<hgl/type/StringList.h>
#include"ShaderDataType.h"

using namespace hgl;

enum class FramebufferType
{
    Color=0,
    Normal,
    Depth,
    Light,
    Shadow,
    Stencil,
    Accume,

    Data=0xFF
};

struct RenderTargetConfig
{
    FramebufferType type;
    char name[33];
};

struct RenderpassConfig
{
    List<RenderTargetConfig>    rt_cfg;                     ///<这次渲染的目标缓冲区配置
    RenderTargetConfig *        rt_depth=nullptr;           ///<深度渲染目标
};

struct ShaderConfig
{
    ShaderAttributeList attr_list;                          ///<材质属性(如BaseColor,Normal)，用于描述该材质最终需要那些数据
    ShaderAttributeList fb_list;                            ///<Framebuffer属性，用于描述当前着色最终会输出到几个Framebuffer上

    UTF8StringList      attr2fb;                            ///<材质属性到Framebuffer(GBuffer)转换代码

    bool                deferred;                           ///<是否属于延迟渲染

    UTF8StringList      gb2attr;                            ///<GBuffer到材质属性转换代码(延迟渲染独有)
};//struct ShaderConfig

ShaderConfig *LoadShaderConfig(const OSString &);
