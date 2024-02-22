/** \file ToolkitEditor_description_widget.hpp
\brief General description tab in the main application's widget.
*/

#ifndef TOOLKITEDITOR_DESCRIPTION_WIDGET_HPP
#define TOOLKITEDITOR_DESCRIPTION_WIDGET_HPP

#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QVector>

#include "toolkit_structures.hpp"

#include <string>

using namespace std;

/// \brief General description tab in the main application's widget.

class ToolkitEditor_description_widget : public QDialog
{
    Q_OBJECT

  public:

    /// \brief Check that all the required data items have
    ///        been input in the widget's interface.
    /**
      \return True if all fields have been set, false
              otherwise.
     */

    bool             check_completeness               (void);

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

    void             copy_toolkit_values              (toolkit& toolkit);

    /// \brief Return the list of errors detected up to the moment.

    QVector<QString> error_list                       (void);

    /// \brief Reset the list of errors, so it is emptied.

    void             reset_error_list                 (void);

    /// \brief Copy the values in the tookit object to the GUI.
    /**
      \param toolkit The toolkit object whose values will be
             copied to the screen (GUI).

      Note that no checks on the completeness or correction of
      the options to copy to the screen are made. It is the responsibility
      of the caller module to guarantee that.
    */

    void             set_values                       (toolkit& toolkit);

    /**
      \brief Constructor.
      \param parent The parent of this dialog.
     */

    explicit         ToolkitEditor_description_widget (QWidget* parent = nullptr);

    /// \brief Check that the values input by the user in the
    ///        interface of the widget are correct, warning the user
    ///        if not (via message boxes).
    /**
      \return True if the values are correct, false otherwise.
    */

    bool             validate_semantics               (void);

  protected slots:

    /**
      \brief Replace the text in the idenfier field by uppercase letters.
      \param the_text The text currently typed in the identifier field.
     */

    void             id_text_changed                  (const QString& the_text);

  protected:


    /// \brief The text field to type the description of the file type.

    QLineEdit*       description_text_;

    /// \brief List of errors detected when checking the completeness
    ///        or validity of the list of parameters.

    QVector<QString> error_list_;

    /// \brief The text field to type the identifier of the parameter.

    QLineEdit*       id_text_;

};

#endif // TOOLKITEDITOR_DESCRIPTION_WIDGET_HPP
