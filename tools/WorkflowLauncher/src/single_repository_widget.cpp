/** \file single_repository_widget.cpp
\brief Implementation file for single_repository_widget.hpp
*/

#include "single_repository_widget.hpp"

bool
single_repository_widget::
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

QString
single_repository_widget::
get_id
(void)
{
  {
    return repo_id_text_->text().trimmed();
  }
}

QString
single_repository_widget::
get_value
(void)
{
  {
    return repo_path_text_->text().trimmed();
  }
}

void
single_repository_widget::
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

    files = dialog.selectedFiles();
    filename = files.first();

    repo_path_text_->setText(filename);

    return;
  }
}

single_repository_widget::
single_repository_widget
(QColor&       bg_color,
 WLRepository& value,
 QWidget*      parent)
    : QWidget(parent)
{
  {
    //
    // Create the different graphic elements.
    //

    QGridLayout* data_layout = new QGridLayout;

    // Header label.

    QLabel* header_label = new QLabel(tr("REPOSITORY"));

    // Repository id.

    QLabel* repo_id_label = new QLabel(tr("  Identifier"));
    repo_id_text_         = new QLineEdit();
    repo_id_text_->setText(QString::fromStdString(value.id));
    repo_id_text_->setReadOnly(true);

    // Repository path (label, text & push button).

    QLabel* repo_path_label = new QLabel(tr("  Path"));
    repo_path_text_         = new QLineEdit();
    repo_path_text_->setText(QString::fromStdString(value.path));
    repo_path_text_->setReadOnly(true);

    QPushButton* repo_path_bt_ = new QPushButton(tr("..."));

    // Place all controls in our grid layout.

    data_layout->addWidget(header_label,    0, 0, 1, 2);

    data_layout->addWidget(repo_id_label,   1, 0, 1, 1);
    data_layout->addWidget(repo_id_text_,   1, 1, 1, 3);

    data_layout->addWidget(repo_path_label, 2, 0, 1, 1);
    data_layout->addWidget(repo_path_text_, 2, 1, 1, 3);
    data_layout->addWidget(repo_path_bt_,   2, 4, 1, 1);

    // Connections.

    connect(repo_path_bt_, SIGNAL (clicked()),
            this,          SLOT   (on_browse_path()));

    // Overall layout.

    setLayout(data_layout);

    // Minimum width.

    setMinimumWidth(700);

    // Background color.

    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, bg_color);
    setAutoFillBackground(true);
    setPalette(pal);

    // That's all.

    return;

  }
}
