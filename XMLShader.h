#pragma once

#include<hgl/type/StringList.h>

namespace glsl_compiler
{
    struct SPVData;
};

namespace shader_lib
{
    using namespace hgl;

    struct Uniform
    {
        UTF8String type_name;
        UTF8String value_name;

        uint binding;
    };

    struct GeometryAttribute
    {
        UTF8String in;
        UTF8String out;
        uint max_vertices;
    };

    struct XMLShader
    {
        UTF8StringList in;
        UTF8StringList out;
        UTF8StringList raw;
        UTF8StringList struct_block;
        ObjectList<Uniform> uniforms;

        GeometryAttribute geom;

        UTF8StringList main;

    public:

        uint32_t shader_type;        
        UTF8String shader_source;
        glsl_compiler::SPVData *spv_data;

    public:

        XMLShader()
        {
            shader_type=0;
            spv_data=nullptr;
        }

        ~XMLShader();

        bool SetShaderSource(const UTF8StringList &);
        
        bool SaveToGLSL(const OSString &filename);
        bool SaveToSPV(const OSString &filename);
        bool SaveToShader(const OSString &filename);
    };//struct XMLShader
}//namespace shader_lib
