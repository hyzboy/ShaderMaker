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
            name_label->setText(tr("ShaderMaker"));
            vlayout->addWidget(name_label);
        }

        {
            QLabel *ver_label = new QLabel(name_and_ver);
            ver_label->setText(tr("Version: 1.0"));
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
    about_text->setHtml("<html><body>\n"
                        "<p>ShaderMaker</p>\n"
                        "<p>Copyright (C) 2018-2020 " HGL_OFFICAL_WEB ".</p>"
                        "</body></html>");
    tab->addTab(about_text,tr("About"));

    QTextBrowser *lib_text=new QTextBrowser();
    lib_text->setOpenExternalLinks(true);
    lib_text->setFrameShape(QFrame::NoFrame);
    lib_text->setHtml(  "<html><body>\n"
                        "<ul>"
                        "<li> Qt " QT_VERSION_STR
                        "</li>\n"
                        "<li> GLSLCompiler v1.02"
                        "</li>\n"
                        "<li> CMGameEngine 2020"
                        "</li>\n"
                        "</ul></body></html>");
    tab->addTab(lib_text,tr("Libraries"));

#if HGL_OS == HGL_OS_Windows
    const uint major = _MSC_VER / 100;
    const uint minor = _MSC_VER % 100;
    const uint patch = _MSC_FULL_VER % 100000;

    const QString msvc_ver = QString::number(major) + "." +QString::number(minor) + "." + QString::number(patch);
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
    resize(480,300);

    QVBoxLayout* aboutLayout = new QVBoxLayout(this);

    aboutLayout->addWidget(CreateIconAndNameLayout(this),0,Qt::AlignVCenter|Qt::AlignLeft);

    aboutLayout->addWidget(CreateTabpage(this));

    QPushButton* close_button = new QPushButton(this);

    close_button->setText(tr("Close"));
    connect(close_button,&QPushButton::clicked,this,&DLGAbout::OnClose);

    aboutLayout->addWidget(close_button, 0, Qt::AlignRight);

    setModal(true);
}

void DLGAbout::OnClose()
{
    close();
}
