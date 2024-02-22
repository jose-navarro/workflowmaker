/** \file ToolkitEditor_single_parameter_widget.cpp
\brief Dialog to enter the data related to a parameter.
*/

#ifndef TOOLKITEDITOR_SINGLE_PARAMETER_WIDGET_HPP
#define TOOLKITEDITOR_SINGLE_PARAMETER_WIDGET_HPP

#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSet>
#include <QString>
#include <QVBoxLayout>

#include "toolkit_structures.hpp"

/// \brief Dialog to enter the data related to a parameter.

class ToolkitEditor_single_parameter_widget : public QDialog
{
    Q_OBJECT

  public:

    /**
      \brief Return the values typed for the parameter.
      \return The parameter, as typed in the dialog box.
     */

    toolkit_parameter parameter (void);

    /**
      \brief Constructor.
      \param parameter The parameter whose values will be shown on the
             screen when the dialog is opened.
      \param mode New / edit work modes. To work in "new" mode, set this
             parameter to 0; any other value will set the work mode to "edit".
      \param parent The parent of this dialog.
     */

    explicit          ToolkitEditor_single_parameter_widget (toolkit_parameter& parameter,
                                                             int                mode,
                                                             QWidget*           parent = nullptr);

  protected slots:

    /**
      \brief Replace the text in the idenfier field by uppercase letters.
      \param the_text The text currently typed in the identifier field.
     */

    void              id_text_changed                       (const QString& the_text);

    /**
      \brief save_clicked Process the click on the save button.

      Copy the data on screen to the parameter, so it can be retrieved
      later. Then, close the dialog by means of the accept() method.
     */

    void              save_clicked                          (void);

  protected:

    /// \brief. The push button to cancel (close) the dialog.

    QPushButton*      cancel_button_;

    /// \brief The text field to type the description of the parameter.

    QLineEdit*        description_text_;

    /// \brief The text field to type the identifier of the parameter.

    QLineEdit*        id_text_;

    /// \brief The structure where the data input by the user will be
    ///        saved.

    toolkit_parameter parameter_;

    /// \brief The push button to save the data input by the user and
    ///        then close the dialog.

    QPushButton*      save_button_;

    /// \brief The set of valid data types (such as INTEGER or STRING)
    ///        for the parameters.

    QSet<QString>     the_types_;

    /// \brief A combo to select the type of the parameter.

    QComboBox*        type_cb_;
};

#endif // TOOLKITEDITOR_SINGLE_PARAMETER_WIDGET_HPP
