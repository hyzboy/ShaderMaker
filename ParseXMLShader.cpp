#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/xml/ElementParseCreater.h>
#include<hgl/io/FileInputStream.h>
#include<hgl/type/StringList.h>
#include"ShaderLib.h"

namespace shader_lib
{
    using namespace hgl;

    namespace
    {
        class CodesElementCreater:public xml::ElementCreater
        {
            UTF8StringList *codes;

        public:

            CodesElementCreater(const u8char *str,UTF8StringList *c):xml::ElementCreater(str)
            {
                codes=c;
            }
            virtual ~CodesElementCreater()=default;

            void CharData(const u8char *str,const int str_length) override
            {
                int len=str_length;

                const u8char *trim_str=trim(str,len);

                if(trim_str)
                    codes->Add(UTF8String(trim_str,len));
            }
        };//class CodesElementCreater:public xml::ElementCreater

        class SetsElementCreater:public xml::ElementCreater
        {
            UTF8StringList *codes;

        public:

            SetsElementCreater(const u8char *str,UTF8StringList *c):xml::ElementCreater(str)
            {
                codes=c;
            }
            virtual ~SetsElementCreater()=default;

            void CharData(const u8char *str,const int str_length) override
            {
                int len=str_length;

                const u8char *trim_str=trim(str,len);

                if(trim_str)
                {
                    UTF8String ss=UTF8String(trim_str,len);

                    if(codes->Find(ss)==-1)
                        codes->Add(ss);
                }
            }
        };//class SetsElementCreater:public xml::ElementCreater
        
        class UniformElementCreater:public xml::ElementCreater
        {
            XMLShader *xml_shader;

            uint binding;

        public:

            UniformElementCreater(XMLShader *xs):xml::ElementCreater(u8"uniform"){xml_shader=xs;binding=0;}
            virtual ~UniformElementCreater()=default;

            void Attr(const u8char *flag,const u8char *info) override
            {
                if(hgl::stricmp(flag,"binding")==0)
                    hgl::stou(info,binding);
            }

            void CharData(const u8char *str,const int str_length) override
            {
                int len=str_length;

                const u8char *trim_str=trim(str,len,notcodechar);

                if(len<=0)return;

                int ll=len;
                int rl=len;
                const u8char *left_str=clipleft(trim_str,ll,notcodechar);
                const u8char *right_str=clipright(trim_str,rl,notcodechar);

                if(left_str&&right_str)
                {
                    Uniform *ubo=new Uniform;

                    ubo->type_name=UTF8String(left_str,ll);
                    ubo->value_name=UTF8String(right_str,rl);
                    ubo->binding=binding;

                    if(xml_shader->struct_block.Find(ubo->type_name)==-1)
                        xml_shader->struct_block.Add(ubo->type_name);

                    xml_shader->uniforms.Add(ubo);
                }
            }
        };//class UniformElementCreater:public xml::ElementCreater

        class GeomElementAttrib:public xml::ElementAttribute
        {
            GeometryAttribute *geom;

        public:

            GeomElementAttrib(GeometryAttribute *ga):xml::ElementAttribute("geom")
            {
                geom=ga;

                geom->max_vertices=0;
            }
            
            bool Start() override
            {
                geom->in=this->ToString("in");
                geom->out=this->ToString("out");
                geom->max_vertices=this->ToUInteger("max_vertices");

                return(true);
            }
        };//class GeomElementAttrib:public xml::ElementAttribute

        class XMLShaderRootElementCreater:public xml::ElementCreater
        {
            XMLShader *xml_shader;

            GeomElementAttrib *geom;
            CodesElementCreater *in,*out,*modules,*codes_main;
            SetsElementCreater *raw;
            UniformElementCreater *uniform;

        public:

            XMLShaderRootElementCreater(XMLShader *xs):xml::ElementCreater("root")
            {
                xml_shader=xs;

                in=new CodesElementCreater(u8"in",&(xml_shader->in));
                out=new CodesElementCreater(u8"out",&(xml_shader->out));
                raw=new SetsElementCreater(u8"raw",&(xml_shader->raw));
                modules=new CodesElementCreater(u8"module",&(xml_shader->modules));
                codes_main=new CodesElementCreater(u8"main",&(xml_shader->main));

                uniform=new UniformElementCreater(xml_shader);

                geom=new GeomElementAttrib(&(xml_shader->geom));

                Registry(in);
                Registry(out);
                Registry(raw);
                Registry(modules);
                Registry(codes_main);

                Registry(uniform);
                Registry(geom);
            }

            ~XMLShaderRootElementCreater()
            {
                delete geom;
                delete uniform;

                delete codes_main;
                delete modules;
                delete raw;
                delete out;
                delete in;
            }
        };//class XMLShaderRootElementCreater:public xml::ElementCreater
    }//namespace

    XMLShader *LoadXMLShader(io::InputStream *is,InfoOutput *info_output)
    {
        if(!is)return(nullptr);

        XMLShader *xml_shader=new XMLShader;

        XMLShaderRootElementCreater root_ec(xml_shader);
        xml::ElementParseCreater epc(&root_ec);
        xml::XMLParse xml(&epc);

        xml.Start();

        if(!xml.Parse(is))
        {
            if(info_output)
            {
                int code,row,col;
                xml.GetError(&code,&row,&col);

                (*info_output)<<"[XML Error] "<<xml::GetExpatError(code)<<" in Row:"<<row<<" Col:"<<col<<"\n";
            }

            delete xml_shader;
            return(nullptr);
        }

        return xml_shader;
    }

    XMLShader *LoadXMLShader(const OSString &filename,InfoOutput *info_output)
    {
        if(!filesystem::FileExist(filename))
        {
            (*info_output)<<OS_TEXT("filename <")<<filename.c_str()<<OS_TEXT("> don't exist!\n");
            return(nullptr);
        }

        io::FileInputStream fis;
        
        if(!fis.Open(filename))
        {
            (*info_output)<<OS_TEXT(" Can't open file <")<<filename.c_str()<<OS_TEXT(">!\n");
            return(nullptr);
        }

        XMLShader *xs=LoadXMLShader(&fis,info_output);

        if(!xs)return(nullptr);
        
        xs->SetFilename(filename);

        return xs;
    }
}//namespace shader_lib
