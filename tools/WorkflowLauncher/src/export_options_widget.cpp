/** \file export_options_widget.cpp
\brief Implementation file for export_options_widget.hpp
*/

#include "export_options_widget.hpp"

bool
export_options_widget::
check_completeness
(void)
{
  {
    QString text_value;

    text_value = repo_path_text_->text().trimmed();
    if (text_value.isEmpty()) return false;

    return true;
  }
}

export_options_widget::
export_options_widget
(QWidget* parent)
    : QDialog(parent)
{
  {
    // Initialize.

    status_ = false;

    //
    // Create the different graphic elements.
    //

    QGridLayout* data_layout = new QGridLayout;

    // Operating system selection (label & combo).

    QLabel* os_label = new QLabel(tr("Select the operating system for\nwhich you want to generate\nthe shell files"));
    os_cb_           = new QComboBox;

    // Legal values for the operating systems.

    os_cb_->addItem("Windows");
    os_cb_->addItem("Linux / Unix");

    // Set the default value for the operating system.

    os_cb_->setCurrentIndex(0);

    // Repository path (label, text & push button).

    QLabel* repo_path_label = new QLabel(tr("Folder to export the output files"));
    repo_path_text_         = new QLineEdit();
    repo_path_text_->setText("");
    repo_path_text_->setEnabled(false);

    QPushButton* repo_path_bt_ = new QPushButton(tr("..."));

    // Control buttons.

    accept_bt_ = new QPushButton(tr("Save"));
    cancel_bt_ = new QPushButton(tr("Cancel"));

    // Place all controls in our grid layout.

    data_layout->addWidget(os_label,        0, 0);
    data_layout->addWidget(os_cb_,          0, 1);

    data_layout->addWidget(repo_path_label, 1, 0);
    data_layout->addWidget(repo_path_text_, 1, 1);
    data_layout->addWidget(repo_path_bt_,   1, 2);

    data_layout->addWidget(cancel_bt_,      3, 0);
    data_layout->addWidget(accept_bt_,      3, 2);

    // Connections.

    connect(accept_bt_,    SIGNAL (clicked()),
            this,          SLOT   (on_accept_dialog()));

    connect(cancel_bt_,    SIGNAL (clicked()),
            this,          SLOT   (on_cancel_dialog()));

    connect(repo_path_bt_, SIGNAL (clicked()),
            this,          SLOT   (on_browse_path()));

    // Overall layout.

    setLayout(data_layout);

    // Minimum width.

    setMinimumWidth(700);

    // That's all.

    return;
  }
}

QString
export_options_widget::
get_os
(void)
{
  {
    return os_cb_->currentText();
  }
}


QString
export_options_widget::
get_path
(void)
{
  {
    return repo_path_text_->text().trimmed();
  }
}

bool
export_options_widget::
get_status
(void)
{
  {
    return status_;
  }
}

void
export_options_widget::
on_accept_dialog
(void)
{
  {
    if (!check_completeness())
    {
      QMessageBox msgBox;

      msgBox.setWindowTitle(tr("Incomplete export information"));
      msgBox.setText("Please, select an output folder before\naccepting this dialog or cancel it");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);

      msgBox.exec();

      return;
    }

    status_ = true;
    close();
    return;
  }
}

void
export_options_widget::
on_browse_path
(void)
{
  {
    QFileDialog dialog;
    QStringList files;
    QString     filename;

    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (!dialog.exec()) return;

    files    = dialog.selectedFiles();
    filename = files.first();

    repo_path_text_->setText(filename);

    return;
  }
}

void
export_options_widget::
on_cancel_dialog
(void)
{
  {
    status_ = false;
    close();
    return;
  }
}
