/** \file launcher_data_widget.hpp
\brief Dialog to collect the lineage data for a new launcher.
*/

#ifndef LAUNCHER_DATA_WIDGET_HPP
#define LAUNCHER_DATA_WIDGET_HPP

#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QWidget>

using namespace std;

/// \brief Dialog to collect the lineage data for a new launcher.
/**
  This class provides a dialog to collect the "lineage" data
  for a new launcher, that is, its identifier, description and
  the workflow on which it relies.
 */

class launcher_data_widget : public QWidget
{
    Q_OBJECT

  public:

    /// \brief Retrieve the data shown on the dialog.
    /**
      \param[out] l_id Launcher identifier.
      \param[out] l_description Launcher description.
      \param[out] wf_path Path to the workflow on which
                  the launcher relies.
     */

    void get_dialog_data          (QString& l_id,
                                   QString& l_description,
                                   QString& wf_path);

    /// \brief Constructor.
    /**
      \param[in] parent This widget's parent.
     */

         launcher_data_widget     (QWidget* parent = nullptr);

    /// Destructor.

         ~launcher_data_widget    (void);

  signals:

    /// \brief Inform whoever interested in the dialog that new data
    ///        is available.

    void data_is_available        (void);

  protected slots:

    /// \brief Accept (confirm) the data input on the dialog.
    /**
       Callback of QPushButton control_button_confirm_.
    */

    void  on_confirm              (void);

    /**
      \brief Replace the text in the workflow idenfier field by uppercase letters.
      \param the_text The text currently typed in the workflow identifier field.
     */

    void  on_id_text_changed      (const QString& the_text);

    /// \brief Select the file with the workflow on which the
    ///        launcher will rely.
    /**
       Callback of QPushButton control_button_select_workflow_.
    */

    void  on_select_workflow_file (void);

  protected:

    /// \brief Check that all the required parameters have
    /// been input in the widget's interface.
    /**
      \return True if all parameters have been set, false
              otherwise.
     */

    bool  check_completeness      (void);

  protected:

    /// \brief Push button: cancel.

    QPushButton* control_button_cancel_;

    /// \brief Push button: confirm data.

    QPushButton* control_button_confirm_;

    /// \brief Push button: select an existing workflow file.

    QPushButton* control_button_select_workflow_;

    /// \brief Line edit:  launcher description.

    QLineEdit*   launcher_description_text_;

    /// \brief Line edit: launcher identifier.

    QLineEdit*   launcher_id_text_;


    /// \brief Line edit: path to the workflow on which
    ///        the launcher will rely.

    QLineEdit*   workflow_filename_text_;
};

#endif // LAUNCHER_DATA_WIDGET_HPP
