/** \file export_options_widget.hpp
\brief Dialog to collect the options related to the exportation of shell files.
*/

#ifndef EXPORT_OPTIONS_WIDGET_HPP
#define EXPORT_OPTIONS_WIDGET_HPP

#include <QComboBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QWidget>


using namespace std;

/// \brief Dialog to collect the options related to the exportation of shell files.

class export_options_widget : public QDialog
{
    Q_OBJECT

  public:

    /// \brief Check that all the required data have
    /// been input in the widget's interface.
    /**
      \return True if all data have been set, false
              otherwise.
     */

    bool    check_completeness    (void);


    /// \brief Constructor.
    /**
      \param[in] parent This widget's parent.
     */

            export_options_widget (QWidget* parent = nullptr);

    /// \brief Retrieve the operating system selected by the user.
    /**
      \return The text identifying the operating system. The possible
              values are:

              - "Windows"
              - "Linux / Unix"
     */

    QString get_os                (void);

    /// \brief Retrieve the value for the repository.
    /**
      \return The path assigned to the repository.
     */

    QString get_path             (void);

    /// \brief Check whether the user accepted the dialog.
    /**
      \return True if the user accepted the dialog, false otherwise.
     */

    bool    get_status           (void);

  protected slots:

    /// \brief Accept the dialog.

    void    on_accept_dialog      (void);

    /// \brief Browse to identify the path to assign to the repository.

    void    on_browse_path        (void);

    /// \brief Cancel the dialog.

    void    on_cancel_dialog      (void);

  protected:

    /// \brief Push button: accept the dialog.

    QPushButton* accept_bt_;

    /// \brief Push button: cancel the dialog.

    QPushButton* cancel_bt_;

    /// \brief Combo box: select operating system for the shell files.

    QComboBox*   os_cb_;

    /// \brief Line edit: Repository path.

    QLineEdit*   repo_path_text_;

    /// \brief Status of the dialog box (accepted = true, rejected = false)

    bool         status_;
};

#endif // EXPORT_OPTIONS_WIDGET_HPP
