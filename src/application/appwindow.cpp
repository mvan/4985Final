#include "appwindow.h"
#include "ui_appwindow.h"
#include <QDesktopServices>
#include <QMessageBox>
#include "../network/filetransfer.h"

AppWindow::AppWindow(QWidget *parent) :
        QTabWidget(parent),
        ui(new Ui::AppWindow) {
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    metaInfoResolver = new Phonon::MediaObject(this);

    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    connect(metaInfoResolver, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));
    connect(mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)),
            this, SLOT(sourceChanged(Phonon::MediaSource)));
    connect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));

    mediaObject->setTickInterval(1000);

    Phonon::createPath(mediaObject, audioOutput);

    setupGui();
    fd = new QFileDialog(this, Qt::Dialog);
}

AppWindow::~AppWindow() {
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

    int index = mediaSources.size();
    foreach (QString name, filenames) {
        treename = QStringList((QFileInfo(name).fileName()));
        lstItem = new QTreeWidgetItem(treename, 0);
        ui->myLibrary->addTopLevelItem(lstItem);

        Phonon::MediaSource mediaSource(name);
        mediaSources.append(mediaSource);
    }
    if (!mediaSources.isEmpty()) {
        metaInfoResolver->setCurrentSource(mediaSources.at(index));
    }
}

void AppWindow::playPause() {
    QString str = QString("Play");

    if (QString::compare(ui->play->text(), str) == 0) {
        ui->play->setText("Pause");
        emit playFile();
    } else {
        ui->play->setText("Play");
        emit pauseFile();
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
    ui->seekSlider->setMediaObject(mediaObject);
    ui->volumeSlider->setAudioOutput(audioOutput);

    connect(ui->addFiles, SIGNAL(clicked()), this, SLOT(addFiles()));
    connect(ui->play, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(ui->txMicroOther, SIGNAL(clicked()), this, SLOT(onOffMicOther()));
    connect(ui->txMicroSelf, SIGNAL(clicked()), this, SLOT(onOffMicSelf()));
    connect(this, SIGNAL(playFile()), mediaObject, SLOT(play()));
    connect(this, SIGNAL(pauseFile()), mediaObject, SLOT(pause()));
    connect(ui->stop, SIGNAL(clicked()), mediaObject, SLOT(stop()));
    connect(ui->myLibrary, SIGNAL(itemSelectionChanged()),
            this, SLOT(fileSelection()));
}

void AppWindow::stateChanged(Phonon::State newState, Phonon::State) {
    switch (newState) {
        case Phonon::ErrorState:
            if (mediaObject->errorType() == Phonon::FatalError) {
                QMessageBox::warning(this, tr("Fatal Error"),
                mediaObject->errorString());
            } else {
                QMessageBox::warning(this, tr("Error"),
                mediaObject->errorString());
            }
            break;
        case Phonon::PlayingState:
            ui->stop->setEnabled(true);
            break;
        case Phonon::StoppedState:
            ui->stop->setEnabled(false);
            break;
        case Phonon::PausedState:
            ui->stop->setEnabled(true);
            break;
        case Phonon::BufferingState:
            break;
        default:
            break;
    }
}

void AppWindow::metaStateChanged(Phonon::State newState, Phonon::State) {
    if (newState == Phonon::ErrorState) {
        QMessageBox::warning(this, tr("Error opening files"),
            metaInfoResolver->errorString());
        while (!mediaSources.isEmpty() &&
               !(mediaSources.takeLast() == metaInfoResolver->currentSource())) {};
        return;
    }

    if (newState != Phonon::StoppedState && newState != Phonon::PausedState)
        return;

    if (metaInfoResolver->currentSource().type()
            == Phonon::MediaSource::Invalid) {
        return;
    }

    if (ui->myLibrary->selectedItems().isEmpty() &&
        ui->otherLibrary->selectedItems().isEmpty()) {
        mediaObject->setCurrentSource(metaInfoResolver->currentSource());
    }

    int index = mediaSources.indexOf(metaInfoResolver->currentSource()) + 1;
    if (mediaSources.size() > index) {
        metaInfoResolver->setCurrentSource(mediaSources.at(index));
    }
}

void AppWindow::sourceChanged(const Phonon::MediaSource &source) {
    ui->myLibrary->setItemSelected(
            ui->myLibrary->topLevelItem(mediaSources.indexOf(source)), TRUE);
}

void AppWindow::fileSelection() {
    bool wasPlaying = mediaObject->state() == Phonon::PlayingState;

    mediaObject->stop();
    mediaObject->clearQueue();

    int index = ui->myLibrary->indexOfTopLevelItem
        (ui->myLibrary->selectedItems().at(0));

    if (index >= mediaSources.size()) {
        return;
    }
    mediaObject->setCurrentSource(mediaSources[index]);

    QString title = QFileInfo(mediaObject->currentSource().fileName())
                    .fileName();
    ui->nowPlaying->clear();
    ui->nowPlaying->setText(tr("Current File: ") + title);

    if (wasPlaying) {
        mediaObject->play();
    }
    else {
        mediaObject->stop();
    }
}

void AppWindow::aboutToFinish() {
    int index = mediaSources.indexOf(mediaObject->currentSource()) + 1;
    if (mediaSources.size() > index) {
        mediaObject->enqueue(mediaSources.at(index));
    }
}
