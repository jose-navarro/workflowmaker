/** \file ToolkitEditor_parameters_widget.hpp
\brief Parameters tab in the main application's widget.
*/

#ifndef TOOLKITEDITOR_PARAMETERS_WIDGET_HPP
#define TOOLKITEDITOR_PARAMETERS_WIDGET_HPP

#include <QString>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include <set>
#include <string>

#include "toolkit_structures.hpp"
#include "ToolkitEditor_parameter_list_widget.hpp"

using namespace std;

/// \brief Parameters tab in the main application's widget (application GUI).
/**
 This class provides a widget to add / edit / remove the different parameters
 included in a WorkflowMaker toolkit.
*/

class ToolkitEditor_parameters_widget : public QWidget
{
    Q_OBJECT

  public:

    /// \brief Check that all the required data items have
    ///        been input in the widget's interface.
    /**
      \return True if all fields have been set, false
              otherwise.
     */

    bool                      check_completeness               (void);

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

    void                      copy_toolkit_values        (toolkit& toolkit);

    /// \brief Return the list of errors detected up to the moment.

    QVector<QString>          error_list                 (void);

    /// \brief Return the current list of parameters.

    QList<toolkit_parameter>* get_current_parameter_list (void);

    /// \brief Reset the list of errors, so it is emptied.

    void                      reset_error_list           (void);

    /// \brief Set the pointer to the widget managing the list
    ///        of tasks.
    /**
      \param task_mgr The pointer to the task manager.
     */

    void                      set_task_manager           (ToolkitEditor_tasks_widget* task_mgr);

    /// \brief Copy the values in the tookit object to the GUI.
    /**
      \param toolkit The toolkit object whose values will be
             copied to the screen (GUI).

      Note that no checks on the completeness or correction of
      the options to copy to the screen are made. It is the responsibility
      of the caller module to guarantee that.
    */

    void                      set_values                 (toolkit& toolkit);

    /// \brief Constructor.
    /**
      \param parent This widget's parent.
     */

                     ToolkitEditor_parameters_widget     (QWidget* parent = nullptr);

    /// Destructor.

                     ~ToolkitEditor_parameters_widget    (void);

    /// \brief Check that the values input by the user in the
    ///        interface of the widget are correct, warning the user
    ///        if not (via message boxes).
    /**
      \return True if the values are correct, false otherwise.
    */

    bool             validate_semantics                  (void);

  protected:

    /// \brief List of errors detected when checking the completeness
    ///        or validity of the list of parameters.

    QVector<QString>                     error_list_;

    /// \brief The widget showing the whole list of parameters.

    ToolkitEditor_parameter_list_widget* parameter_list_;
};

#endif // TOOLKITEDITOR_PARAMETERS_WIDGET_HPP
