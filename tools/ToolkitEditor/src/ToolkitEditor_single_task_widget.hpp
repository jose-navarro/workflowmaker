/** \file ToolkitEditor_single_task_widget.cpp
\brief Dialog to enter the data related to a task.
*/

#ifndef TOOLKITEDITOR_SINGLE_TASK_WIDGET_HPP
#define TOOLKITEDITOR_SINGLE_TASK_WIDGET_HPP

#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSet>
#include <QString>
#include <QVBoxLayout>

#include "toolkit_structures.hpp"

#include "ToolkitEditor_file_list_widget.hpp"

#include <map>
#include <set>
#include <string>

using namespace std;

class ToolkitEditor_parameter_list_widget; // To avoid cyclic inclusion.

/// \brief Dialog to enter the data related to a task.

class ToolkitEditor_single_task_widget : public QDialog
{
    Q_OBJECT

  public:

    /**
      \brief Return the values typed for the task.
      \return The task, as typed in the dialog box.
     */

    toolkit_task      task                             (void);

    /**
      \brief Constructor.
      \param task The task whose values will be shown on the
             screen when the dialog is opened.
      \param parameter_map A map with the whole set of available parameters.
      \param filetype_list A list with the whole list of available file types.
      \param parent The parent of this dialog.

      The parameter map is used to find the whole information describing
      a parameter using just its identifier.
     */

    explicit          ToolkitEditor_single_task_widget (toolkit_task&                   task,
                                                        map<string, toolkit_parameter>& parameter_map,
                                                        QList<toolkit_file_type>*       filetype_list,
                                                        QWidget*                        parent = nullptr);

  protected slots:

    /**
      \brief Replace the text in the idenfier field by uppercase letters.
      \param the_text The text currently typed in the identifier field.
     */

    void              id_text_changed                  (const QString& the_text);

    /**
      \brief save_clicked Process the click on the save button.

      Copy the data on screen to the task, so it can be retrieved
      later. Then, close the dialog by means of the accept() method.
     */

    void              save_clicked                     (void);

  protected:

    /**
      \brief Check that all the required elements in the dialog have been
             entered by the user. Warns when it is not so by means of
             warning dialogs.
      \return True if everything has been input, false otherwise.
     */

    bool              validate                         (void);

  protected:

    /// \brief. The push button to cancel (close) the dialog.

    QPushButton*                         cancel_button_;

    /// \brief The text field to type the description of the task.

    QLineEdit*                           description_text_;

    /// \brief The text field to type the name of the executable for the task.

    QLineEdit*                           executable_text_;

    /// \brief List of available file types.

    QList<toolkit_file_type>*            filetype_list_;

    /// \brief The text field to type the identifier of the task.

    QLineEdit*                           id_text_;

    /// \brief The list of input files.

    ToolkitEditor_file_list_widget*      ifiles_;

    /// \brief The list of input files.

    ToolkitEditor_file_list_widget*      ofiles_;

    /// \brief The parameter map. Given a parameter identifier,
    ///        we'll be able to recover the whole parameter data.

    map<string, toolkit_parameter>       parameter_map_;

    /// \brief The widget with the list of parameters.

    ToolkitEditor_parameter_list_widget* params_;

    /// \brief The structure where the data input by the user will be
    ///        saved.

    toolkit_task                         task_;

    /// \brief The push button to save the data input by the user and
    ///        then close the dialog.

    QPushButton*                         save_button_;
};

#endif // TOOLKITEDITOR_SINGLE_TASK_WIDGET_HPP
