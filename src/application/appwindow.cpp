#include "appwindow.h"
#include "ui_appwindow.h"
#include "../network/filetransfer.h"

AppWindow::AppWindow(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::AppWindow)
{
    ui->setupUi(this);
}

AppWindow::~AppWindow()
{
    delete ui;
}

void AppWindow::on_stream_clicked()
{
    HANDLE file;
    file = CreateFile(TEXT("C:\\Users\\Admin\\Desktop\\test.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                      NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    FileReadThread *thread = new FileReadThread(file);
    thread->start();
}
