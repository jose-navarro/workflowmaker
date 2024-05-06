/** \file workflow_data_widget.cpp
\brief Implementation file for workflow_data_widget.hpp
*/

#include "workflow_data_widget.hpp"

void
workflow_data_widget::
get_dialog_data
(QString& wk_id,
 QString& wk_description,
 QString& tk_path)
{
  {
    wk_id          = workflow_id_text_->text().trimmed();
    wk_description = workflow_description_text_->text().trimmed();
    tk_path        = toolkit_filename_text_->text().trimmed();
  }
}

bool
workflow_data_widget::
check_completeness
(void)
{
  bool result;

  {
    QString text_value;

    result = true;

    text_value = workflow_id_text_->text().trimmed();
    if (text_value.isEmpty()) result = false;

    text_value = workflow_description_text_->text().trimmed();
    if (text_value.isEmpty()) result = false;

    text_value = toolkit_filename_text_->text().trimmed();
    if (text_value.isEmpty()) result = false;
  }

  return result;
}

void
workflow_data_widget::
on_confirm
(void)
{
  {
    bool complete;

    //
    // Check that all the requested information has been
    // provided by the user.
    //

    complete = check_completeness();

    if (!complete)
    {
      QMessageBox msgBox;

      msgBox.setWindowTitle(tr("Missing data"));
      msgBox.setText(tr("All fields must be set to proceed.\nPlease, enter the missing data"));
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);

      msgBox.exec();

      return;
    }

    emit data_is_available ();

    close();

    qDebug() << "Datos confirmados, señal emitida";
  }
}

void
workflow_data_widget::
on_id_text_changed
(const QString& the_text)
{
  {
    QString utext = the_text.toUpper();
    workflow_id_text_->setText(utext);
  }
}

void
workflow_data_widget::
on_select_toolkit_file
(void)
{
  {
    string                     error_message;
    QStringList                fileNames;
    QString                    path;

    // Get the name of the toolkit file.

    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Name of the toolkit file for this workflow"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Toolkit file (*.xml)");

    if (!dialog.exec()) return;

    fileNames = dialog.selectedFiles();

    path = fileNames[0];

    toolkit_filename_text_->setText(path);
  }
}

workflow_data_widget::
workflow_data_widget
(QWidget*                parent)
    : QWidget(parent)
{
  {
    int                  row;


    // Initialize the non-graphic members.

    row = 0;

    //
    // Create the different graphic elements.
    //

    QGridLayout* data_layout = new QGridLayout;

    // Workflow identifier.

    QLabel* workflow_id_label = new QLabel(tr("Workflow identifier"));
    workflow_id_text_         = new QLineEdit();
    workflow_id_text_->setText("");

    data_layout->addWidget(workflow_id_label,   row, 0);
    data_layout->addWidget(workflow_id_text_,   row, 1);

    row++;

    // Workflow description.

    QLabel* workflow_identifier_label = new QLabel(tr("Workflow description"));
    workflow_description_text_        = new QLineEdit();
    workflow_description_text_->setText("");

    data_layout->addWidget(workflow_identifier_label,  row, 0);
    data_layout->addWidget(workflow_description_text_, row, 1);

    row++;

    // Path to the toolkit on which the workflow will rely.

    QLabel* toolkit_filename_label = new QLabel(tr("Path to the toolkit\nused by the workflow"));
    toolkit_filename_text_         = new QLineEdit();
    control_button_select_toolkit_ = new QPushButton("...");
    toolkit_filename_text_->setText("");

    data_layout->addWidget(toolkit_filename_label,         row, 0);
    data_layout->addWidget(toolkit_filename_text_,         row, 1);
    data_layout->addWidget(control_button_select_toolkit_, row, 2);

    toolkit_filename_text_->setReadOnly(true);

    row++;

    // Control buttons.

    QHBoxLayout* control_button_layout = new QHBoxLayout;
    control_button_cancel_  = new QPushButton(tr("Cancel"));
    control_button_confirm_ = new QPushButton(tr("OK"));

    control_button_layout->addWidget(control_button_cancel_);
    control_button_layout->addStretch();
    control_button_layout->addWidget(control_button_confirm_);

    // Overall layout.

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(data_layout);
    mainLayout->addLayout(control_button_layout);
    setLayout(mainLayout);

    //
    // Connect signals and slots.
    //

    // Select toolkit.

    connect(control_button_select_toolkit_, SIGNAL(clicked()),
            this,                           SLOT  (on_select_toolkit_file()));

    // Close the dialog.

    connect(control_button_cancel_,         SIGNAL(clicked()),
            this,                           SLOT  (close()));

    // Confirm (OK)

    connect(control_button_confirm_,        SIGNAL(clicked()),
            this,                           SLOT  (on_confirm()));

    // Value of the identifier workflow text field changed.

    connect(workflow_id_text_,              SIGNAL(textChanged(const QString&)),
            this,                           SLOT  (on_id_text_changed(const QString&)));

    //
    // Set the window title.
    //

    setWindowTitle(tr("Please, provide the following workflow data"));

  }
}

workflow_data_widget::
~workflow_data_widget
(void)
{
  {
    // Intentionally left blank.
  }
}
