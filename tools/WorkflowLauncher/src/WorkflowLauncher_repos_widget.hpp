/** \file WorkflowLauncher_repos_widget.hpp
\brief Repositories tab in the main application's widget.
*/

#ifndef WORKFLOWLAUNCHER_REPOS_WIDGET_HPP
#define WORKFLOWLAUNCHER_REPOS_WIDGET_HPP

#include <QColor>
#include <QScrollArea>
#include <QString>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "launcher_structures.hpp"
#include "single_repository_widget.hpp"

#include <vector>
#include <string>

using namespace std;

/// \brief Repositories tab in the main application's widget (application GUI).
/**
 This class provides a widget to see / input the values of the repositories
 involved in a launcher file.
*/

class WorkflowLauncher_repos_widget : public QWidget
{
    Q_OBJECT

  public:

    /// \brief Check that all the required data items have
    ///        been input in the widget's interface.
    /**
      \return True if all fields have been set, false
              otherwise.
     */

    bool                 check_completeness             (void);

    /// \brief Return the list of errors detected up to the moment.

    QVector<QString>     error_list                     (void);

    /// \brief Return the current list of repositories.

    vector<WLRepository> get_current_repo_list          (void);

    /// \brief Reset the list of errors, so it is emptied.

    void                 reset_error_list               (void);

    /// \brief Set the repositories that will be handled by the widget and
    ///        shown on the screen.
    /**
      \param repos The set of repositories to handle.

      Note that no checks on the completeness or correction of
      the options to copy to the screen are made. It is the responsibility
      of the caller module to guarantee that.
    */

    void                 set_values                     (vector<WLRepository>& repos);

    /// \brief Constructor.
    /**
      \param parent This widget's parent.
     */

                         WorkflowLauncher_repos_widget  (QWidget* parent = nullptr);

    /// Destructor.

                         ~WorkflowLauncher_repos_widget (void);


  protected:

    /// \brief Widget including the different repository sub-widgets.

    QVBoxLayout*                      container_layout_;

    /// \brief List of errors detected when checking the completeness
    ///        or validity of the list of repositories.

    QVector<QString>                  error_list_;

    /// \brief The list of repositories to handle.

    vector<WLRepository>              repos_;

    /// \brief The list of repository sub-widgets we're handling.

    vector<single_repository_widget*> wrepos_;

};

#endif // WORKFLOWLAUNCHER_REPOS_WIDGET_HPP
