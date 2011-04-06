/**
  * Class: MainWindow
  *
  * Description:
  * MainWindow is the startup window. This is where you can
  *     - start a connection
  *     - change the port number, ip address, and username values
  *     - open the Manual window
  *     - open the about window
  * Starting a connection will open the main application window.
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
    class MainWindow;
}

class AboutWindow;
class ManWindow;
class AppWindow;
class ConnectionControl;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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

    /**
      * Function: getConnectionControl()
      *
      * @return the ConnectionControl object associated with this program
      *
      * Description:
      * This function returns the ConnectionControl object associated with
      * this instance of the program.
      */
    ConnectionControl* getConnectionControl();

public slots:
    /**
      * Slot: openAbout()
      *
      * Description:
      * Connected to ui->about. This will show the about window created in
      * this window's constructor.
      */
    void openAbout();
    /**
      * Slot: openManual()
      *
      * Description:
      * Connected to ui->manual. This will show the manual window created in
      * this window's constructor.
      */
    void openManual();
    /**
      * Slot: openApp()
      *
      * Description:
      * Connected to ui->connect. This will open a connection based on the
      * parameters filled out in the window, using the ConnectionControl.
      * It will hide this window and show the main application window created
      * in this window's constructor.
      */
    void openApp();

private:
    /** The user interface for this window. */
    Ui::MainWindow *ui;

    /** The about window showing information about the program. */
    AboutWindow *aw;

    /** The user manual window showing how-to's for the program. */
    ManWindow *mw;

    /** The main application window for all functionality of the program. */
    AppWindow *apw;

    /** The ConnectionControl object which organizes the current connection. */
    ConnectionControl *connectionControl_;
};

#endif // MAINWINDOW_H
