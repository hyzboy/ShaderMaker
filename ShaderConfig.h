#pragma once

#include<hgl/type/StringList.h>
#include"ShaderDataType.h"

using namespace hgl;

enum class FramebufferType:size_t
{
    Data=0,

    Color,
    Normal,
    Depth,
    Light,
    Shadow,
    Stencil,
    Accume,

    BEGIN_RANGE =Data,
    END_RANGE   =Accume,
    RANGE_SIZE  =(END_RANGE-BEGIN_RANGE)+1    
};

struct RenderTargetConfig
{
    FramebufferType type;
    bool sRGB;                  ///<是否使用sRGB颜色空间(仅Color，有某些情况下Lightmap也是)
    char name[33];
};

struct RenderpassConfig
{
    List<RenderTargetConfig>    rt_cfg;                     ///<这次渲染的目标缓冲区配置
    RenderTargetConfig *        rt_depth=nullptr;           ///<深度渲染目标
};

struct ShaderConfig
{
    UTF8StringList depend_list;                             ///<依赖模块列表

    ShaderAttributeList attr_list;                          ///<材质属性(如BaseColor,Normal)，用于描述该材质最终需要那些数据
    ShaderAttributeList fb_list;                            ///<Framebuffer属性，用于描述当前着色最终会输出到几个Framebuffer上

    bool fb_exist[FramebufferType::RANGE_SIZE];

    UTF8StringList      attr2fb;                            ///<材质属性到Framebuffer(GBuffer)转换代码

    bool                deferred;                           ///<是否属于延迟渲染

    UTF8StringList      gb2attr;                            ///<GBuffer到材质属性转换代码(延迟渲染独有)

public:

    ShaderConfig()
    {
        hgl_zero(fb_exist);
        deferred=false;
    }

    virtual ~ShaderConfig()=default;

    const bool isFB(enum class FramebufferType ft)const
    {
        if(ft<FramebufferType::BEGIN_RANGE
         ||ft>FramebufferType::END_RANGE)
        return(false);

        return fb_exist[int(ft)-int(FramebufferType::BEGIN_RANGE)];
    }
};//struct ShaderConfig

ShaderConfig *LoadShaderConfig(const OSString &);
