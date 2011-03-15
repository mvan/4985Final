#include "appwindow.h"
#include "ui_appwindow.h"

AppWindow::AppWindow(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::AppWindow)
{
    fd = new QFileDialog(this, Qt::Dialog);
    ui->setupUi(this);
    ui->addFiles->setEnabled(TRUE);
    connect(ui->addFiles, SIGNAL(clicked()), this, SLOT(addFiles()));
}

AppWindow::~AppWindow()
{
    delete ui;
}

void AppWindow::addFiles() {
    fd->setFileMode(QFileDialog::ExistingFiles);
    fd->setNameFilter(tr("Audio (*.wav)"));
    if (fd->exec()) {
        filenames = fd->selectedFiles();
    }
    lstItem = new QTreeWidgetItem(filenames, 0);
    ui->library->addTopLevelItem(lstItem);
}
