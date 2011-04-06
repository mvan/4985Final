/**
  * Class: AppWindow
  *
  * Description:
  * This window organizes all the functionality in 4 tabs.
  * The first tab is the audio library page. It features adding files to
  * your library, up to date lists on the other server(s), and the ability to
  * transfer and stream audio files from the server.
  *
  * The second tab features the microphone. It has a simple interface for
  * transfering microphone audio by turning it on or off.
  *
  * The third tab features instant messaging. In this tab you can send messages
  * to and from the other clients.
  *
  * The fourth tab shows connection information for the current connection.
  */

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
#include <QShortcut>
#include "../control/connectioncontrol.h"
#include "../network/chattransferin.h"


namespace Ui {
    class AppWindow;
}

class ConnectionControl;

class AppWindow : public QTabWidget
{
    Q_OBJECT

public:
    explicit AppWindow(ConnectionControl *connectionControl, QWidget *parent = 0);
    virtual ~AppWindow();

    /**
      * Function: addUser(QString username)
      *
      * @param username username of the user we want to add to the user list
      *
      * Description:
      * This function add a new connected user to current user list.
      */
    void addUser(QString username);

public slots:
    /**
      * Slot: addFiles()
      *
      * Description:
      * Connected to ui->addFiles. This will open a "Open File" dialog.
      * It will add the files chosen by the dialog to the "Your Media Library"
      * list.
      */
    void addFiles();

    /**
      * Slot: playPause()
      *
      * Description:
      * Connected to ui->play. This will start or stop playback from the
      * current selected file.
      */
    void playPause();

    /**
      * Slot: onOffMicSelf()
      *
      * Description:
      * Connected to ui->txMicroSelf. This will either start or stop
      * a microphone stream. And change the button to turn on or turn off.
      */
    void onOffMicSelf();

    /**
      * Slot: stateChanged((Phonon::State newState, Phonon::State oldState)
      *
      * @param newState The new state of the mediaObject
      * @param oldState The former state of the mediaObject
      *
      * Description:
      * Connected to mediaObject. This checks for errors in the current state
      * and applies certain ui functionality based on the current state.
      */
    void stateChanged(Phonon::State newState, Phonon::State oldState);

    /**
      * Slot: metaStateChanged(const Phonon::MediaSource &source)
      *
      * @param newState The new state of the mediaObject
      * @param oldState The former state of the mediaObject
      *
      * Description:
      * Connected to metaInfoResolver. Checks for Errors in the current state
      * and updates lists and the source if it needs to be changed.
      */
    void metaStateChanged(Phonon::State newState, Phonon::State oldState);

    /**
      * Slot: sourceChanged(const Phonon::MediaSource &source)
      *
      * @param source the reference to the mediasource defined
      *
      * Description:
      * Connected to mediaObject.
      * Makes sure the element currently playing is also selected in the list.
      */
    void sourceChanged(const Phonon::MediaSource &source);

    /**
      * Slot: fileSelection()
      *
      * Description:
      * Connected to ui->myLibrary. This will update the currently playing and
      * start playing the next selected file if the former selection was
      * already playing.
      */
    void fileSelection();

    /**
      * Slot: aboutToFinish()
      *
      * Description:
      * Connected to mediaObject. This will enqueue the next mediasource
      * available when the current source is about to reach it's end.
      */
    void aboutToFinish();

    /**
      * Slot: addChat(char* packet)
      *
      * @param packet the chat message received
      *
      * Description:
      * Connected to chatInThread_. This will append the received chat
      * message to the chatLog in the chat tab.
      */
    void addChat(char* packet);

    /**
      * Slot: sendChat()
      *
      * Description:
      * Connected to this window. Sends the message specified by the send box.
      */
    void sendChat();

    /**
      * Slot: ftReq()
      *
      * Description:
      * Connected to ui->transfer. This sends a file transfer request for the
      * selected file.
      */
    void ftReq();

    /**
      * Slot: streamReq()
      *
      * Description:
      * Connected to ui->stream. This sends or stops a stream request for the
      * selected file.
      */
    void streamReq();

    /**
      * Slot: updateOtherPlaylist(char* filename);
      *
      * @param filename the name of the file selected
      *
      * Description:
      * Connected to connectionControl_. When a list update is requested this
      * will update the list in ui->otherLibrary.
      */
    void updateOtherPlaylist(char* filename);

    /**
      * Slot: forwardFile()
      *
      * Description:
      * Connected to ui->forward. This will seek the mediaobject forward 3000
      * milliseconds.
      */
    void forwardFile();

    /**
      * Slot: reverseFile()
      *
      * Description:
      * Connected to ui->reverse. This will seek the mediaobject backward 3000
      * milliseconds.
      */
    void reverseFile();

    /**
      * Function: applyStylesheet(QString path)
      *
      * @param path filepath of the stylesheet
      *
      * Description:
      * This function applies the stylesheet specified by path
      * to the current window.
      */
    void applyStyleSheet(QString path);

signals:
    void playFile();
    void pauseFile();
    void chatSignal(char*);
    void requestFT(char* fName);
    void requestStream(char* fName);
    void addAudioFile(QString fName);
    void startMicStream();
    void endMicStream();

private:
    /**
      * Function: setupGui()
      *
      * Description:
      * This function sets up all the buttons and other ui objects, and
      * connects all signals and slots based on this ui.
      */
    void setupGui();

    /**
      * Function: seekFromCurrent(qint64 time);
      *
      * @param time the time to seek in milliseconds
      *
      * Description:
      * This function seeks the current mediaObject by the amount of time
      * specified from the current location.
      */
    void seekFromCurrent(qint64 time);

    /** The user interface for this window. */
    Ui::AppWindow *ui;

    /** The file dialog for opening or saving files. */
    QFileDialog *fd;

    /** The list item for one of the libraries. */
    QTreeWidgetItem *lstItem;

    /** The list of filenames specified by the fileDialog. */
    QStringList filenames;

    /** The filename for a list item. */
    QStringList treename;

    /** The phonon slider for seeking a mediaobject. */
    Phonon::SeekSlider *seekSlider;

    /** The current phonon mediaobject. */
    Phonon::MediaObject *mediaObject;

    /** The current phonon mediaobject which resolves the media info. */
    Phonon::MediaObject *metaInfoResolver;

    /** The audio output object which will output to the speakers. */
    Phonon::AudioOutput *audioOutput;

    /** The slider that changes the volume of the mediaobject. */
    Phonon::VolumeSlider *volumeSlider;

    /** The list of media sources currently available. */
    QList<Phonon::MediaSource> mediaSources;
    QList<QString> currentUsers;

    /** The current connection of this instance. */
    ConnectionControl* connectionControl_;

    /** The thread for sending and receiving chat messages. */
    ChatWriteThread* chatInThread_;

};

#endif // APPWINDOW_H
