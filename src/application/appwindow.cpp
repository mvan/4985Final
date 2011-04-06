#include "appwindow.h"
#include "ui_appwindow.h"
#include <QDesktopServices>
#include <QMessageBox>
#include <QFile>
#include <QString>
#include <QBitmap>
#include "../network/filetransfer.h"
#include "../network/externs.h"

AppWindow::AppWindow(ConnectionControl *connectionControl, QWidget *parent) :
        QTabWidget(parent),
        ui(new Ui::AppWindow) {
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    metaInfoResolver = new Phonon::MediaObject(this);
    connectionControl_ = connectionControl;
    chatInThread_ = new ChatWriteThread();
    chatInThread_->start();

    setWindowFlags(Qt::FramelessWindowHint);
    QPixmap pixmap(":/files/appWindowBg.png");
    this->setMask(pixmap.mask());

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
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->quit2, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->quit3, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->quit4, SIGNAL(clicked()), this, SLOT(close()));
    ui->stopStream->hide();

    QShortcut* sendChatShortcut = new QShortcut(QKeySequence("alt+s"), ui->message);
    connect(sendChatShortcut, SIGNAL(activated()), this, SLOT(sendChat()));
}

AppWindow::~AppWindow() {
    chatinBuffer.bufferNotEmpty.wakeAll();
    delete ui;
}

void AppWindow::addFiles() {
    fd->setFileMode(QFileDialog::ExistingFiles);
    filenames = fd->getOpenFileNames(this,tr("Select a Music File"),
                                     QDesktopServices::storageLocation
                                     (QDesktopServices::MusicLocation),
                                     tr("Audio (*.wav)"));

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

        emit addAudioFile(name);
    }
    if (!mediaSources.isEmpty()) {
        metaInfoResolver->setCurrentSource(mediaSources.at(index));
        ui->play->setEnabled(true);
    }
}

void AppWindow::playPause() {
    QString str = QString("Play");

    if (QString::compare(ui->play->toolTip(), str) == 0) {
        ui->play->setIcon(QIcon(":/files/pause.png"));
        ui->play->setToolTip("Pause");
        ui->forward->setEnabled(true);
        ui->reverse->setEnabled(true);
        emit playFile();
    } else {
        ui->play->setIcon(QIcon(":/files/play.png"));
        ui->play->setToolTip("Play");
        emit pauseFile();
    }
}

void AppWindow::onOffMicSelf() {
    QString str = QString("Turn on");

    if (QString::compare(ui->txMicroSelf->text(), str) == 0) {
        ui->txMicroSelf->setText("Turn off");
        ui->txMicroSelf->setIcon(QIcon(":/files/offMicrophone.png"));
        emit(startMicStream());
    } else {
        ui->txMicroSelf->setText("Turn on");
        ui->txMicroSelf->setIcon(QIcon(":/files/yourMicrophone.png"));
        emit(endMicStream());
    }

}

void AppWindow::setupGui() {
    ui->setupUi(this);
    ui->addFiles->setEnabled(TRUE);
    ui->seekSlider->setMediaObject(mediaObject);
    ui->volumeSlider->setAudioOutput(audioOutput);

    //connect chat signals
    connect(chatInThread_, SIGNAL(addChatToDisplay(char*)),
                this, SLOT(addChat(char*)), Qt::QueuedConnection);
    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(sendChat()));
    connect(this, SIGNAL(chatSignal(char*)), connectionControl_,
            SLOT(sendChatPacket(char*)));

    //connect lots of other signals
    connect(ui->stream, SIGNAL(clicked()), this, SLOT(streamReq()));
    connect(this, SIGNAL(requestStream(char*)), connectionControl_, SLOT(requestStream(char*)));
    connect(connectionControl_, SIGNAL(listUpdate(char*)), this,
            SLOT(updateOtherPlaylist(char*)));
    connect(ui->transfer, SIGNAL(clicked()), this, SLOT(ftReq()));
    connect(this, SIGNAL(requestFT(char*)), connectionControl_, SLOT(requestFT(char*)));
    connect(ui->addFiles, SIGNAL(clicked()), this, SLOT(addFiles()));
    connect(this, SIGNAL(addAudioFile(QString)), connectionControl_, SLOT(addAudioFile(QString)));
    connect(ui->play, SIGNAL(clicked()), this, SLOT(playPause()));
    connect(ui->txMicroSelf, SIGNAL(clicked()), this, SLOT(onOffMicSelf()));
    connect(this, SIGNAL(startMicStream()), connectionControl_, SLOT(startMicStream()));
    connect(this, SIGNAL(endMicStream()), connectionControl_, SLOT(endMicStream()));

    connect(this, SIGNAL(playFile()), mediaObject, SLOT(play()));
    connect(this, SIGNAL(pauseFile()), mediaObject, SLOT(pause()));
    connect(ui->stop, SIGNAL(clicked()), mediaObject, SLOT(stop()));
    connect(ui->myLibrary, SIGNAL(itemSelectionChanged()),
            this, SLOT(fileSelection()));
    connect(ui->forward, SIGNAL(clicked()), this, SLOT(forwardFile()));
    connect(ui->reverse, SIGNAL(clicked()), this, SLOT(reverseFile()));
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
            ui->play->setIcon(QIcon(":/files/play.png"));
            ui->play->setToolTip("Play");
            ui->play->setEnabled(true);
            ui->forward->setEnabled(false);
            ui->reverse->setEnabled(false);
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
void AppWindow::addChat(char* packet) {
    QString msg(packet+4);
    QString user(msg.split(":").at(0));
    ui->chatLog->append(msg);
    if(!currentUsers.contains(user)) {
        addUser(user);
    }
}

void AppWindow::sendChat() {
    if(ui->message->toPlainText().length() < 1) {
        return;
    }
    QString tmp;
    char packet[PACKETSIZE], buf[PACKETSIZE];

    ZeroMemory(packet, PACKETSIZE);
    ZeroMemory(buf, PACKETSIZE);
    tmp.append(userName);
    tmp.append(": ");
    tmp.append(ui->message->toPlainText().toAscii().constData());
    memcpy(buf, tmp.toAscii().constData(), tmp.size());
    mkPacket(packet, MSG_CHAT, (unsigned short)tmp.size(), 0, buf);
    emit chatSignal(packet);
    ui->chatLog->append(buf);
    ui->message->clear();
}

void AppWindow::aboutToFinish() {
    int index = mediaSources.indexOf(mediaObject->currentSource()) + 1;
    if (mediaSources.size() > index) {
        mediaObject->enqueue(mediaSources.at(index));
    }
}

void AppWindow::updateOtherPlaylist(char *filename) {
    //Add filename into "other" playlist
    QStringList filenames = QStringList(QString(filename));
    QTreeWidgetItem *item = new QTreeWidgetItem(filenames, 0);
    ui->otherLibrary->addTopLevelItem(item);
}

void AppWindow::seekFromCurrent(qint64 time) {
    mediaObject->seek(mediaObject->currentTime()+time);
}

void AppWindow::forwardFile() {
    this->seekFromCurrent(3000);
}

void AppWindow::reverseFile() {
    this->seekFromCurrent(-3000);
}

void AppWindow::applyStyleSheet(QString path) {
    QFile f(path);
    f.open(QIODevice::ReadOnly);
    this->setStyleSheet(QString(f.readAll()));
    f.close();
}

void AppWindow::ftReq() {
    QString q(this->ui->otherLibrary->currentIndex().data().toString());
    emit(requestFT(q.toAscii().data()));
}

void AppWindow::streamReq() {
    QString q(this->ui->otherLibrary->currentIndex().data().toString());
    emit(requestStream(q.toAscii().data()));
}

void AppWindow::addUser(QString username) {
    currentUsers.push_back(username);
    ui->userList->clear();
    foreach(QString user, currentUsers) {
        ui->userList->append(user);
    }
}
