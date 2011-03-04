/** ---------------------------------------------------------------------------
  * Source file:    clientprocessthread.cpp - A helper thread. The server will
  *                     create this thread for each client connected.
  *
  * Program:        Winsock2 client - server
  *
  * Functions:      explicit ClientProcessThread(SOCKET clientSocket, QObject *parent = 0);
  *                 ~ClientProcessThread();
  *                 void run();
  *                 bool openFile(QString filename);
  *                 void getFileName(QString filename);
  *                 static void CALLBACK receivedRoutine(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD inFlags);
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
    --------------------------------------------------------------------------- */
#include "clientprocessthread.h"
#include "serverwindow.h"

using namespace std;
/** ---------------------------------------------------------------------------
  * Initialize static data member
    --------------------------------------------------------------------------- */
ClientProcessThread *myClientProcess = 0;

/** ---------------------------------------------------------------------------
  * Constructor:    ClientProcessThread
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * @param          clientSocket - An initialized client socket
  * @param          parent - The parent object
    --------------------------------------------------------------------------- */
ClientProcessThread::ClientProcessThread(SOCKET clientSocket, QObject *parent) :
        QThread(parent),
        clientSocket_(clientSocket),
        buffsize_(122880), //Maximum of 120KB
        bytesReceived_(0),
        flags_(0){
    ZeroMemory(&overlapped_, sizeof(WSAOVERLAPPED));
    myClientProcess = this;
    data_ = (WSABUF *)malloc(sizeof(WSABUF));
    buff_ = new char[buffsize_];
    file_ = NULL;
    time_ = new QTime();
}

/** ---------------------------------------------------------------------------
  * Destructor:     ~ClientProcessThread
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
    --------------------------------------------------------------------------- */
ClientProcessThread::~ClientProcessThread(){
}


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
  * thread opens a file and sends packets according to user's selected packet
  * size and protocol.
    --------------------------------------------------------------------------- */
void ClientProcessThread::run(){
    ServerWindow *parent = (ServerWindow *)this->parent();

    if(parent->getProtocol() == IPPROTO_TCP){
        QEventLoop loop;
        emit receivedData(&loop);
        QObject::connect(this->parent(), SIGNAL(selectedFile(QString)), this, SLOT(openFile(QString)));
        loop.exec();
    } else if(parent->getProtocol() == IPPROTO_UDP) {
        QEventLoop loop;
        emit receivedData(&loop);
        QObject::connect(this->parent(), SIGNAL(selectedFile(QString)), this, SLOT(getFileName(QString)));
        loop.exec();
    }
    ZeroMemory(buff_, buffsize_);

    time_->start();

    forever {
        ZeroMemory(buff_, buffsize_);
        data_->len = buffsize_;
        data_->buf = buff_;

        if(parent->getProtocol() == IPPROTO_TCP){
            WSARecv(clientSocket_, data_, 1, &bytesReceived_, &flags_, &overlapped_, &receivedRoutine);
        } else if (parent->getProtocol() == IPPROTO_UDP){
            int clientLen = sizeof(clientSockaddr_);
            WSARecvFrom(clientSocket_, data_, 1, &bytesReceived_, &flags_, (struct sockaddr *)&clientSockaddr_, &clientLen, &overlapped_, &receivedRoutine);
        }

        switch(WSAGetLastError()){
        case WSAECONNRESET:
            if(file_ != NULL){
                fclose(file_);
                file_ = NULL;
                emit changeStatus(QString("Closed file"));
            }
            this->terminate();
            closesocket(clientSocket_);
            WSACleanup();
            emit serverStopped();
            return;
            break;
        case WSA_IO_PENDING:
            break;
        case 0:
            if(bytesReceived_ == 0){
                if(file_ != NULL){
                    fclose(file_);
                    file_ = NULL;
                    emit changeStatus(QString("Closed file"));
                }
                this->terminate();
            }
            break;
        default:
            if(file_ != NULL){
                fclose(file_);
                file_ = NULL;
                emit changeStatus(QString("Closed file"));
            }
            this->terminate();
            emit errorOccurred(QString("Receiving failed"), QString("WSARecv() failed with error ").append(QString::number(WSAGetLastError(), 10)));
            closesocket(clientSocket_);
            WSACleanup();
            emit serverStopped();
            return;
        }

        SleepEx(INFINITE, true);
    }
}

/** ---------------------------------------------------------------------------
  * Function:       openFile - A helper function to open a file.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
  * @param          filename -  The name of the file to be opened
  * @return         True if successfully opened, false otherwise
    --------------------------------------------------------------------------- */
bool ClientProcessThread::openFile(QString filename){
    file_ = fopen(filename.toAscii(), "wb");
    if(file_ == NULL){
        emit errorOccurred(QString("Open failed"), QString("Cannot open file for saving"));
    }
    return true;
}

/** ---------------------------------------------------------------------------
  * Function:       getFileName - A file name setter.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  * This method sets our private data member called filename_. So this filename_
  * can be used later.
  *
  * @param          filename -  The name of the file to be opened
    --------------------------------------------------------------------------- */
void ClientProcessThread::getFileName(QString filename){
    fileName_ = filename;
}

/** ---------------------------------------------------------------------------
  * Function:       receivedRoutine - The complete routine for WSARecv().
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  * This method write the packet received into a file.
  *
  * @param          error -  WSAerror
  * @param          bytesTransferred - Number of bytes received from WSARecv()
  * @param          overlapped - An overlapped structure, used internally by the
  *                     system.
  * @param          inFlags - Flags used to modify the behevior of WSARecv()
    --------------------------------------------------------------------------- */
void CALLBACK ClientProcessThread::receivedRoutine(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD inFlags){

    if(error == WSA_OPERATION_ABORTED){
        return;
    }

    ServerWindow *parent = (ServerWindow *)myClientProcess->parent();

    //Open file for UDP
    if(parent->getProtocol() == IPPROTO_UDP && myClientProcess->file_ == NULL){
        myClientProcess->openFile(myClientProcess->fileName_);
    }

    //Close file for UPD
    if(parent->getProtocol() == IPPROTO_UDP && bytesTransferred == 1){
        if(*(myClientProcess->buff_) == 0x04 && myClientProcess->file_ != NULL){
            fclose(myClientProcess->file_);
            myClientProcess->file_ = NULL;
            emit myClientProcess->changeStatus(QString("Closed file"));
            if(parent->getProtocol() == IPPROTO_UDP) {
                QEventLoop loop;
                emit myClientProcess->receivedData(&loop);
                QObject::connect(parent, SIGNAL(selectedFile(QString)), myClientProcess, SLOT(getFileName(QString)));
                loop.exec();
            }
            return;
        }
    }

    if(bytesTransferred > 0){
        size_t bytesWrite = fwrite(myClientProcess->buff_, 1, bytesTransferred, myClientProcess->file_);
        emit myClientProcess->changeStatus(QString("Wrote to file"));
        if(bytesWrite != bytesTransferred){
            emit myClientProcess->errorOccurred(QString("Save failed"), QString("Error occured durring fwrite()"));
        }

        emit myClientProcess->receivedMoreData(bytesTransferred);
    }
}
