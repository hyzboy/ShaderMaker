void MakeGBufferAttribute(UTF8StringList &shader)
{
    ShaderAttribute *sa= shader_config->attr_list.GetData();
    UTF8String type_name;
    
    for(int i=0;i< shader_config->attr_list.GetCount();i++)
    {
        type_name=MakeValueName(sa->format);

        shader.Add(U8_TEXT("    ")+type_name+U8_TEXT(" ")+UTF8String(sa->name)+U8_TEXT(";"));

        ++sa;
    }
}

void MakeGBufferFragmentShader(const OSString &output_filename)
{
    UTF8StringList shader;

    MakeShaderHeader(shader);
    shader_lib::AddTo(shader,si_push_constant);
    shader_lib::AddTo(shader,si_ubo_world_matrix);
    
    shader.Add("");

    ShaderAttribute *sa= shader_config->gbuffer_list.GetData();
    UTF8String type_name;

    for(int i=0;i< shader_config->gbuffer_list.GetCount();i++)
    {
        type_name=MakeValueName(sa->format);

        shader.Add(U8_TEXT("layout(location=")+UTF8String(i)+U8_TEXT(") out ")+type_name+U8_TEXT(" ")+UTF8String(sa->name)+U8_TEXT(";"));

        ++sa;
    }

    shader.Add(R"(
void main()
{)");

    MakeGBufferAttribute(shader);

    MakeCustomCode(shader);

    shader.Add(shader_config->attribute_to_gbuffer);

    shader.Add(UTF8String("}"));

    OutputStringList(shader);
    
    SaveShaderToFile(output_filename+OS_TEXT("_gbuffer.frag"),shader);
}

void MakeCompositionVertexShader(const OSString &output_filename)
{
    UTF8StringList shader;

    MakeShaderHeader(shader);
    shader_lib::AddTo(shader,si_fs_rectangle_vs);

    OutputStringList(shader);

    SaveShaderToFile(output_filename+OS_TEXT("_composition.vert"),shader);
}

void MakeCompositionFragmentShader(const OSString &output_filename)
{
    UTF8StringList shader;

    MakeShaderHeader(shader);
    shader_lib::AddTo(shader,si_push_constant);
    shader_lib::AddTo(shader,si_ubo_world_matrix);

    shader.Add("");

    ShaderAttribute *sa= shader_config->gbuffer_list.GetData();
    UTF8String type_name;

    //if(use_subpass)
    //    shader.Add(U8_TEXT("layout(input_attachment_index=0,binding=0) uniform subpassInput rb_depth;"));
    //else
        shader.Add(U8_TEXT("layout(binding=0) uniform sampler2D rb_depth;"));

    for(int i=0;i< shader_config->gbuffer_list.GetCount();i++)
    {
        type_name=MakeValueName(sa->format);
        
        //if(use_subpass)
        //    shader.Add(U8_TEXT("layout(input_attachment_index=")+UTF8String(i+1)+U8_TEXT(",binding=")+UTF8String(i+1)+U8_TEXT(") uniform subpassInput si_")+UTF8String(sa->name)+U8_TEXT(";"));
        //else
            shader.Add(U8_TEXT("layout(binding=")+UTF8String(i+1)+U8_TEXT(") uniform sampler2D ")+UTF8String(sa->name)+U8_TEXT(";"));

        ++sa;
    }

    shader.Add("");
    shader.Add(U8_TEXT("layout(location=0) in vec2 vs_out_position;"));
    shader.Add(U8_TEXT("layout(location=0) out vec4 FragColor;"));

//    shader.Add(R"(
//layout(location=0) out vec4 FragColor;
//
//highp vec3 RetrievePosition()
//{)");
//
//if(use_subpass)
//    shader.Add(R"(    highp vec4 clip    = vec4(gl_FragCoord.xy * world.resolution * 2.0 - 1.0, subpassLoad(rb_depth).x, 1.0);)");
//else
//    shader.Add(R"(    highp vec4 clip    = vec4(gl_FragCoord.xy * world.resolution * 2.0 - 1.0, texelFetch(rb_depth,gl_FragCoord.xy,0).x,1.0);)");
//
//shader.Add(R"(
//    highp vec4 world_w = world.inverse_projection * clip;
//    highp vec3 pos     = world_w.xyz / world_w.w;
//
//    return pos;
//}
//
//void main()
//{
//    highp vec3 Position=RetrievePosition();
//)");

    shader.Add(R"(
void main()
{)");

    MakeGBufferAttribute(shader);
    
    shader.Add("");
    shader.Add(shader_config->gbuffer_to_attribute);
    shader.Add("");

    MakeCustomCode(shader);

    shader.Add(U8_TEXT("    FragColor="));
    shader.Add(U8_TEXT("}"));

    OutputStringList(shader);

    SaveShaderToFile(output_filename+OS_TEXT("_composition.frag"),shader);
}