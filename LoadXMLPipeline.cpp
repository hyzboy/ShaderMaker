#include<hgl/type/StringList.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/graph/vulkan/VKPipelineData.h>

namespace shader_lib
{
    using namespace hgl;

    namespace
    {
        const u8char *polygon_mode[]={u8"fill",u8"line",u8"point",u8""};
        const u8char *cull_mode[]={u8"none",u8"front",u8"back",u8"front_and_back",u8""};
        const u8char *front_face[]={u8"ccw",u8"cw",u8""};
        const u8char *stencil_op[]={u8"keep",u8"0",u8"replace",u8"+clamp",u8"-clamp",u8"inv",u8"+wrap",u8"-wrap",u8""};
        const u8char *compare_op[]={u8"never",u8"<",u8"=",u8"<=",u8">",u8"!=",u8">=",u8"always",u8""};
        const u8char *logic_op[]=
        {
            u8"clear",
            u8"and",
            u8"and_reverse",
            u8"copy",
            u8"and_inverted",
            u8"no_op",
            u8"xor",
            u8"or",
            u8"nor",
            u8"equivalent",
            u8"invert",
            u8"or_reverse",
            u8"copy_inverted",
            u8"or_inverted",
            u8"nand",
            u8"set"
            ,u8""
        };

        const u8char *blend_factor[]=
        {
            u8"zero",
            u8"one",
            u8"src_color",
            u8"one_minus_src_color",
            u8"dst_color",
            u8"one_minus_dst_color",
            u8"src_alpha",
            u8"one_minus_src_alpha",
            u8"dst_alpha",
            u8"one_minus_dst_alpha",
            u8"constant_color",
            u8"one_minus_constant_color",
            u8"constant_alpha",
            u8"one_minus_constant_alpha",
            u8"src_alpha_saturate",
            u8"src1_color",
            u8"one_minus_src1_color",
            u8"src1_alpha",
            u8"one_minus_src1_alpha",
            u8""
        };

        const u8char *blend_op[]=
        {
            u8"ADD",
            u8"SUBTRACT",
            u8"REVERSE_SUBTRACT",
            u8"MIN",
            u8"MAX",
            u8"ZERO",
            u8"SRC",
            u8"DST",
            u8"SRC_OVER",
            u8"DST_OVER",
            u8"SRC_IN",
            u8"DST_IN",
            u8"SRC_OUT",
            u8"DST_OUT",
            u8"SRC_ATOP",
            u8"DST_ATOP",
            u8"XOR",
            u8"MULTIPLY",
            u8"SCREEN",
            u8"OVERLAY",
            u8"DARKEN",
            u8"LIGHTEN",
            u8"COLORDODGE",
            u8"COLORBURN",
            u8"HARDLIGHT",
            u8"SOFTLIGHT",
            u8"DIFFERENCE",
            u8"EXCLUSION",
            u8"INVERT",
            u8"INVERT_RGB",
            u8"LINEARDODGE",
            u8"LINEARBURN",
            u8"VIVIDLIGHT",
            u8"LINEARLIGHT",
            u8"PINLIGHT",
            u8"HARDMIX",
            u8"HSL_HUE",
            u8"HSL_SATURATION",
            u8"HSL_COLOR",
            u8"HSL_LUMINOSITY",
            u8"PLUS",
            u8"PLUS_CLAMPED",
            u8"PLUS_CLAMPED_ALPHA",
            u8"PLUS_DARKER",
            u8"MINUS",
            u8"MINUS_CLAMPED",
            u8"CONTRAST",
            u8"INVERT_OVG",
            u8"RED",
            u8"GREEN",
            u8"BLUE",
            u8""
        };

        const VkBlendOp blend_op_list[]=
        {
            VK_BLEND_OP_ADD,
            VK_BLEND_OP_SUBTRACT,
            VK_BLEND_OP_REVERSE_SUBTRACT,
            VK_BLEND_OP_MIN,
            VK_BLEND_OP_MAX,
            VK_BLEND_OP_ZERO_EXT,
            VK_BLEND_OP_SRC_EXT,
            VK_BLEND_OP_DST_EXT,
            VK_BLEND_OP_SRC_OVER_EXT,
            VK_BLEND_OP_DST_OVER_EXT,
            VK_BLEND_OP_SRC_IN_EXT,
            VK_BLEND_OP_DST_IN_EXT,
            VK_BLEND_OP_SRC_OUT_EXT,
            VK_BLEND_OP_DST_OUT_EXT,
            VK_BLEND_OP_SRC_ATOP_EXT,
            VK_BLEND_OP_DST_ATOP_EXT,
            VK_BLEND_OP_XOR_EXT,
            VK_BLEND_OP_MULTIPLY_EXT,
            VK_BLEND_OP_SCREEN_EXT,
            VK_BLEND_OP_OVERLAY_EXT,
            VK_BLEND_OP_DARKEN_EXT,
            VK_BLEND_OP_LIGHTEN_EXT,
            VK_BLEND_OP_COLORDODGE_EXT,
            VK_BLEND_OP_COLORBURN_EXT,
            VK_BLEND_OP_HARDLIGHT_EXT,
            VK_BLEND_OP_SOFTLIGHT_EXT,
            VK_BLEND_OP_DIFFERENCE_EXT,
            VK_BLEND_OP_EXCLUSION_EXT,
            VK_BLEND_OP_INVERT_EXT,
            VK_BLEND_OP_INVERT_RGB_EXT,
            VK_BLEND_OP_LINEARDODGE_EXT,
            VK_BLEND_OP_LINEARBURN_EXT,
            VK_BLEND_OP_VIVIDLIGHT_EXT,
            VK_BLEND_OP_LINEARLIGHT_EXT,
            VK_BLEND_OP_PINLIGHT_EXT,
            VK_BLEND_OP_HARDMIX_EXT,
            VK_BLEND_OP_HSL_HUE_EXT,
            VK_BLEND_OP_HSL_SATURATION_EXT,
            VK_BLEND_OP_HSL_COLOR_EXT,
            VK_BLEND_OP_HSL_LUMINOSITY_EXT,
            VK_BLEND_OP_PLUS_EXT,
            VK_BLEND_OP_PLUS_CLAMPED_EXT,
            VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT,
            VK_BLEND_OP_PLUS_DARKER_EXT,
            VK_BLEND_OP_MINUS_EXT,
            VK_BLEND_OP_MINUS_CLAMPED_EXT,
            VK_BLEND_OP_CONTRAST_EXT,
            VK_BLEND_OP_INVERT_OVG_EXT,
            VK_BLEND_OP_RED_EXT,
            VK_BLEND_OP_GREEN_EXT,
            VK_BLEND_OP_BLUE_EXT
        };

        class TessellationElement:public xml::ElementAttribute
        {
        public:

            VkPipelineTessellationStateCreateInfo tessellation;

        public:

            TessellationElement():xml::ElementAttribute("tessellation")
            {
                tessellation.sType=VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
                tessellation.pNext=nullptr;
                tessellation.flags=0;
                tessellation.patchControlPoints=0;
            }

            bool Start() override
            {
                tessellation.patchControlPoints=ToUInteger("PatchControlPoints");

                return(true);
            }
        };//

        class DepthBiasElement:public xml::ElementAttribute
        {
            VkPipelineRasterizationStateCreateInfo *rasterizer;

        public:

            DepthBiasElement(VkPipelineRasterizationStateCreateInfo *prsci):xml::ElementAttribute("depthbias")
            {
                rasterizer=prsci;
                
                rasterizer->depthBiasEnable = VK_FALSE;
                rasterizer->depthBiasConstantFactor = 0;
                rasterizer->depthBiasClamp = 0;
                rasterizer->depthBiasSlopeFactor = 0;
            }

            bool Start() override
            {
                rasterizer->depthBiasConstantFactor =ToFloat("ConstantFactor");
                rasterizer->depthBiasClamp          =ToFloat("Clamp");
                rasterizer->depthBiasSlopeFactor    =ToFloat("SlopeFactor");

                return(true);
            }
        };//class DepthBiasElement:public xml::ElementAttribute

        class RasterizerElement:public xml::ElementAttribute
        {
            DepthBiasElement *depth_bias;

        public:
        
            VkPipelineRasterizationStateCreateInfo rasterizer;

        public:

            RasterizerElement():xml::ElementAttribute("rasterizer")
            {
                rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizer.pNext = nullptr;
                rasterizer.flags = 0;
                rasterizer.depthClampEnable = VK_FALSE;
                rasterizer.rasterizerDiscardEnable = VK_FALSE;
                rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
                rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
                rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;     //逆时针，和opengl一样
                rasterizer.lineWidth = 1.0f;

                depth_bias=new DepthBiasElement(&rasterizer);
                Registry(depth_bias);
            }

            ~RasterizerElement()
            {
                delete depth_bias;
            }

            bool Start() override
            {

                rasterizer.depthClampEnable         =ToBool("DepthClamp",false);
                rasterizer.rasterizerDiscardEnable  =ToBool("Discard",false);
                rasterizer.polygonMode              =ToEnum<VkPolygonMode>(polygon_mode,"PolygonMode",VK_POLYGON_MODE_FILL);
                rasterizer.cullMode                 =ToEnum<VkCullModeFlags>(cull_mode,"Cull",VK_CULL_MODE_BACK_BIT);
                rasterizer.frontFace                =ToEnum<VkFrontFace>(front_face,"FrontFace",VK_FRONT_FACE_CLOCKWISE);
                
                rasterizer.lineWidth                =ToFloat("LineWidth",1.0);

                return(true);
            }
        };//class RasterizerElement:public xml::ElementAttribute

        class MultisampleElement:public xml::ElementAttribute
        {
        public:

            VkPipelineMultisampleStateCreateInfo multisample;

        public:

            MultisampleElement():xml::ElementAttribute("multisample")
            {
                multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisample.pNext = nullptr;
                multisample.flags = 0;
                multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
                multisample.sampleShadingEnable = VK_FALSE;
                multisample.minSampleShading = 0.0;
                multisample.pSampleMask = nullptr;
                multisample.alphaToCoverageEnable = VK_FALSE;
                multisample.alphaToOneEnable = VK_FALSE;
            }

            bool Start() override
            {
                multisample.rasterizationSamples    =(VkSampleCountFlagBits)ToUInteger("samples",1);
                multisample.sampleShadingEnable     =ToBool("Shading");
                multisample.minSampleShading        =ToFloat("MinShading");
                //multisample.pSampleMask
                multisample.alphaToCoverageEnable   =ToBool("AlphaToCoverage");
                multisample.alphaToOneEnable        =ToBool("AlphaToOne");

                return(true);
            }
        };//class MultisampleElement:public xml::ElementAttribute

        class StencilOpElement:public xml::ElementAttribute
        {
            VkCompareOp default_comp_op;
            VkStencilOpState *state;

        public:

            StencilOpElement(VkStencilOpState *sos,const VkCompareOp cop,const u8char *name):xml::ElementAttribute(name)
            {
                state=sos;

                default_comp_op=cop;

                state->failOp = VK_STENCIL_OP_KEEP;
                state->passOp = VK_STENCIL_OP_KEEP;
                state->depthFailOp = VK_STENCIL_OP_KEEP;
                state->compareOp = cop;
                state->compareMask = 0;
                state->reference = 0;
                state->writeMask = 0;
            }

            bool Start() override
            {
                state->failOp       =ToEnum<VkStencilOp>(stencil_op,"FailOp",VK_STENCIL_OP_KEEP);
                state->passOp       =ToEnum<VkStencilOp>(stencil_op,"PassOp",VK_STENCIL_OP_KEEP);
                state->depthFailOp  =ToEnum<VkStencilOp>(stencil_op,"DepthFailOp",VK_STENCIL_OP_KEEP);
                state->compareOp    =ToEnum<VkCompareOp>(compare_op,"CompareOp",default_comp_op);
                state->compareMask  =ToUInteger("CompareMask");
                state->writeMask    =ToUInteger("writeMask");
                state->reference    =ToUInteger("reference");

                return(true);
            }
        };//class StencilOpElement:public xml::ElementAttribute

        class DepthStencilElement:public xml::ElementAttribute
        {
            StencilOpElement *front,*back;

        public:

            VkPipelineDepthStencilStateCreateInfo depth_stencil;

        public:

            DepthStencilElement():xml::ElementAttribute("DepthStencil")
            {
                depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
                depth_stencil.pNext = nullptr;
                depth_stencil.flags = 0;
                depth_stencil.depthTestEnable = VK_TRUE;
                depth_stencil.depthWriteEnable = VK_TRUE;
                depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
                depth_stencil.depthBoundsTestEnable = VK_FALSE;
                depth_stencil.stencilTestEnable = VK_FALSE;
                depth_stencil.minDepthBounds = 0;
                depth_stencil.maxDepthBounds = 0;

                front=new StencilOpElement(&depth_stencil.front,VK_COMPARE_OP_NEVER,    "front");
                back =new StencilOpElement(&depth_stencil.back, VK_COMPARE_OP_ALWAYS,   "back");

                Registry(front);
                Registry(back);
            }

            ~DepthStencilElement()
            {
                delete back;
                delete front;
            }

            bool Start() override
            {
                depth_stencil.depthTestEnable       =ToBool("DepthTest",true);
                depth_stencil.depthWriteEnable      =ToBool("DepthWrite",true);
                depth_stencil.depthCompareOp        =ToEnum<VkCompareOp>(compare_op,"DepthCompareOp",VK_COMPARE_OP_LESS_OR_EQUAL);
                depth_stencil.depthBoundsTestEnable =ToBool("DepthBoundsTest",false);
                depth_stencil.stencilTestEnable     =ToBool("StencilTest",false);
                depth_stencil.minDepthBounds        =ToFloat("MinDepthBounds",0);
                depth_stencil.maxDepthBounds        =ToFloat("MaxDepthBounds",0);

                return(true);
            }
        };//class DepthStencilElement:public xml::ElementAttribute

        class ColorBlendAttachmentElement:public xml::ElementAttribute
        {            
            List<VkPipelineColorBlendAttachmentState> *att_list;

            VkPipelineColorBlendAttachmentState cba;

        public:

            ColorBlendAttachmentElement(List<VkPipelineColorBlendAttachmentState> *pcbas_list):xml::ElementAttribute("attachment")
            {
                att_list=pcbas_list;

                cba.blendEnable         = VK_FALSE;
                cba.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
                cba.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
                cba.colorBlendOp        = VK_BLEND_OP_ADD;
                cba.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
                cba.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
                cba.alphaBlendOp        = VK_BLEND_OP_ADD;
                cba.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
            }

            bool Start() override
            {
                const UTF8String write_mask=ToString("mask");

                if(!write_mask.IsEmpty())
                {
                    cba.colorWriteMask=0;

                    if(write_mask.FindChars("rR")!=-1)cba.colorWriteMask|=VK_COLOR_COMPONENT_R_BIT;
                    if(write_mask.FindChars("gG")!=-1)cba.colorWriteMask|=VK_COLOR_COMPONENT_G_BIT;
                    if(write_mask.FindChars("bB")!=-1)cba.colorWriteMask|=VK_COLOR_COMPONENT_B_BIT;
                    if(write_mask.FindChars("aA")!=-1)cba.colorWriteMask|=VK_COLOR_COMPONENT_A_BIT;
                }

                cba.blendEnable         =ToBool("blend",false);
                cba.srcColorBlendFactor =ToEnum<VkBlendFactor>(blend_factor,"SrcColor",VK_BLEND_FACTOR_ZERO);
                cba.dstColorBlendFactor =ToEnum<VkBlendFactor>(blend_factor,"DstColor",VK_BLEND_FACTOR_ZERO);
                cba.alphaBlendOp        =blend_op_list[ToSerial(blend_op,"alpha",0)];
                cba.srcAlphaBlendFactor =ToEnum<VkBlendFactor>(blend_factor,"SrcAlpha",VK_BLEND_FACTOR_ZERO);
                cba.dstAlphaBlendFactor =ToEnum<VkBlendFactor>(blend_factor,"DstAlpha",VK_BLEND_FACTOR_ZERO);
                cba.colorBlendOp        =blend_op_list[ToSerial(blend_op,"color",0)];

                att_list->Add(cba);
                return(true);
            }
        };//class ColorBlendAttachmentElement:public xml::ElementAttribute

        class ColorBlendElement:public xml::ElementAttribute
        {
            List<VkPipelineColorBlendAttachmentState> attachments;

            ColorBlendAttachmentElement *cba;

        public:
        
            VkPipelineColorBlendStateCreateInfo color_blend;

        public:

            ColorBlendElement():xml::ElementAttribute("ColorBlend")
            { 
                color_blend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                color_blend.pNext = nullptr;
                color_blend.flags = 0;
                color_blend.logicOpEnable = VK_FALSE;
                color_blend.logicOp = VK_LOGIC_OP_CLEAR;
                color_blend.attachmentCount = 0;
                color_blend.pAttachments = nullptr;
                color_blend.blendConstants[0] = 0.0f;
                color_blend.blendConstants[1] = 0.0f;
                color_blend.blendConstants[2] = 0.0f;
                color_blend.blendConstants[3] = 0.0f;

                cba=new ColorBlendAttachmentElement(&attachments);

                Registry(cba);
            }

            ~ColorBlendElement()
            {
                delete cba;
            }

            bool Start()
            {
                color_blend.logicOpEnable=ToBool("LogicOp");
                color_blend.logicOp=ToEnum<VkLogicOp>(logic_op,"LogicOp",VK_LOGIC_OP_CLEAR);

                return(true);
            }

            void End()
            {
                color_blend.attachmentCount = attachments.GetCount();
                color_blend.pAttachments = attachments.GetData();

                const UTF8String blend_constants=ToString("BlendConstants");

                parse_float_array<u8char,float>(blend_constants.c_str(),color_blend.blendConstants,4);
            }
        };//class ColorBlendElement:public xml::ElementAttribute

        class RootElementCreater:public xml::ElementAttribute
        {
            VkGraphicsPipelineCreateInfo *pipeline_info;

            TessellationElement tess;
            RasterizerElement rasterizer;
            MultisampleElement ms;
            DepthStencilElement ds;
            ColorBlendElement cb;

        public:

            RootElementCreater(VkGraphicsPipelineCreateInfo *pci):ElementAttribute(u8"root")
            {
                pipeline_info=pci;
                pipeline_info->pTessellationState=&(tess.tessellation);
                pipeline_info->pRasterizationState=&(rasterizer.rasterizer);
                pipeline_info->pMultisampleState=&(ms.multisample);
                pipeline_info->pDepthStencilState=&(ds.depth_stencil);
                pipeline_info->pColorBlendState=&(cb.color_blend);
                
                Registry(&tess);
                Registry(&rasterizer);
                Registry(&ms);
                Registry(&ds);
                Registry(&cb);
            }

            virtual ~RootElementCreater()=default;

            bool Start() override
            {
                pipeline_info->stageCount=ToUInteger("stageCount");
                return(true);
            }
        };//class RootElementCreater:public ElementAttribute
    }//namespace

    bool LoadPipeline(const hgl::OSString &filename)
    {
        VkGraphicsPipelineCreateInfo pipeline_info;
        
        hgl_zero(pipeline_info);
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

        RootElementCreater root_ec(&pipeline_info);
        xml::ElementParseCreater epc(&root_ec);
        xml::XMLParse xml(&epc);

        xml.Start();

        os_out<<OS_TEXT("      LoadPipeline config filename: ")<<filename.c_str()<<std::endl;

        bool result=xml::XMLParseFile(&xml,filename);

        return result;
    }
}//namespace shader_lib
