#include"ShaderModule.h"
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/type/Stack.h>

namespace shader
{
    namespace
    {
        using namespace hgl::xml;

        class AuthorElementCreater:public ElementCreater
        {
        public:

            AuthorElementCreater():ElementCreater(u8"author"){}
            virtual ~AuthorElementCreater()=default;
        };//class AuthorElementCreater:public ElementCreater

        class TimeElementCreater:public ElementCreater
        {
        public:

            TimeElementCreater():ElementCreater(u8"time"){}
            virtual ~TimeElementCreater()=default;
        };//class TimeElementCreater:public ElementCreater

        class VertexAttrribElementCreater:public ElementCreater
        {
            Module *shader_module;

            graph::VertexAttribType vat;
            UTF8String name;
            UTF8String comment;

        public:

            VertexAttrribElementCreater(Module *sm):ElementCreater(u8"vertex_attrib"){shader_module=sm;}
            virtual ~VertexAttrribElementCreater()=default;
            
            void Attr(const u8char *flag,const u8char *info) override
            {
                if(!shader_module)return;

                if(strcmp(flag,u8"type")==0)graph::ParseVertexAttribType(&vat,info);else
                if(strcmp(flag,u8"name")==0)name=(u8char *)info;else
                if(strcmp(flag,u8"comment")==0)comment=(u8char *)info;
            }

            void End() override
            {
                if(!shader_module)return;

                ShaderStage *ss=new ShaderStage;

                ss->type=vat;
                ss->name=name;
                ss->comment=comment;

                shader_module->shader_stage.Add(ss);

                const u8char *type_string=graph::GetVertexAttribName(&(ss->type));

                std::cout<<"shader stage: "<<type_string<<" "<<name.c_str()<<";\t//"<<comment.c_str()<<std::endl;
            }
        };//class VertexAttrribElementCreater:public ElementCreater
        
    }//namespace

    Module *LoadXMLShader(const XMLShaderModuleType &type_hint,const OSString &filename)
    {
        Module *sm=new Module;

        sm->filename=filename;

        RootElementCreater root_ec(sm);
        ElementParseCreater epc(&root_ec);
        XMLParse xp(&epc);

        xp.Start();

        if(!XMLParseFile(&xp,filename))
        {
            delete sm;
            return(nullptr);
        }

        return sm;
    }
}//namespace shader
