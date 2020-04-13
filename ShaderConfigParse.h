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

struct RenderPipelineConfig
{
};//

struct ShaderConfigParse
{
    ShaderAttributeList attr_list;
    ShaderAttributeList gbuffer_list;

    UTF8StringList attribute_to_gbuffer;
    UTF8StringList gbuffer_to_attribute;
};//

ShaderConfigParse *LoadShaderConfig(const OSString &);
