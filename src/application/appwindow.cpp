#include "appwindow.h"
#include "ui_appwindow.h"
#include <QDesktopServices>

AppWindow::AppWindow(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::AppWindow)
{
    setupGui();
    fd = new QFileDialog(this, Qt::Dialog);
}

AppWindow::~AppWindow()
{
    delete ui;
}

void AppWindow::addFiles() {
    fd->setFileMode(QFileDialog::ExistingFiles);
    filenames = fd->getOpenFileNames(this,tr("Select a Music File"),
                                     QDesktopServices::storageLocation
                                     (QDesktopServices::MusicLocation),
                                     tr("Audio (*.wav *.mp3)"));

    if (filenames.isEmpty()) {
        return;
    }

    foreach (QString name, filenames) {
        treename = QStringList((QFileInfo(name).fileName()));
        lstItem = new QTreeWidgetItem(treename, 0);
        ui->myLibrary->addTopLevelItem(lstItem);
    }
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

void AppWindow::setupGui() {
    ui->setupUi(this);
    ui->addFiles->setEnabled(TRUE);
    ui->play->setEnabled(TRUE);

    connect(ui->addFiles, SIGNAL(clicked()), this, SLOT(addFiles()));
    connect(ui->play, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(ui->txMicroOther, SIGNAL(clicked()), this, SLOT(onOffMicOther()));
    connect(ui->txMicroSelf, SIGNAL(clicked()), this, SLOT(onOffMicSelf()));
}
