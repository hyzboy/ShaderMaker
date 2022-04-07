#include"DLG_About.h"
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QTabWidget>
#include<QTextBrowser>
#include<QPushButton>
#include<QDesktopServices>
#include<QLabel>
#include<QFont>
#include<QUrl>
#include<QPixmap>
#include<hgl/type/DataType.h>
#include<hgl/type/QTString.h>
#include<hgl/util/xml/XMLParse.h>
#include<hgl/util/json/JsonTool.h>

QWidget *DLGAbout::CreateIconAndNameLayout(QWidget *parent)
{
    QWidget *icon_and_name=new QWidget(parent);
    QHBoxLayout *hlayout=new QHBoxLayout(icon_and_name);

    {
        QLabel *iconLabel = new QLabel(icon_and_name);
        iconLabel->setPixmap(QPixmap(":/res/logo64.png"));
        hlayout->addWidget(iconLabel,0, Qt::AlignVCenter|Qt::AlignLeft);
    }

    {
        QWidget *name_and_ver=new QWidget(icon_and_name);
        QVBoxLayout *vlayout=new QVBoxLayout(name_and_ver);

        {
            QLabel *name_label=new QLabel(name_and_ver);
            QFont fnt=name_label->font();
            fnt.setPointSize(fnt.pointSize()*2);
            fnt.setBold(true);
            fnt.setFamily("Tahoma");
            name_label->setFont(fnt);
            name_label->setText(tr("MaterialWriter"));
            vlayout->addWidget(name_label);
        }

        {
            QLabel *ver_label = new QLabel(name_and_ver);
            ver_label->setText(tr("Version: 1.01"));
            vlayout->addWidget(ver_label);
        }

        hlayout->addWidget(name_and_ver,0,Qt::AlignVCenter|Qt::AlignLeft);
    }

    return icon_and_name;
}

QWidget *DLGAbout::CreateTabpage(QWidget *parent)
{
    QTabWidget *tab=new QTabWidget(parent);

    QTextBrowser *about_text=new QTextBrowser();
    about_text->setOpenExternalLinks(true);
    about_text->setFrameShape(QFrame::NoFrame);
    about_text->setHtml("<html><body>"
                        "<p>MaterialWriter</p>"
                        "<p>Copyright (C) 2018-2021</p>"
                        "<p>Offical web: <a href=\"" HGL_OFFICAL_WEB_URL "\">" HGL_OFFICAL_WEB "</a></p>"
                        "</body></html>");
    tab->addTab(about_text,tr("About"));

    const QString expat_ver=ToQString(hgl::xml::GetExpatVersion());
    const QString json_ver=ToQString(hgl::GetJsoncppVersion());

    QTextBrowser *lib_text=new QTextBrowser();
    lib_text->setOpenExternalLinks(true);
    lib_text->setFrameShape(QFrame::NoFrame);
    lib_text->setHtml(  "<html><body>\n"
                        "<ul>"
                            "<li> <a href=\"https://www.qt.io/\">Qt " QT_VERSION_STR "</a></li>\n"
                            "<li> <a href=\"https://github.com/libexpat/libexpat\">libexpat "+expat_ver+"</a></li>\n"
                            "<li> <a href=\"https://github.com/open-source-parsers/jsoncpp\">jsoncpp "+json_ver+"</a></li>\n"
                            "<li> <a href=\"https://github.com/hyzboy/GLSLCompiler\">GLSLCompiler v1.02</a></li>\n"
                            "<ul>"
                                "<li> <a href=\"https://github.com/KhronosGroup/glslang\">glslang</a></li>\n"
                                "<li> <a href=\"https://github.com/KhronosGroup/SPIRV-Cross\">SPIRV-Cross</a></li>\n"
                            "</ul>"
                            "<li> <a href=\"https://github.com/hyzboy/CMCMakeModule\">CMCMakeModule</a></li>\n"
                            "<li> <a href=\"https://github.com/hyzboy/CMCore\">CMCore</a></li>\n"
                            "<li> <a href=\"https://github.com/hyzboy/CMUtil\">CMUtil</a></li>\n"
                            "<li> <a href=\"https://github.com/hyzboy/CMSceneGraph\">CMSceneGraph</a></li>\n"
                            "<li> <a href=\"https://github.com/hyzboy/CMPlatform\">CMPlatform</a></li>\n"
                        "</ul></body></html>");
    tab->addTab(lib_text,tr("Libraries"));

#if HGL_OS == HGL_OS_Windows
    const uint msc_major = _MSC_VER / 100;
    const uint msc_minor = _MSC_VER % 100;
    const uint msc_patch = _MSC_FULL_VER % 100000;

    const QString msvc_ver = QString::number(msc_major) + "." +QString::number(msc_minor) + "." + QString::number(msc_patch);

#endif//
    const QString text_compiler=tr("Compiler");
    const QString text_build_time=tr("Build-Time");
    const QString text_host_system=tr("Host system");
    const QString text_c_compile_flags=tr("C Compile Flags");
    const QString text_cxx_compile_flags=tr("C++ Compile Flags");

    QTextBrowser *build_text=new QTextBrowser();
    build_text->setOpenExternalLinks(true);
    build_text->setFrameShape(QFrame::NoFrame);
    build_text->setHtml("<html><body>\n"
                        "<ul>"
                        "<li> CMAKE: " CMAKE_VERSION "</li>"
#if HGL_OS == HGL_OS_Windows
                        "<li> "+text_compiler+": "+QString::fromWCharArray(HGL_COMPILER_NAME)+" "+ msvc_ver+
#else
                        "<li> "+text_compiler+": " HGL_COMPILER_NAME " (" HGL_LIB_COMPILER_VERSION ")"
#endif//
                        "</li>\n"
                        "<li> "+text_build_time+": " __DATE__ " " __TIME__
                        "</li>\n"
                        "<li> "+text_host_system+": " HGL_HOST_SYSTEM
                        "</li>\n"
                        "<li> "+text_c_compile_flags+": " HGL_COMPILE_C_FLAGS
                        "</li>\n"
                        "<li> "+text_cxx_compile_flags+": " HGL_COMPILE_CXX_FLAGS
                        "</li>\n"
                        "</ul></body></html>");
    tab->addTab(build_text,tr("Build"));

    return tab;
}

DLGAbout::DLGAbout()
{
    setWindowTitle(tr("About"));
    resize(640,480);

    QVBoxLayout* aboutLayout = new QVBoxLayout(this);

    aboutLayout->addWidget(CreateIconAndNameLayout(this),0,Qt::AlignVCenter|Qt::AlignLeft);

    aboutLayout->addWidget(CreateTabpage(this));

    QPushButton* close_button = new QPushButton(this);

    close_button->setText(tr("OK"));
    connect(close_button,&QPushButton::clicked,this,&DLGAbout::OnClose);

    aboutLayout->addWidget(close_button, 0, Qt::AlignRight);

    setModal(true);
}

void DLGAbout::OnClose()
{
    close();
}
