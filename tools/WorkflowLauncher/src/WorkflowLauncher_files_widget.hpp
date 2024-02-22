/** \file WorkflowLauncher_files_widget.hpp
\brief Files tab in the main application's widget.
*/

#ifndef WORKFLOWLAUNCHER_FILES_WIDGET_HPP
#define WORKFLOWLAUNCHER_FILES_WIDGET_HPP

#include <QColor>
#include <QScrollArea>
#include <QString>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "launcher_structures.hpp"
#include "single_file_widget.hpp"

#include <vector>
#include <string>

using namespace std;

/// \brief Files tab in the main application's widget.
/**
 This class provides a widget to see / input the values of the files
 involved in a launcher file.
*/

class WorkflowLauncher_files_widget : public QWidget
{
    Q_OBJECT

  public:

    /// \brief Check that all the required data items have
    ///        been input in the widget's interface.
    /**
      \return True if all fields have been set, false
              otherwise.
     */

    bool              check_completeness             (void);

    /// \brief Return the list of errors detected up to the moment.

    QVector<QString>  error_list                     (void);

    /// \brief Return the current list of files.

    vector<WLFile>    get_current_file_list          (void);

    /// \brief Reset the list of errors, so it is emptied.

    void              reset_error_list               (void);

    /// \brief Set the files that will be handled by the widget and
    ///        shown on the screen.
    /**
      \param files The set of files to handle.

      Note that no checks on the completeness or correction of
      the options to copy to the screen are made. It is the responsibility
      of the caller module to guarantee that.
    */

    void              set_values                     (vector<WLFile>& files);

    /// \brief Constructor.
    /**
      \param parent This widget's parent.
     */

                      WorkflowLauncher_files_widget  (QWidget* parent = nullptr);

    /// Destructor.

                      ~WorkflowLauncher_files_widget (void);


  protected:

    /// \brief Widget including the different file sub-widgets.

    QVBoxLayout*                 container_layout_;

    /// \brief List of errors detected when checking the completeness
    ///        or validity of the list of repositories.

    QVector<QString>             error_list_;

    /// \brief The list of files to handle.

    vector<WLFile>               files_;

    /// \brief The list of repository sub-widgets we're handling.

    vector<single_file_widget*>  wfiles_;

};

#endif // WORKFLOWLAUNCHER_FILES_WIDGET_HPP
