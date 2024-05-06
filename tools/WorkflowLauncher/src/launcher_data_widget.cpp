/** \file launcher_data_widget.cpp
\brief Implementation file for launcher_data_widget.hpp
*/

#include "launcher_data_widget.hpp"

void
launcher_data_widget::
get_dialog_data
(QString& l_id,
 QString& l_description,
 QString& wf_path)
{
  {
    l_id          = launcher_id_text_->text().trimmed();
    l_description = launcher_description_text_->text().trimmed();
    wf_path       = workflow_filename_text_->text().trimmed();
  }
}

bool
launcher_data_widget::
check_completeness
(void)
{
  bool result;

  {
    QString text_value;

    result = true;

    text_value = launcher_id_text_->text().trimmed();
    if (text_value.isEmpty()) result = false;

    text_value = launcher_description_text_->text().trimmed();
    if (text_value.isEmpty()) result = false;

    text_value = workflow_filename_text_->text().trimmed();
    if (text_value.isEmpty()) result = false;
  }

  return result;
}

void
launcher_data_widget::
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

    emit data_is_available();

    close();
  }
}

void
launcher_data_widget::
on_id_text_changed
(const QString& the_text)
{
  {
    QString utext = the_text.toUpper();
    launcher_id_text_->setText(utext);
  }
}

void
launcher_data_widget::
on_select_workflow_file
(void)
{
  {
    string                     error_message;
    QStringList                fileNames;
    QString                    path;

    // Get the name of the toolkit file.

    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Name of the workflow file for this launcher"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Workflow file (*.xml)");

    if (!dialog.exec()) return;

    fileNames = dialog.selectedFiles();

    path = fileNames[0];

    workflow_filename_text_->setText(path);
  }
}

launcher_data_widget::
launcher_data_widget
(QWidget*                parent)
    : QWidget(parent)
{
  {
    int row;


    // Initialize the non-graphic members.

    row = 0;

    //
    // Create the different graphic elements.
    //

    QGridLayout* data_layout = new QGridLayout;

    // Workflow identifier.

    QLabel* launcher_id_label = new QLabel(tr("Launcher identifier"));
    launcher_id_text_         = new QLineEdit();
    launcher_id_text_->setText("");

    data_layout->addWidget(launcher_id_label,   row, 0);
    data_layout->addWidget(launcher_id_text_,   row, 1);

    row++;

    // Launcher description.

    QLabel* launcher_description_label = new QLabel(tr("Launcher description"));
    launcher_description_text_         = new QLineEdit();
    launcher_description_text_->setText("");

    data_layout->addWidget(launcher_description_label, row, 0);
    data_layout->addWidget(launcher_description_text_, row, 1);

    row++;

    // Path to the workflow on which the launcher will rely.

    QLabel* workflow_filename_label = new QLabel(tr("Path to the workflow\nused by the launcher"));
    workflow_filename_text_         = new QLineEdit();
    control_button_select_workflow_ = new QPushButton("...");
    workflow_filename_text_->setText("");

    data_layout->addWidget(workflow_filename_label,         row, 0);
    data_layout->addWidget(workflow_filename_text_,         row, 1);
    data_layout->addWidget(control_button_select_workflow_, row, 2);

    workflow_filename_text_->setReadOnly(true);

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

    // Select workflow.

    connect(control_button_select_workflow_, SIGNAL(clicked()),
            this,                            SLOT  (on_select_workflow_file()));

    // Close the dialog.

    connect(control_button_cancel_,          SIGNAL(clicked()),
            this,                            SLOT  (close()));

    // Confirm (OK)

    connect(control_button_confirm_,         SIGNAL(clicked()),
            this,                            SLOT  (on_confirm()));

    // Value of the launcher identifier text field changed.

    connect(launcher_id_text_,               SIGNAL(textChanged(const QString&)),
            this,                            SLOT  (on_id_text_changed(const QString&)));

    //
    // Set the window title.
    //

    setWindowTitle(tr("Please, provide the following data for the launcher"));

  }
}

launcher_data_widget::
~launcher_data_widget
(void)
{
  {
    // Intentionally left blank.
  }
}
