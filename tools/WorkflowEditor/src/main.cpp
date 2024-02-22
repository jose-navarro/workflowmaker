/** \file main.cpp
\brief Application's entry point.
*/

#include "mainwindow.hpp"

#include <QApplication>

/// \brief Application's entry point.
/**
  \param argv Total number of command line parameters.
  \param args The values of the command line parameters.
  \return The value returned by the QApplication controlling the Qt
          event loop.
 */

int
main
(int   argv,
 char* args[])
{
  {
    Q_INIT_RESOURCE(WorkflowEditor);

    #ifdef Q_OS_ANDROID
      QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    #endif


    QApplication app(argv, args);
    MainWindow mainWindow;
    //mainWindow.setGeometry(100, 100, 800, 500);
    mainWindow.showMaximized();

    return app.exec();
  }
}
