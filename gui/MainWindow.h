#include<QMainWindow>
#include<QTreeWidgetItem>
#include<QListWidget>
#include<QTextEdit>
#include<QSyntaxHighlighter>
#include<hgl/type/StringList.h>

using namespace hgl;

class MainWindow:public QMainWindow
{
    Q_OBJECT

private:    //menu

    void OnHomepage();
    void OnAbout();

    void OnConfig();
    
    void OnCreateVertexShader();
    void OnCreateTessControlShader();
    void OnCreateTessEvalShader();
    void OnCreateGeometryShader();
    void OnCreateFragmentShader();
    
    void OnCreateMaterial();

    void InitMenu();

public:

    using QMainWindow::QMainWindow;
    ~MainWindow()=default;

    void Init();
};//class MainWindow:public QMainWindow
