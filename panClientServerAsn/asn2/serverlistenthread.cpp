/** ---------------------------------------------------------------------------
  * Source file:    serverlistenthread.cpp - A helper thread. The server will
  *                     create this thread to listen for a client connection.
  *
  * Program:        Winsock2 client - server
  *
  * Functions:      ServerListenThread(QObject *parent = 0);
  *                 ~ServerListenThread();
  *                 void run();
  *                 SOCKET getServerSocket();
  *                 void setServerSocket(SOCKET serverSocket);
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
    --------------------------------------------------------------------------- */
#include "serverlistenthread.h"
#include "serverwindow.h"

/** ---------------------------------------------------------------------------
  * Constructor:    ServerListenThread
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * @param          parent - The parent object
    --------------------------------------------------------------------------- */
ServerListenThread::ServerListenThread(QObject *parent):
        QThread(parent){
}

/** ---------------------------------------------------------------------------
  * Destructor:     ~ServerListenThread
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
    --------------------------------------------------------------------------- */
ServerListenThread::~ServerListenThread(){
}

/** ---------------------------------------------------------------------------
  * Function:       getters and setters for private data members
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
    --------------------------------------------------------------------------- */
SOCKET ServerListenThread::getServerSocket(){
    return serverSocket_;
}

void ServerListenThread::setServerSocket(SOCKET serverSocket){
    serverSocket_ = serverSocket;
}
/** ---------------------------------------------------------------------------
  * ========   End of getters and setters   ========
    --------------------------------------------------------------------------- */

/** ---------------------------------------------------------------------------
  * Function:       run - An entry point of this thread.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  * When a public method start() is called, we will get into this method. This
  * thread listen and accept client connections.
    --------------------------------------------------------------------------- */
void ServerListenThread::run(){
    SOCKET acceptSocket;
    WSAEVENT acceptEvent;

    //Create a WSAEvent event to signal the accept
    if ((acceptEvent = WSACreateEvent()) == WSA_INVALID_EVENT){
        emit errorOccurred(QString("Create event failed"), QString("WSACreateEvent() failed with error ").append(QString::number(WSAGetLastError(), 10)));
        closesocket(serverSocket_);
        WSACleanup();
        emit serverStopped();
        return;
    }

    forever{
        acceptSocket = WSAAccept(serverSocket_, NULL, NULL, NULL, NULL);

        if (WSASetEvent(acceptEvent) == FALSE){
            emit errorOccurred(QString("Set event failed"), QString("WSASetEvent failed with error ").append(QString::number(WSAGetLastError(), 10)));
            closesocket(serverSocket_);
            WSACleanup();
            emit serverStopped();
            return;
        }
        emit clientConnected(acceptSocket);
    }
}
