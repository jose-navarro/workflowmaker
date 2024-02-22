/** \file ToolkitEditor_single_parameter_widget.hpp
\brief Implementation file for ToolkitEditor_single_parameter_widget.cpp.
*/

#include "ToolkitEditor_single_parameter_widget.hpp"

void
ToolkitEditor_single_parameter_widget::
id_text_changed
(const QString& the_text)
{
  {
    QString utext = the_text.toUpper();
    id_text_->setText(utext);
  }
}

void
ToolkitEditor_single_parameter_widget::
save_clicked
(void)
{
  {
    parameter_.id          = id_text_->text().toStdString();
    parameter_.type        = type_cb_->currentText().toStdString();
    parameter_.description = description_text_->text().toStdString();

    accept();
  }
}

ToolkitEditor_single_parameter_widget::
ToolkitEditor_single_parameter_widget
(toolkit_parameter& parameter,
 int                mode,
 QWidget*           parent) :
  QDialog(parent)
{
  {

    //------------------------------------------------------------
    // LOCAL VARIABLES AND MEMBER INITIALIZATION
    //------------------------------------------------------------

    // Copy the point parameter to our member.

    parameter_ = parameter;

    // Legal values for the types.

    the_types_.insert("INTEGER");
    the_types_.insert("REAL");
    the_types_.insert("BOOLEAN");
    the_types_.insert("STRING");

    //------------------------------------------------------------
    // CREATE LAYOUTS AND SUBLAYOUTS.
    //------------------------------------------------------------

    QVBoxLayout* mainLayout   = new QVBoxLayout(this);
    QGridLayout* editLayout   = new QGridLayout;
    QHBoxLayout* buttonLayout = new QHBoxLayout;

    mainLayout->addLayout(editLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    //------------------------------------------------------------
    // INSTANTIATE ALL GRAPHIC ELEMENTS IN THE INTERFACE
    //------------------------------------------------------------

    QLabel* id_label          = new QLabel(tr("Identifier"));
    QLabel* type_label        = new QLabel(tr("Type"));
    QLabel* description_label = new QLabel(tr("Description"));

    id_text_                  = new QLineEdit;
    type_cb_                  = new QComboBox;
    description_text_         = new QLineEdit;

    save_button_              = new QPushButton(tr("Save"));
    cancel_button_            = new QPushButton(tr("Cancel"));


    //------------------------------------------------------------
    // SET VALUES / PROPERTIES FOR SEVERAL GRAPHIC ELEMENTS
    //------------------------------------------------------------

    foreach (QString item, the_types_)
    {
      type_cb_->addItem(item);
    }

    //------------------------------------------------------------
    // SET SCREEN VALUES
    //------------------------------------------------------------

    // Display the values of the identifier, description and type on the screen.


    id_text_->setText(QString::fromStdString(parameter_.id));
    description_text_->setText(QString::fromStdString(parameter_.description));


    QString the_type = QString::fromStdString(parameter_.type).toUpper();
    type_cb_->setCurrentText(the_type);

    //------------------------------------------------------------
    // BUILD THE INTERFACE: INSERT THE DIFFERENT ELEMENTS INTO
    // THEIR RESPECTIVE LAYOUTS.
    //------------------------------------------------------------

    //
    // "Editable" area (identifier, type & description).
    //

    editLayout->addWidget(id_label,          0, 0);
    editLayout->addWidget(id_text_,          0, 1);
    editLayout->addWidget(type_label,        1, 0);
    editLayout->addWidget(type_cb_,          1, 1);
    editLayout->addWidget(description_label, 2, 0);
    editLayout->addWidget(description_text_, 2, 1);

    // Control buttons (save & cancel)

    buttonLayout->addWidget(cancel_button_);
    buttonLayout->addStretch();
    buttonLayout->addWidget(save_button_);

    //
    // Enable or disable the identifier text box depending on the
    // work mode.
    //

    if (mode != 0) id_text_->setReadOnly(true);

    // Set window title.

    setWindowTitle(tr("Parameter data"));

    //------------------------------------------------------------
    //
    // SET UP SIGNALS AND ACTIONS
    //
    //-------------------------------------------------------------

    // Buttons save and close.

    connect(save_button_  , SIGNAL(clicked()), this , SLOT(save_clicked()));
    connect(cancel_button_, SIGNAL(clicked()), this , SLOT(close()));

    // Value of the identifier text field changed.

    connect(id_text_, SIGNAL(textChanged(const QString&)),
            this,     SLOT  (id_text_changed(const QString&)));
  }
}

toolkit_parameter
ToolkitEditor_single_parameter_widget::
parameter
(void)
{
  {
    return parameter_;
  }
}


