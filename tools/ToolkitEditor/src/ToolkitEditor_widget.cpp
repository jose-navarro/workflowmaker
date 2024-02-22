/** \file ToolkitEditor_widget.cpp
\brief Implementation file for ToolkitEditor_widget.hpp
*/

#include "ToolkitEditor_widget.hpp"

bool
ToolkitEditor_widget::
check_completeness
(void)
{
  bool result;

  {
    bool result1, result2, result3, result4;

    error_list_.clear();

    result1 = tab_description_->check_completeness();
    if (!result1)
    {
      QVector<QString> errors = tab_description_->error_list();
      error_list_ += errors;
    }

    result2 = tab_parameters_->check_completeness();
    if (!result2)
    {
      QVector<QString> errors = tab_parameters_->error_list();
      error_list_ += errors;
    }

    result3 = tab_filetypes_->check_completeness();
    if (!result3)
    {
      QVector<QString> errors = tab_filetypes_->error_list();
      error_list_ += errors;
    }

    result4 = tab_tasks_->check_completeness();
    if (!result4)
    {
      QVector<QString> errors = tab_tasks_->error_list();
      error_list_ += errors;
    }

    result = result1 & result2 & result3 & result4;
  }

  return result;
}

void
ToolkitEditor_widget::
copy_toolkit_values
(void)
{
  { 
    tab_description_->copy_toolkit_values(toolkit_);
    tab_parameters_->copy_toolkit_values(toolkit_);
    tab_filetypes_->copy_toolkit_values(toolkit_);
    tab_tasks_->copy_toolkit_values(toolkit_);
  }
}

#ifdef __GNUC__

string
ToolkitEditor_widget::
get_executable_path
(void)
{
  {
    char result[PATH_MAX] = {};
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    const char *path;
    if (count != -1)
    {
      path = dirname(result);
      return string(path);
    }

    return "";
  }
}
#else
string
ToolkitEditor_widget::
get_executable_path
(void)
{
  {
    char buffer[MAX_PATH];

    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");

    return std::string(buffer).substr(0, pos);
  }
}
#endif

void
ToolkitEditor_widget::
on_change_tab
(int new_tab)
{
  switch (new_tab)
  {
    case 3: // Tasks.

      // Get the list of available parameters.

      QList<toolkit_parameter>* current_parameters;
      current_parameters = tab_parameters_->get_current_parameter_list();
      tab_tasks_->set_current_parameters(current_parameters);

      // Get the list of available file types.

      QList<toolkit_file_type>* current_file_types;
      current_file_types = tab_filetypes_->get_current_file_types_list();
      tab_tasks_->set_current_file_types(current_file_types);

      break;

    default:

      break;
  }
}

void
ToolkitEditor_widget::
on_load_toolkit_file
(void)
{
  {
    string         error_message;
    QStringList    file_names;
    QString        path;
    QString        schema_file;
    int            status;
    toolkit_parser tk_parser;

    // First, get the name of the toolkit file to load.

    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Select an existing toolkit file"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Toolkit file (*.xml)");

    if (!dialog.exec()) return;

    file_names = dialog.selectedFiles();

    path = file_names[0];

    //
    // Get the path to the schema defining the toolkit files.
    // It resides in the same folder than our executable file.
    //

    schema_file = QString::fromStdString(get_executable_path() + "/toolkit.xsd");

    // Check the validity of the input file using the schema.

    status = tk_parser.set_schema(schema_file);

    if (!status)
    {
      vector<string> errors = tk_parser.error_list();

      error_message  = "Unable to load the schema definition for toolkit files.\n";
      error_message += "Maybe reinstalling the application would solve the problem.\n";
      error_message += "The precise error messages detected are listed below:\n";

      for (size_t i = 0; i < errors.size(); i++)
      {
        error_message = error_message + "  " + errors[i] + "\n";
      }

      QMessageBox msgBox;
      msgBox.setText(QString::fromStdString(error_message));
      msgBox.setWindowTitle("Error(s) loading the toolkit schema");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
      return;
    }

    // Try to parse the toolkit file, since it could be erroneous.

    status = tk_parser.parse(path, toolkit_);
    if (!status)
    {
      vector<string> error_list;

      //
      // We've got problems when parsing the toolkit file.
      // Notify about the issue and go away!
      //

      error_list = tk_parser.error_list();

      error_message  = "Errors detected when loading the toolkit file.\n";
      error_message += "See the list of detected errors below:\n";

      for (size_t i = 0; i < error_list.size(); i++)
      {
        error_message = error_message + "  " + error_list[i] + "\n";
      }

      QMessageBox msgBox;
      msgBox.setText(QString::fromStdString(error_message));
      msgBox.setWindowTitle("Error(s) reading the toolkit file");
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
      return;
    }

    //
    // Well, at this point the toolkit file has been successfully
    // loaded. Let's copy its values into the widget's fields.
    //

    tab_description_->set_values(toolkit_);
    tab_parameters_->set_values(toolkit_);
    tab_filetypes_->set_values(toolkit_);
    tab_tasks_->set_values(toolkit_);
  }
}

void
ToolkitEditor_widget::
on_save_toolkit_file
(void)
{
  {
    bool              correct;
    QString           destFile;
    QString           errors;
    QStringList       file_names;
    QString           message;
    QMessageBox       msgBox;

    //
    // First of all, retrieve the values set on the screen
    // This may produce errors!!!
    //

    correct = check_completeness();

    if (!correct)
    {
      errors = "";
      for (int i = 0; i < error_list_.size(); i++)
        errors = errors + error_list_[i] + "\n";

      reset_error_messages();

      msgBox.setWindowTitle(tr("Can't save the toolkit file: missing data"));
      msgBox.setText(errors);
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);

      msgBox.exec();

      return;
    }

    //
    // Since all data are present, get all the values on the screen.
    // This must be done now, since some further checks go against the
    // toolkit file object; therefore, it must be updated.
    //

    copy_toolkit_values();

    // And now, check that such information is correct.

    correct = validate_semantics();
    if (!correct)
    {
      errors = "";
      for (int i = 0; i < error_list_.size(); i++)
        errors = errors + error_list_[i] + "\n";

      reset_error_messages();

      reset_error_messages();

      msgBox.setWindowTitle(tr("Can't save the toolkit file: invalid data"));
      msgBox.setText(errors);
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);

      msgBox.exec();

      return;
    }

    //
    // Get the name of the output options file name.
    //

    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Name of the output toolkit file"));
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter("Toolkit files (*.xml)");

    if (!dialog.exec()) return;

    file_names = dialog.selectedFiles();

    destFile = file_names[0];

    QFileInfo finfo2(destFile);
    QString suffix = finfo2.suffix();

    if (suffix.compare("xml", Qt::CaseInsensitive) != 0)
    {
      destFile += ".xml";
    }

    //
    // OK, let's write the toolkit file.
    //

    toolkit_writer tk_writer;
    if (!tk_writer.write(toolkit_, destFile))
    {
      string         error_message;
      vector<string> error_list;

      //
      // We've got problems when trying to write the toolkit file.
      // Notify about the issue and go away!
      //


      error_list = tk_writer.error_list();

      error_message  = "Errors detected when writing the toolkit file.\n";
      error_message += "See the list of detected errors below:\n";

      for (size_t i = 0; i < error_list.size(); i++)
      {
        error_message = error_message + "  " + error_list[i] + "\n";
      }

      msgBox.setWindowTitle(tr("Error writing a toolkit file"));
      msgBox.setText(errors);
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);

      msgBox.exec();

      return;
    }

    // Notify the user that (s)he's got a brand new options file.

    message  = "Toolkit file successfully saved.";

    msgBox.setWindowTitle(tr("File operation finished"));
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.exec();

  }
}


void
ToolkitEditor_widget::
reset_error_messages
(void)
{
  {
    // Reset all the error messages for all tabs.

    tab_description_->reset_error_list();
    tab_parameters_->reset_error_list();
    tab_filetypes_->reset_error_list();
    tab_tasks_->reset_error_list();
  }
}

ToolkitEditor_widget::
ToolkitEditor_widget
(QWidget* parent)
    : QWidget(parent)
{
  {
    string               sversion;
    QString              title;
    QString              version;

    // Get the WorkflowMaker package's current version.

    sversion = version_string();
    version  = QString::fromStdString(sversion);

    //
    // Initialize.
    //

    title   = "ToolkitEditor " + version;

    //
    // Create the different graphic elements.
    //

    //
    // The tabs with the tookit's description, parameters,
    // file types and tasks.
    //

    tabs_            = new QTabWidget;
    tab_description_ = new ToolkitEditor_description_widget();
    tab_parameters_  = new ToolkitEditor_parameters_widget();
    tab_filetypes_   = new ToolkitEditor_filetypes_widget();
    tab_tasks_       = new ToolkitEditor_tasks_widget();

    tabs_->addTab(tab_description_, tr("General info"));
    tabs_->addTab(tab_parameters_,  tr("Parameters"));
    tabs_->addTab(tab_filetypes_,   tr("File types"));
    tabs_->addTab(tab_tasks_,       tr("Tasks"));

    tab_description_->setMinimumWidth (700);
    tab_parameters_->setMinimumWidth  (700);
    tab_filetypes_->setMinimumWidth   (700);
    tab_tasks_->setMinimumWidth       (700);

    // Control buttons.

    QHBoxLayout* control_button_layout = new QHBoxLayout;
    control_button_cancel_  = new QPushButton(tr("Quit"));
    control_button_load_    = new QPushButton(tr("Load toolkit"));
    control_button_save_    = new QPushButton(tr("Save toolkit"));

    control_button_layout->addWidget(control_button_cancel_);
    control_button_layout->addStretch();
    control_button_layout->addWidget(control_button_load_);
    control_button_layout->addWidget(control_button_save_);

    // Tabs.

    QHBoxLayout* tabs_layout = new QHBoxLayout;
    tabs_layout->addWidget(tabs_);


    // Overall layout.

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(tabs_layout);
    mainLayout->addLayout(control_button_layout);
    setLayout(mainLayout);

    // Window title.

    setWindowTitle(title);

    //
    // Connect signals and slots.
    //

    // Close the app.

    connect(control_button_cancel_, SIGNAL (clicked()),
            this,                   SLOT   (close()));

    // Load toolkit file.

    connect(control_button_load_,   SIGNAL (clicked()),
            this,                   SLOT   (on_load_toolkit_file()));

    // Save toolkit file.

    connect(control_button_save_,   SIGNAL (clicked()),
            this,                   SLOT   (on_save_toolkit_file()));

    // Change tabs.

    connect(tabs_,                  SIGNAL(currentChanged(int)),
            this,                   SLOT  (on_change_tab(int)));

    // Set cross-pointers.

    tab_parameters_->set_task_manager(tab_tasks_);
    tab_filetypes_->set_task_manager(tab_tasks_);

    // Window icon

    setWindowIcon(QIcon(":/resources/ToolkitEditor.png"));

  }
}

ToolkitEditor_widget::
~ToolkitEditor_widget
(void)
{
  {
    // Intentionally left blank.
  }
}

bool
ToolkitEditor_widget::
validate_semantics
(void)
{
  bool result;

  {
    bool result1, result2, result3, result4;

    error_list_.clear();

    // Validate the semantics for the whole set of tabs.

    result1 = tab_description_->validate_semantics();
    if (!result1)
    {
      QVector<QString> errors = tab_description_->error_list();
      error_list_ += errors;
    }

    result2 = tab_parameters_->validate_semantics();
    if (!result2)
    {
      QVector<QString> errors = tab_parameters_->error_list();
      error_list_ += errors;
    }

    result3 = tab_filetypes_->validate_semantics();
    if (!result3)
    {
      QVector<QString> errors = tab_filetypes_->error_list();
      error_list_ += errors;
    }

    result4 = tab_tasks_->validate_semantics();
    if (!result4)
    {
      QVector<QString> errors = tab_tasks_->error_list();
      error_list_ += errors;
    }

    // Overall result.

    result = result1 & result2 & result3 & result4;
  }

  return result;
}

string
ToolkitEditor_widget::
version_string
(void)
{
  string version;

  {
    ifstream version_file;
    string   version_file_name;

    //
    // Get the path of the file containing the version string.
    // It is (should be) located in the application's directory.
    //

    version_file_name = get_executable_path()
                        + "/workflowmaker_version.txt";

    //
    // Open and read the version file. Should this file, return
    // 'Unknown WorkflowMaker version' as the result.
    //

    version = "'Unknown WorkflowMaker version'";

    version_file.open(version_file_name);

    if (version_file.is_open())
    {
      getline(version_file, version);
      version_file.close();
    }
  }

  return version;
}
