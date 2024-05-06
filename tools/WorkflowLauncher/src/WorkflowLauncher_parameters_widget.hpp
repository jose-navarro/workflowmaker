/** \file WorkflowLauncher_parameters_widget.hpp
\brief Parameters tab in the main application's widget.
*/

#ifndef WORKFLOWLAUNCHER_PARAMETERS_WIDGET_HPP
#define WORKFLOWLAUNCHER_PARAMETERS_WIDGET_HPP

#include <QColor>
#include <QScrollArea>
#include <QString>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "launcher_structures.hpp"
#include "single_parameter_widget.hpp"

#include <vector>
#include <string>

using namespace std;

/// \brief Parameters tab in the main application's widget (application GUI).
/**
 This class provides a widget to see / input the values of the parameters
 involved in a launcher file.
*/

class WorkflowLauncher_parameters_widget : public QWidget
{
    Q_OBJECT

  public:

    /// \brief Check that all the required data items have
    ///        been input in the widget's interface.
    /**
      \return True if all fields have been set, false
              otherwise.
     */

    bool                check_completeness         (void);

    /// \brief Return the list of errors detected up to the moment.

    QVector<QString>    error_list                 (void);

    /// \brief Return the current list of parameters.

    vector<WLParameter> get_current_parameter_list (void);

    /// \brief Reset the list of errors, so it is emptied.

    void                reset_error_list           (void);

    /// \brief Set the parameters that will be handled by the widget and
    ///        shown on the screen.
    /**
      \param params The set of parameter to handle.

      Note that no checks on the completeness or correction of
      the options to copy to the screen are made. It is the responsibility
      of the caller module to guarantee that.
    */

    void                set_values                 (vector<WLParameter>& params);

    /// \brief Constructor.
    /**
      \param parent This widget's parent.
     */

                     WorkflowLauncher_parameters_widget  (QWidget* parent = nullptr);

    /// Destructor.

                     ~WorkflowLauncher_parameters_widget (void);


  protected:

    /// \brief Widget including the different parameter sub-widgets.

    QVBoxLayout*                     container_layout_;

    /// \brief List of errors detected when checking the completeness
    ///        or validity of the list of parameters.

    QVector<QString>                 error_list_;

    /// \brief The list of parameters to handle.

    vector<WLParameter>              params_;

    /// \brief The list of parameters sub-widgets we're handling.

    vector<single_parameter_widget*> wparams_;

};

#endif // WORKFLOWLAUNCHER_PARAMETERS_WIDGET_HPP
