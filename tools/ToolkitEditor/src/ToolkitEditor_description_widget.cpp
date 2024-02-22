/** \file ToolkitEditor_description_widget.cpp
\brief Implementation file for ToolkitEditor_description_widget.hpp.
*/

#include "ToolkitEditor_description_widget.hpp"

bool
ToolkitEditor_description_widget::
check_completeness
(void)
{
  bool result;

  {
    result = true;

    if (id_text_->text().isEmpty())
    {
      result = false;

      QString message = "General information: the identifier of the toolkit may not be empty.";
      error_list_.push_back(message);
    }

    if (description_text_->text().isEmpty())
    {
      result = false;

      QString message = "General information: the description of the toolkit may not be empty.";
      error_list_.push_back(message);
    }

  }

  return result;
}

void
ToolkitEditor_description_widget::
copy_toolkit_values
(toolkit& toolkit)
{
  {
    toolkit.id          = id_text_->text().toStdString();
    toolkit.description = description_text_->text().toStdString();
  }
}

QVector<QString>
ToolkitEditor_description_widget::
error_list
(void)
{
  {
    return error_list_;
  }
}

void
ToolkitEditor_description_widget::
id_text_changed
(const QString& the_text)
{
  {
    QString utext = the_text.toUpper();
    id_text_->setText(utext);
  }
}

void
ToolkitEditor_description_widget::
reset_error_list
(void)
{
  {
    error_list_.clear();
  }
}

void
ToolkitEditor_description_widget::
set_values
(toolkit& toolkit)
{
  {
    id_text_->setText(QString::fromStdString(toolkit.id));
    description_text_->setText(QString::fromStdString(toolkit.description));
  }
}

ToolkitEditor_description_widget::
ToolkitEditor_description_widget
(QWidget* parent) :
  QDialog(parent)
{
  {

    //------------------------------------------------------------
    // CREATE LAYOUTS AND SUBLAYOUTS.
    //------------------------------------------------------------

    QVBoxLayout* mainLayout   = new QVBoxLayout(this);
    QGridLayout* editLayout   = new QGridLayout;

    mainLayout->addLayout(editLayout);
    mainLayout->addStretch();

    //------------------------------------------------------------
    // INSTANTIATE ALL GRAPHIC ELEMENTS IN THE INTERFACE
    //------------------------------------------------------------

    QLabel* id_label          = new QLabel(tr("Identifier"));
    QLabel* description_label = new QLabel(tr("Description"));

    id_text_                  = new QLineEdit;
    description_text_         = new QLineEdit;

    //------------------------------------------------------------
    // BUILD THE INTERFACE: INSERT THE DIFFERENT ELEMENTS INTO
    // THEIR RESPECTIVE LAYOUTS.
    //------------------------------------------------------------

    //
    // "Editable" area (identifier & description).
    //

    editLayout->addWidget(id_label,          0, 0);
    editLayout->addWidget(id_text_,          0, 1);
    editLayout->addWidget(description_label, 2, 0);
    editLayout->addWidget(description_text_, 2, 1);


    // Set window title.

    setWindowTitle(tr("General information"));

    //------------------------------------------------------------
    //
    // SET UP SIGNALS AND ACTIONS
    //
    //-------------------------------------------------------------

    // Value of the identifier text field changed.

    connect(id_text_, SIGNAL(textChanged(const QString&)),
            this,     SLOT  (id_text_changed(const QString&)));
  }
}

bool
ToolkitEditor_description_widget::
validate_semantics
(void)
{
  {
    //
    // Nothing special to check. However, we'll rely on
    // completeness to state whether our data are correct.
    //

    return check_completeness();
  }
}
