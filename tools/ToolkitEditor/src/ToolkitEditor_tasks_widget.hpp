/** \file ToolkitEditor_tasks_widget.hpp
\brief Tasks tab in the main application's widget.
*/

#ifndef TOOLKITEDITOR_TASKS_WIDGET_HPP
#define TOOLKITEDITOR_TASKS_WIDGET_HPP

#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include <map>
#include <set>
#include <string>

#include "toolkit_structures.hpp"
#include "ToolkitEditor_task_list_widget.hpp"

using namespace std;

/// \brief Tasks tab in the main application's widget (application GUI).
/**
 This class provides a widget to add / edit / remove the different tasks
 included in a WorkflowMaker toolkit.
*/

class ToolkitEditor_tasks_widget : public QWidget
{
    Q_OBJECT

  public:

    /// \brief Check that all the required data items have
    ///        been input in the widget's interface.
    /**
      \return True if all fields have been set, false
              otherwise.
     */

    bool             check_completeness          (void);

    /// \brief Copy the values from the fields
    ///        on the screen to an toolkit object.
    /**
      \param toolkit The toolkit object where the data input on
             by the user will be copied.

      Note that no checks on the completeness or correction of
      the values on the screen are made. Please, call first
      check_completeness() and validate_semantics() to be sure
      that no errors are present in the data.
    */

    void             copy_toolkit_values         (toolkit& toolkit);

    /// \brief Return the list of errors detected up to the moment.

    QVector<QString> error_list                  (void);

    /**
      \brief Get the list of task identifiers using a given file type.
      \param filetype The filetype to search for.
      \return The list of tasks using the sought file type.

      The output list may be empty if no task is using the file type sought.
     */

    QStringList      list_of_tasks_for_filetype  (string& filetype);

    /**
      \brief Get the list of task identifiers using a given parameter.
      \param parameter The parameter to search for.
      \return The list of tasks using the sought parameter.

      The output list may be empty if no task is using the parameter sought.
     */

    QStringList      list_of_tasks_for_parameter (string& parameter);

    /// \brief Reset the list of errors, so it is emptied.

    void             reset_error_list            (void);

    /// \brief Set the list of current file types, so these are the only
    ///        ones that may be added to tasks.
    /**
      \param cur_ftypes The list of current file types.
     */

    void             set_current_file_types      (QList<toolkit_file_type>* cur_ftypes);

    /// \brief Set the list of current parameters, so these are the only
    ///        ones that may be added to tasks.
    /**
      \param cur_pars The list of current parameters.
     */

    void             set_current_parameters      (QList<toolkit_parameter>* cur_pars);

    /// \brief Copy the values in the tookit object to the GUI.
    /**
      \param toolkit The toolkit object whose values will be
             copied to the screen (GUI).

      Note that no checks on the completeness or correction of
      the options to copy to the screen are made. It is the responsibility
      of the caller module to guarantee that.
    */

    void             set_values                  (toolkit& toolkit);

    /// \brief Constructor.
    /**
      \param parent This widget's parent.
     */

                     ToolkitEditor_tasks_widget  (QWidget* parent = nullptr);

    /// Destructor.

                     ~ToolkitEditor_tasks_widget (void);

    /// \brief Check that the values input by the user in the
    ///        interface of the widget are correct, warning the user
    ///        if not (via message boxes).
    /**
      \return True if the values are correct, false otherwise.
    */

    bool             validate_semantics          (void);

  protected:

    /// \brief List of errors detected when checking the completeness
    ///        or validity of the list of tasks.

    QVector<QString>                error_list_;

    /// \brief The list of the currently available file types.

    QList<toolkit_file_type>*       filetypes_list_;

    /// \brief Map containing the parameters in the toolkit. To be
    ///        used by the widget with the list of tasks.

    map<string, toolkit_parameter>  parameter_map_;

    /// \brief The widget showing the list of input files.

    ToolkitEditor_task_list_widget* task_list_;
};

#endif // TOOLKITEDITOR_TASKS_WIDGET_HPP
