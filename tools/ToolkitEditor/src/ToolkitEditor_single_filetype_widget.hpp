/** \file ToolkitEditor_single_filetype_widget.cpp
\brief Dialog to enter the data related to a file type.
*/

#ifndef TOOLKITEDITOR_SINGLE_FILETYPE_WIDGET_HPP
#define TOOLKITEDITOR_SINGLE_FILETYPE_WIDGET_HPP

#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

#include "toolkit_structures.hpp"

/// \brief Dialog to enter the data related to a file type.

class ToolkitEditor_single_filetype_widget : public QDialog
{
    Q_OBJECT

  public:

    /**
      \brief Return the values typed for the file type.
      \return The file type, as typed in the dialog box.
     */

    toolkit_file_type file_type (void);

    /**
      \brief Constructor.
      \param filetype The file type whose values will be shown on the
             screen when the dialog is opened.
      \param mode Work mode. When set to 0, the dialog will work in "new
             file type" mode, allowing to modify all fields; on the contrary,
             when it is set to any other value, the dialog will work in
             "edit file type" mode, not allowing to change the file type
             identifier.
      \param parent The parent of this dialog.
     */

    explicit          ToolkitEditor_single_filetype_widget (toolkit_file_type& filetype,
                                                            int                mode,
                                                            QWidget*           parent = nullptr);

  protected slots:

    /**
      \brief Replace the text in the idenfier field by uppercase letters.
      \param the_text The text currently typed in the identifier field.
     */

    void              id_text_changed                      (const QString& the_text);

    /**
      \brief save_clicked Process the click on the save button.

      Copy the data on screen to the file type, so it can be retrieved
      later. Then, close the dialog by means of the accept() method.
     */

    void              save_clicked                         (void);

  protected:

    /// \brief. The push button to cancel (close) the dialog.

    QPushButton*      cancel_button_;

    /// \brief The text field to type the description of the file type.

    QLineEdit*        description_text_;

    /// \brief The text field to type the extension for this file type.

    QLineEdit*        extension_text_;

    /// \brief The structure where the data input by the user will be
    ///        saved.

    toolkit_file_type filetype_;

    /// \brief The text field to type the identifier of the file type.

    QLineEdit*        id_text_;

    /// \brief The push button to save the data input by the user and
    ///        then close the dialog.

    QPushButton*      save_button_;
};

#endif // TOOLKITEDITOR_SINGLE_FILETYPE_WIDGET_HPP
