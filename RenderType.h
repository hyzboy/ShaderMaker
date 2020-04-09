#pragma once

/**
 * 渲染类型
 */
enum class RenderType
{
    Forward=0,

    DeferredGBuffer,
    DeferredComposition,
    DeferredTransparent,

    PostProcess,

    BEGIN_RANGE =Forward,
    END_RANGE   =PostProcess,
    RANGE_SIZE  =(END_RANGE-BEGIN_RANGE)+1,

    FS  = Forward,

    DS1 = DeferredGBuffer,
    DS2 = DeferredComposition,
    DS3 = DeferredTransparent,

    PP  = PostProcess,
};//enum class RenderType

constexpr char *render_typenames[]={"f","gb","dc","dt","pp"};

/**
 * 帧缓冲区类型
 */
enum class FramebufferType
{
    Color=0,
    Depth,
    Normal,
    Shadowmap,
    Lightmap,
    Mask,
    Velocity,

    BEGIN_RANGE = Color,
    END_RANGE   = Velocity,
    RANGE_SIZE  = (END_RANGE - BEGIN_RANGE) + 1
};//enum class FramebufferType

constexpr char *framebuffer_typenames[]={"color","depth","normal","shadowmap","lightmap","mask","velocity"};
