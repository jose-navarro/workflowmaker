/** \file single_file_widget.cpp
\brief Implementation file for single_file_widget.hpp
*/

#include "single_file_widget.hpp"

bool
single_file_widget::
check_completeness
(void)
{
  {
    QString text_value;

    text_value = file_name_text_->text().trimmed();
    if (text_value.isEmpty()) return false;

    return true;
  }
}

QString
single_file_widget::
get_value
(void)
{
  {
    return file_name_text_->text().trimmed();
  }
}

single_file_widget::
single_file_widget
(QColor&  bg_color,
 WLFile&  value,
 QWidget* parent)
    : QWidget(parent)
{
  {
    //
    // Create the different graphic elements.
    //

    QGridLayout* data_layout = new QGridLayout;

    // Header label.

    QLabel* header_label = new QLabel(tr("FILE"));

    // File description.

    QLabel* file_description_label = new QLabel(tr("  Description"));
    file_description_text_         = new QLineEdit();
    file_description_text_->setText(QString::fromStdString(value.description));
    file_description_text_->setReadOnly(true);

    // File type & general description.

    QString tmp_string;

    tmp_string = QString::fromStdString(value.file_type_id)
               + " - "
               + QString::fromStdString(value.file_type_description);

    QLabel* file_type_label = new QLabel(tr("  Type"));
    file_type_text_         = new QLineEdit();
    file_type_text_->setText(tmp_string);
    file_type_text_->setReadOnly(true);

    QString slot1, slot2, taskid1, taskid2, type1, type2;

    if (value.start_node_type == TaskEndPoint)
    {
      taskid1 = "(" + QString::number(value.start_node_seqid) + ") " + QString::fromStdString(value.start_node_id);
      type1   = "Created by task ";
      slot1   = " (output of slot #" + QString::number(value.start_node_slot + 1) + ") ";
    }
    else
    {
      taskid1 = QString::fromStdString(value.start_node_id);
      type1   = "Stored in repository ";
      slot1   = "";
    }

    if (value.end_node_type == TaskEndPoint)
    {
      taskid2 = "(" + QString::number(value.end_node_seqid) + ") " + QString::fromStdString(value.end_node_id);
      type2 = " and read by task ";
      slot2 = " (input for slot #" + QString::number(value.end_node_slot +1) + ")";
    }
    else
    {
      taskid2 = QString::fromStdString(value.end_node_id);
      type2 = " and saved to repository ";
      slot2 = "";
    }

    tmp_string = type1
               + taskid1
               + slot1
               + type2
               + taskid2
               + slot2;

    QLabel* file_connection_label = new QLabel(tr("  Usage"));
    file_connection_text_         = new QLineEdit();
    file_connection_text_->setText(tmp_string);
    file_connection_text_->setReadOnly(true);

    // File name.

    QLabel* file_name_label = new QLabel(tr("  File name"));
    file_name_text_         = new QLineEdit();
    file_name_text_->setText(QString::fromStdString(value.filename));


    // Place all controls in our grid layout.

    data_layout->addWidget(header_label,           0, 0, 1, 2);

    data_layout->addWidget(file_description_label, 1, 0, 1, 1);
    data_layout->addWidget(file_description_text_, 1, 1, 1, 3);

    data_layout->addWidget(file_type_label,        2, 0, 1, 1);
    data_layout->addWidget(file_type_text_,        2, 1, 1, 3);

    data_layout->addWidget(file_connection_label,  3, 0, 1, 1);
    data_layout->addWidget(file_connection_text_,  3, 1, 1, 3);

    data_layout->addWidget(file_name_label,        4, 0, 1, 1);
    data_layout->addWidget(file_name_text_,        4, 1, 1, 3);

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
