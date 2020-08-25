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

        class CPPElementCreater:public ElementCreater
        {
            SubModule *sm;

        public:

            CPPElementCreater():ElementCreater(u8"cpp"){sm=nullptr;}
            virtual ~CPPElementCreater()=default;

            void Set(SubModule *_sm){sm=_sm;}
            void End() override {sm=nullptr;}

            void CharData(const u8char *str,const int str_length) override
            {
                if(!sm)return;

                sm->cpp_files.Add(UTF8String((u8char *)str,str_length));
            }
        };//class CPPElementCreater:public ElementCreater
        
        class UBOCodeElementCreater:public ElementCreater
        {
            UBO *ubo;

        public:

            UBOCodeElementCreater():ElementCreater(u8"code"){ubo=nullptr;}
            virtual ~UBOCodeElementCreater()=default;
            
            void SetUBO(UBO *su){ubo=su;}
            void End() override 
            {
                const int lines=ubo->codes.GetCount();

                std::cout<<"uniform"<<std::endl<<"{"<<std::endl;

                for(int i=0;i<lines;i++)
                    std::cout<<"    "<<ubo->codes.GetString(i).c_str()<<std::endl;

                std::cout<<"};"<<std::endl;

                ubo=nullptr;
            }

            void CharData(const u8char *str,const int str_length) override
            {
                if(!str||!*str||str_length<=0)return;
                if(str_length==1)
                    if(hgl::isspace(*str))return;

                if(ubo)
                {
                    UTF8String u8str;
                    LoadStringFromText(u8str,(void *)str,str_length,UTF8CharSet);

                    u8str.Trim();

                    if(!u8str.IsEmpty())
                        ubo->codes.Add(u8str);
                }
            }
        };//class UBOCodeElementCreater:public ElementCreater

        class UBOElementCreater:public ElementCreater
        {
            CPPElementCreater cpp_ec;
            UBOCodeElementCreater code_ec;

            Module *shader_module;
            UBO *ubo;

        public:

            UBOElementCreater(Module *sm):ElementCreater(u8"ubo")
            {
                shader_module=sm;
                ubo=nullptr;

                Registry(&cpp_ec);
                Registry(&code_ec);
            }

            virtual ~UBOElementCreater()=default;

            bool Start() override
            {
                ubo=new UBO;

                cpp_ec.Set(ubo);
                code_ec.SetUBO(ubo);

                return(true);
            }

            void Attr(const u8char *flag,const u8char *info) override
            {
                if(!ubo)return;

                if(strcmp(flag,u8"name")==0)ubo->name=info;else
                if(strcmp(flag,u8"value")==0)ubo->value_name=info;                
            }

            void End() override
            {
                if(ubo)
                {
                    shader_module->ubo_list.Add(ubo);
                    ubo=nullptr;
                }
            }
        };//class UBOElementCreater:public ElementCreater

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
        
        class RootElementCreater:public ElementCreater
        {        
            VertexAttrribElementCreater *va_ec;
            UBOElementCreater *ubo_ec;

            Module *shader_module;

        public:

            RootElementCreater(Module *sm):ElementCreater(u8"root")
            {
                shader_module=sm;

                va_ec=new VertexAttrribElementCreater(sm);
                ubo_ec=new UBOElementCreater(sm);

                Registry(va_ec);
                Registry(ubo_ec);
            }

            virtual ~RootElementCreater()
            {
                SAFE_CLEAR(ubo_ec);
                SAFE_CLEAR(va_ec);
            }
        };//class RootElementCreater:public ElementCreater

        class ShaderElementCreater:public ElementCreater
        {
            Module *shader_module;

            RootElementCreater *root_ec;

        public:

            ShaderElementCreater(Module *sm):ElementCreater(u8"shader")
            {
                shader_module=sm;
                root_ec=new RootElementCreater(sm);

                Registry(root_ec);
            }

            ~ShaderElementCreater()
            {
                SAFE_CLEAR(root_ec);
            }
        };//class ShaderElementCreater:public ElementCreater
    }//namespace

    Module *LoadXMLShader(const XMLShaderModuleType &type_hint,const OSString &filename)
    {
        Module *sm=new Module;

        sm->filename=filename;

        ShaderElementCreater shader_ec(sm);
        ElementParseCreater epc(&shader_ec);
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
