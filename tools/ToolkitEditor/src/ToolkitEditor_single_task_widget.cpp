/** \file ToolkitEditor_single_task_widget.hpp
\brief Implementation file for ToolkitEditor_single_task_widget.cpp.
*/

#include "ToolkitEditor_parameter_list_widget.hpp" // To avoid cyclic inclusion.
#include "ToolkitEditor_single_task_widget.hpp"


void
ToolkitEditor_single_task_widget::
id_text_changed
(const QString& the_text)
{
  {
    QString utext = the_text.toUpper();
    id_text_->setText(utext);
  }
}

void
ToolkitEditor_single_task_widget::
save_clicked
(void)
{
  {
    // Check that the task is well defined.

    if (!validate()) return;

    // Copy data to the task!!!!!!

    task_.id                 = id_text_->text().toStdString();
    task_.description        = description_text_->text().toStdString();
    task_.name_of_executable = executable_text_->text().toStdString();

    QList<toolkit_parameter>* pars = params_->get_list();
    task_.parameters.clear();
    for (int i = 0; i < pars->size(); i++)
    {
      toolkit_parameter tp = pars->at(i);
      task_.parameters.push_back(tp.id);
    }

    QList<toolkit_file>* files = ifiles_->get_list();
    task_.input_files.clear();
    for (int i = 0; i < files->size(); i++)
    {
      toolkit_file fil = files->at(i);
      task_.input_files.push_back(fil);
    }

    files = ofiles_->get_list();
    task_.output_files.clear();
    for (int i = 0; i < files->size(); i++)
    {
      toolkit_file fil = files->at(i);
      task_.output_files.push_back(fil);
    }

    // Go away!

    accept();
  }
}

toolkit_task
ToolkitEditor_single_task_widget::
task
(void)
{
  {
    return task_;
  }
}

ToolkitEditor_single_task_widget::
ToolkitEditor_single_task_widget
(toolkit_task&                   task,
 map<string, toolkit_parameter>& parameter_map,
 QList<toolkit_file_type>*       filetype_list,
 QWidget*                        parent) :
  QDialog(parent)
{
  {

    //------------------------------------------------------------
    // LOCAL VARIABLES AND MEMBER INITIALIZATION
    //------------------------------------------------------------

    // Copy the input parameters.

    task_          = task;
    parameter_map_ = parameter_map;
    filetype_list_ = filetype_list;

    //------------------------------------------------------------
    // CREATE LAYOUTS AND SUBLAYOUTS.
    //------------------------------------------------------------

    QVBoxLayout* mainLayout   = new QVBoxLayout(this);
    QGridLayout* editLayout   = new QGridLayout;
    QVBoxLayout* tkInfoLayout = new QVBoxLayout;
    QGridLayout* gInfoLayout  = new QGridLayout;
    QVBoxLayout* paramsLayout = new QVBoxLayout;
    QVBoxLayout* ifilesLayout = new QVBoxLayout;
    QVBoxLayout* ofilesLayout = new QVBoxLayout;
    QHBoxLayout* buttonLayout = new QHBoxLayout;

    mainLayout->addLayout(editLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    editLayout->addLayout(tkInfoLayout, 0, 0);
    editLayout->addLayout(paramsLayout, 0, 1);
    editLayout->addLayout(ifilesLayout, 1, 0);
    editLayout->addLayout(ofilesLayout, 1, 1);

    //------------------------------------------------------------
    // INSTANTIATE ALL GRAPHIC ELEMENTS IN THE INTERFACE
    //------------------------------------------------------------

    QLabel* task_info_label   = new QLabel(tr("General info"));
    QLabel* whitespace_label  = new QLabel(tr(" "));
    QLabel* id_label          = new QLabel(tr("Identifier"));
    QLabel* description_label = new QLabel(tr("Description"));
    QLabel* executable_label  = new QLabel(tr("Executable"));

    QLabel* params_label      = new QLabel(tr("Parameters"));
    QLabel* ifiles_label      = new QLabel(tr("Input files"));
    QLabel* ofiles_label      = new QLabel(tr("Output files"));


    id_text_                  = new QLineEdit;
    description_text_         = new QLineEdit;
    executable_text_          = new QLineEdit;

    params_                   = new ToolkitEditor_parameter_list_widget(1);
    params_->set_parameter_map(parameter_map_);

    ifiles_                   = new ToolkitEditor_file_list_widget();
    ifiles_->set_file_types(filetype_list_);

    ofiles_                   = new ToolkitEditor_file_list_widget();
    ofiles_->set_file_types(filetype_list_);

    save_button_              = new QPushButton(tr("Save"));
    cancel_button_            = new QPushButton(tr("Cancel"));

    //------------------------------------------------------------
    // SET SCREEN VALUES
    //------------------------------------------------------------

    // Set the values of the several items shown on the screen.

    id_text_->setText(QString::fromStdString(task_.id));
    description_text_->setText(QString::fromStdString(task_.description));
    executable_text_->setText(QString::fromStdString(task_.name_of_executable));

    map<string, toolkit_parameter>::iterator finder;

    for (size_t i = 0; i < task.parameters.size(); i++)
    {
      finder = parameter_map_.find(task.parameters[i]);
      if (finder != parameter_map_.end())
      {
        toolkit_parameter par;
        par = finder->second;
        params_->add(par);
      }
    }

    for (size_t i = 0; i < task.input_files.size(); i++)
    {
      toolkit_file file;

      file = task.input_files[i];
      ifiles_->add(file);
    }

    for (size_t i = 0; i < task.output_files.size(); i++)
    {
      toolkit_file file;

      file = task.output_files[i];
      ofiles_->add(file);
    }

    //------------------------------------------------------------
    // BUILD THE INTERFACE: INSERT THE DIFFERENT ELEMENTS INTO
    // THEIR RESPECTIVE LAYOUTS.
    //------------------------------------------------------------

    tkInfoLayout->addWidget(task_info_label);
    tkInfoLayout->addWidget(whitespace_label);
    tkInfoLayout->addLayout(gInfoLayout);
    tkInfoLayout->addStretch();

    gInfoLayout->addWidget(id_label,          0, 0);
    gInfoLayout->addWidget(id_text_,          0, 1);
    gInfoLayout->addWidget(description_label, 1, 0);
    gInfoLayout->addWidget(description_text_, 1, 1);
    gInfoLayout->addWidget(executable_label,  2, 0);
    gInfoLayout->addWidget(executable_text_,  2, 1);

    paramsLayout->addWidget(params_label);
    paramsLayout->addWidget(params_);

    ifilesLayout->addWidget(ifiles_label);
    ifilesLayout->addWidget(ifiles_);

    ofilesLayout->addWidget(ofiles_label);
    ofilesLayout->addWidget(ofiles_);

    // Control buttons (save & cancel)

    buttonLayout->addWidget(cancel_button_);
    buttonLayout->addStretch();
    buttonLayout->addWidget(save_button_);

    // Set window title.

    setWindowTitle(tr("Task data"));

    //------------------------------------------------------------
    //
    // SET UP SIGNALS AND ACTIONS
    //
    //-------------------------------------------------------------

    // Buttons save and close.

    connect(save_button_  , SIGNAL(clicked()),
            this ,          SLOT  (save_clicked()));

    connect(cancel_button_, SIGNAL(clicked()),
            this ,          SLOT  (close()));

    // Value of the identifier text field changed.

    connect(id_text_,       SIGNAL(textChanged(const QString&)),
            this,           SLOT  (id_text_changed(const QString&)));
  }
}


bool
ToolkitEditor_single_task_widget::
validate
(void)
{
  {
    set<string>               checker;
    QList<toolkit_file>*      files;
    set<string>::iterator     finder;
    QString                   message;
    int                       n_ifiles;
    int                       n_ofiles;
    int                       n_params;
    QList<toolkit_parameter>* params;
    bool                      success;

    message = "The following errors must be solved before saving the task:\n\n";
    success  = true;

    // First, check the text fields.

    if (id_text_->text().isEmpty())
    {
      success = false;
      message += "  The task identifier may not be empty.\n";
    }

    if (description_text_->text().isEmpty())
    {
      success = false;
      message += "  The description may not be empty.\n";
    }

    if (executable_text_->text().isEmpty())
    {
      success = false;
      message += "  The name of the task's executable may not be empty.\n";
    }

    // Check the AMOUNT of parameters, input and output files.

    n_params = params_->n_elements();
    n_ifiles = ifiles_->n_elements();
    n_ofiles = ofiles_->n_elements();

    if ((n_params == 0) & (n_ifiles == 0))
    {
      success = false;
      message += "  The task must include at least one parameter or INPUT file.\n";
    }

    if (n_ofiles == 0)
    {
      success = false;
      message += "  The task must include at least one OUTPUT file.\n";
    }

    // Look for repeated parameter / input / output files identifiers.

    checker.clear();

    params = params_->get_list();
    for (int i = 0; i < params->size(); i++)
    {
      string id = params->at(i).id;

      finder = checker.find(id);
      if (finder != checker.end())
      {
        success = false;
        message += "  Repeated parameter identifier /options file label: ";
        message += QString::fromStdString(id);
        message += "\n";
      }
      else
      {
        checker.insert(id);
      }
    }

    files = ifiles_->get_list();
    for (int i = 0; i < files->size(); i++)
    {
      string label = files->at(i).options_file_label;

      finder = checker.find(label);
      if (finder != checker.end())
      {
        success = false;
        message += "  Repeated parameter identifier /options file label: ";
        message += QString::fromStdString(label);
        message += "\n";
      }
      else
      {
        checker.insert(label);
      }
    }

    files = ofiles_->get_list();
    for (int i = 0; i < files->size(); i++)
    {
      string label = files->at(i).options_file_label;

      finder = checker.find(label);
      if (finder != checker.end())
      {
        success = false;
        message += "  Repeated parameter identifier /options file label: ";
        message += QString::fromStdString(label);
        message += "\n";
      }
      else
      {
        checker.insert(label);
      }
    }

    if (!success)
    {
      QMessageBox msgBox;

      msgBox.setText(message);
      msgBox.setWindowTitle("Incomplete task data");
      msgBox.setIcon(QMessageBox::Warning);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
    }

    return success;
  }
}
