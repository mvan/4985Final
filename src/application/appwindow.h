#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QTabWidget>
#include <QFileDialog>
#include <QStringList>
#include <QTreeWidgetItem>
#include <phonon/audiooutput.h>
#include <phonon/seekslider.h>
#include <phonon/mediaobject.h>
#include <phonon/volumeslider.h>
#include <phonon/backendcapabilities.h>
#include <QList>
#include "../control/connectioncontrol.h"
#include "../network/chattransferin.h"


namespace Ui {
    class AppWindow;
}

class ServerControl;
class ConnectionControl;

class AppWindow : public QTabWidget
{
    Q_OBJECT

public:
    explicit AppWindow(ConnectionControl *connectionControl, QWidget *parent = 0);
    ~AppWindow();

    void updateOtherPlaylist(char *filename);

public slots:
    void addFiles();
    void playPause();
    void onOffMicOther();
    void onOffMicSelf();
    void sourceChanged(const Phonon::MediaSource &source);
    void stateChanged(Phonon::State newState, Phonon::State oldState);
    void metaStateChanged(Phonon::State newState, Phonon::State oldState);
    void fileSelection();
    void aboutToFinish();
    void addChat(char* packet);
    void sendChat();

signals:
    void playFile();
    void pauseFile();

private:
    void setupGui();

    Ui::AppWindow *ui;
    QFileDialog *fd;
    QTreeWidgetItem *lstItem;
    QStringList filenames;
    QStringList treename;
    Phonon::SeekSlider *seekSlider;
    Phonon::MediaObject *mediaObject;
    Phonon::MediaObject *metaInfoResolver;
    Phonon::AudioOutput *audioOutput;
    Phonon::VolumeSlider *volumeSlider;
    QList<Phonon::MediaSource> mediaSources;

    ConnectionControl* connectionControl_;
    ServerControl *serverControl_;
    ChatWriteThread* chatInThread_;

};

#endif // APPWINDOW_H
