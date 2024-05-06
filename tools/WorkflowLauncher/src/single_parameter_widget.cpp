/** \file single_parameter_widget.cpp
\brief Implementation file for single_parameter_widget.hpp
*/

#include "single_parameter_widget.hpp"

bool
single_parameter_widget::
check_completeness
(void)
{
  {
    QString text_value;
    QString type_value;

    type_value = param_type_text_->text().trimmed();


    if ((type_value == "boolean") || (type_value == "BOOLEAN"))
    {
      QString value = param_value_cb_->currentText();
      return (value != "Not set");
    }

    text_value = param_value_text_->text().trimmed();
    if (text_value.isEmpty()) return false;

    return true;
  }
}

QString
single_parameter_widget::
get_id
(void)
{
  {
    return param_label_text_->text().trimmed();
  }
}

QString
single_parameter_widget::
get_value
(void)
{
  {
    QString type_value;
    type_value = param_type_text_->text().trimmed();

    if ((type_value == "boolean") || (type_value == "BOOLEAN"))
    {
       return param_value_cb_->currentText();
    }
    else
    {
      return param_value_text_->text().trimmed();
    }
  }
}

single_parameter_widget::
single_parameter_widget
(QColor&      bg_color,
 WLParameter& value,
 QWidget*     parent)
    : QWidget(parent)
{
  {
    bool is_boolean;

    //
    // Create the different graphic elements.
    //

    QGridLayout* data_layout = new QGridLayout;

    // Header label.

    QLabel* header_label = new QLabel(tr("PARAMETER"));

    // Identifier of the task to which the parameter belongs.

    QString full_id = "(" + QString::fromStdString(value.task_seqid) + ") " + QString::fromStdString(value.task_id);

    QLabel* param_task_id_label = new QLabel(tr("  Task identifier"));
    param_task_id_text_         = new QLineEdit();
    param_task_id_text_->setText(full_id);
    param_task_id_text_->setReadOnly(true);

    // Parameter description.

    QLabel* param_description_label = new QLabel(tr("  Description"));
    param_description_text_         = new QLineEdit();
    param_description_text_->setText(QString::fromStdString(value.description));
    param_description_text_->setReadOnly(true);

    // Parameter type & label.

    QLabel* param_label_label = new QLabel(tr("  Label"));
    param_label_text_         = new QLineEdit();
    param_label_text_->setText(QString::fromStdString(value.id));
    param_label_text_->setReadOnly(true);

    QLabel* param_type_label = new QLabel(tr("  Type"));
    param_type_text_         = new QLineEdit();
    param_type_text_->setText(QString::fromStdString(value.type));
    param_type_text_->setReadOnly(true);

    // Parameter value.

    QLabel* param_value_label = new QLabel(tr("  Value"));

    if ((value.type == "boolean") || (value.type == "BOOLEAN"))
    {
      is_boolean = true;

      param_value_cb_ = new QComboBox;

      if (value.value == "")
      {
        param_value_cb_->addItem("Not set");
      }

      param_value_cb_->addItem("TRUE");
      param_value_cb_->addItem("FALSE");

      if      (value.value == "")                                  param_value_cb_->setCurrentIndex(0);
      else if ((value.value == "true") || (value.value == "TRUE")) param_value_cb_->setCurrentIndex(1);
      else                                                         param_value_cb_->setCurrentIndex(2);

    }
    else
    {
      is_boolean = false;
      param_value_text_         = new QLineEdit();
      param_value_text_->setText(QString::fromStdString(value.value));

      if ((value.type == "integer") || (value.type == "INTEGER"))
      {
        QIntValidator* v = new QIntValidator();
        param_value_text_->setValidator(v);
      }
      else if ((value.type == "real") || (value.type == "REAL"))
      {
        QDoubleValidator* v = new QDoubleValidator();
        param_value_text_->setValidator(v);
      }
    }

    // Place all controls in our grid layout.

    data_layout->addWidget(header_label,            0, 0, 1, 2);

    data_layout->addWidget(param_task_id_label,     1, 0, 1, 1);
    data_layout->addWidget(param_task_id_text_,     1, 1, 1, 3);

    data_layout->addWidget(param_description_label, 2, 0, 1, 1);
    data_layout->addWidget(param_description_text_, 2, 1, 1, 3);

    data_layout->addWidget(param_type_label,        3, 0, 1, 1);
    data_layout->addWidget(param_type_text_,        3, 1, 1, 3);

    data_layout->addWidget(param_label_label,       4, 0, 1, 1);
    data_layout->addWidget(param_label_text_,       4, 1, 1, 3);

    data_layout->addWidget(param_value_label,       5, 0, 1, 1);

    if (is_boolean)
    {
      data_layout->addWidget(param_value_cb_,         5, 1, 1, 3);
    }
    else
    {
      data_layout->addWidget(param_value_text_,       5, 1, 1, 3);
    }

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
