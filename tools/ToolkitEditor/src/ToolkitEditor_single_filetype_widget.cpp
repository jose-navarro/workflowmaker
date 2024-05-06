/** \file ToolkitEditor_single_filetype_widget.hpp
\brief Implementation file for ToolkitEditor_single_filetype_widget.cpp.
*/

#include "ToolkitEditor_single_filetype_widget.hpp"

toolkit_file_type
ToolkitEditor_single_filetype_widget::
file_type
(void)
{
  {
    return filetype_;
  }
}

void
ToolkitEditor_single_filetype_widget::
id_text_changed
(const QString& the_text)
{
  {
    QString utext = the_text.toUpper();
    id_text_->setText(utext);
  }
}

void
ToolkitEditor_single_filetype_widget::
save_clicked
(void)
{
  {
    filetype_.id          = id_text_->text().toStdString();
    filetype_.description = description_text_->text().toStdString();
    filetype_.extension   = extension_text_->text().toStdString();

    accept();
  }
}

ToolkitEditor_single_filetype_widget::
ToolkitEditor_single_filetype_widget
(toolkit_file_type& filetype,
 int                mode,
 QWidget*           parent) :
  QDialog(parent)
{
  {

    //------------------------------------------------------------
    // LOCAL VARIABLES AND MEMBER INITIALIZATION
    //------------------------------------------------------------

    // Copy the point parameter to our member.

    filetype_ = filetype;

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
    QLabel* description_label = new QLabel(tr("Description"));
    QLabel* extension_label   = new QLabel(tr("File extension"));

    id_text_                  = new QLineEdit;
    description_text_         = new QLineEdit;
    extension_text_           = new QLineEdit;

    save_button_              = new QPushButton(tr("Save"));
    cancel_button_            = new QPushButton(tr("Cancel"));

    //------------------------------------------------------------
    // SET SCREEN VALUES
    //------------------------------------------------------------

    // Display the identifier and description on the screen.


    id_text_->setText(QString::fromStdString(filetype_.id));
    description_text_->setText(QString::fromStdString(filetype_.description));
    extension_text_->setText(QString::fromStdString(filetype_.extension));

    //------------------------------------------------------------
    // BUILD THE INTERFACE: INSERT THE DIFFERENT ELEMENTS INTO
    // THEIR RESPECTIVE LAYOUTS.
    //------------------------------------------------------------

    //
    // "Editable" area (identifier & description)
    //

    editLayout->addWidget(id_label,          0, 0);
    editLayout->addWidget(id_text_,          0, 1);

    editLayout->addWidget(description_label, 1, 0);
    editLayout->addWidget(description_text_, 1, 1);

    editLayout->addWidget(extension_label,   2, 0);
    editLayout->addWidget(extension_text_,   2, 1);

    // Control buttons (save & cancel)

    buttonLayout->addWidget(cancel_button_);
    buttonLayout->addStretch();
    buttonLayout->addWidget(save_button_);

    // Enable or disable controls, depending on the work mode.

    if (mode != 0) id_text_->setReadOnly(true);

    // Set minimum width.

    setMinimumWidth(500);

    // Set window title.

    setWindowTitle(tr("File type data"));

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
