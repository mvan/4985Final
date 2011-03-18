#include "appwindow.h"
#include "ui_appwindow.h"
#include "../network/filetransfer.h"

AppWindow::AppWindow(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::AppWindow)
{
    ui->setupUi(this);
    ui->addFiles->setEnabled(TRUE);
    ui->play->setEnabled(TRUE);

    fd = new QFileDialog(this, Qt::Dialog);

    connect(ui->addFiles, SIGNAL(clicked()), this, SLOT(addFiles()));
    connect(ui->play, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(ui->txMicroOther, SIGNAL(clicked()), this, SLOT(onOffMicOther()));
    connect(ui->txMicroSelf, SIGNAL(clicked()), this, SLOT(onOffMicSelf()));
}

AppWindow::~AppWindow()
{
    delete ui;
}

void AppWindow::addFiles() {
    fd->setFileMode(QFileDialog::ExistingFile);
    fd->setNameFilter(tr("Audio (*.wav)"));
    if (fd->exec()) {
        filenames = fd->selectedFiles();
    }
    lstItem = new QTreeWidgetItem(filenames, 0);
    ui->myLibrary->addTopLevelItem(lstItem);
}

void AppWindow::playPause() {
    QString str = QString("Play");

    if (QString::compare(ui->play->text(), str) == 0) {
        ui->play->setText("Pause");
    } else {
        ui->play->setText("Play");
    }
}

void AppWindow::onOffMicOther() {
    QString str = QString("Turn on");

    if (QString::compare(ui->txMicroOther->text(), str) == 0) {
        ui->txMicroOther->setText("Turn off");
    } else {
        ui->txMicroOther->setText("Turn on");
    }
}

void AppWindow::onOffMicSelf() {
    QString str = QString("Turn on");

    if (QString::compare(ui->txMicroSelf->text(), str) == 0) {
        ui->txMicroSelf->setText("Turn off");
    } else {
        ui->txMicroSelf->setText("Turn on");
    }
}
