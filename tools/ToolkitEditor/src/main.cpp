/** \file main.cpp
\brief Main entry point for the ToolkitEditor application.
*/

#include "ToolkitEditor_widget.hpp"

#include <QApplication>

using namespace std;

/// \brief ToolkitEditor application's main function.
/**
  \param argc Number of command line parameters.
  \param argv The command line parameters.
  \return 0 if the process finished successfully, any other
         value otherwise.

  This is the ToolkitEditor application's main entry point.
 */

int
main
(int  argc,
 char*argv[])
{
  {
    QApplication a(argc, argv);

    // Start the main application's widget.

    ToolkitEditor_widget w;
    w.show();

    return a.exec();
  }
}
