/** \file workflow_data_widget.hpp
\brief Dialog to collect the lineage data for a new workflow.
*/

#ifndef WORKFLOW_DATA_WIDGET_HPP
#define WORKFLOW_DATA_WIDGET_HPP

#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QWidget>

#include <QDebug>

using namespace std;

/// \brief Dialog to collect the lineage data for a new workflow.
/**
  This class provides a dialog to collect the "lineage" data
  for a new workflow, that is, its identifier, description and
  the toolkit on which it relies.
 */

class workflow_data_widget : public QWidget
{
    Q_OBJECT

  public:

    /// \brief Retrieve the data shown on the dialog.
    /**
      \param[out] wk_id
      \param[out] wk_description
      \param[out] tk_path
     */

    void get_dialog_data        (QString& wk_id,
                                 QString& wk_description,
                                 QString& tk_path);

    /// \brief Constructor.
    /**
      \param[in] parent This widget's parent.
     */

         workflow_data_widget   (QWidget* parent = nullptr);

    /// Destructor.

         ~workflow_data_widget  (void);

  signals:

    /// \brief Inform whoever interested in the dialog that new data
    ///        is available.

    void data_is_available      (void);

  protected slots:

    /// \brief Accept (confirm) the data input on the dialog.
    /**
       Callback of QPushButton control_button_confirm_.
    */

    void  on_confirm             (void);

    /**
      \brief Replace the text in the workflow idenfier field by uppercase letters.
      \param the_text The text currently typed in the workflow identifier field.
     */

    void  on_id_text_changed     (const QString& the_text);

    /// \brief Select the file with the toolkit on which the
    ///        workflow will rely.
    /**
       Callback of QPushButton control_button_select_toolkit_.
    */

    void  on_select_toolkit_file (void);

  protected:

    /// \brief Check that all the required parameters have
    /// been input in the widget's interface.
    /**
      \return True if all parameters have been set, false
              otherwise.
     */

    bool  check_completeness     (void);

  protected:

    /// \brief Push button: cancel.

    QPushButton* control_button_cancel_;

    /// \brief Push button: confirm data.

    QPushButton* control_button_confirm_;

    /// \brief Push button: select an existing toolkit file.

    QPushButton* control_button_select_toolkit_;

    /// \brief Line edit: path to the toolkit on which
    ///        the workflow will rely.

    QLineEdit*   toolkit_filename_text_;

    /// \brief Line edit:  workflow description.

    QLineEdit*   workflow_description_text_;

    /// \brief Line edit: workflow identifier.

    QLineEdit*   workflow_id_text_;

};

#endif // WORKFLOW_DATA_WIDGET_HPP
