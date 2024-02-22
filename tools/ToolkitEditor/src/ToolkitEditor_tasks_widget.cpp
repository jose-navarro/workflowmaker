/** \file ToolkitEditor_tasks_widget.cpp
\brief Implementation file for ToolkitEditor_tasks_widget.hpp
*/

#include "ToolkitEditor_tasks_widget.hpp"

bool
ToolkitEditor_tasks_widget::
check_completeness
(void)
{
  bool result;

  {
    result = (task_list_->n_elements() > 0);
    if (!result)
    {
      QString message = "Tasks: the list of tasks may not be empty.";
      error_list_.push_back(message);
    }
  }

  return result;
}

void
ToolkitEditor_tasks_widget::
copy_toolkit_values
(toolkit& tk)
{
  {
    QList<toolkit_task>* tasks;

    tasks = task_list_->get_list();
    tk.tasks.clear();

    for (int i = 0; i < tasks->size(); i++)
    {
      toolkit_task tsk = tasks->at(i);
      tk.tasks.push_back(tsk);
    }
  }
}

QVector<QString>
ToolkitEditor_tasks_widget::
error_list
(void)
{
  {
    return error_list_;
  }
}

QStringList
ToolkitEditor_tasks_widget::
list_of_tasks_for_filetype
(string& filetype)
{
  {
    return task_list_->list_of_tasks_for_filetype(filetype);
  }
}


QStringList
ToolkitEditor_tasks_widget::
list_of_tasks_for_parameter
(string& parameter)
{
  {
    return task_list_->list_of_tasks_for_parameter(parameter);
  }
}

void
ToolkitEditor_tasks_widget::
reset_error_list
(void)
{
  {
    error_list_.clear();
  }
}

void
ToolkitEditor_tasks_widget::
set_current_file_types
(QList<toolkit_file_type>* cur_ftypes)
{
  {
    filetypes_list_ = cur_ftypes;
    task_list_->set_file_types(filetypes_list_);
  }
}

void
ToolkitEditor_tasks_widget::
set_current_parameters
(QList<toolkit_parameter>* cur_pars)
{
  {
    parameter_map_.clear();

    for (int i = 0; i < cur_pars->size(); i++)
    {
      toolkit_parameter p = cur_pars->at(i);
      parameter_map_[p.id] = p;
    }

    task_list_->set_parameters (parameter_map_);
  }
}

void
ToolkitEditor_tasks_widget::
set_values
(toolkit& toolkit)
{
  {
    //
    // First of all, remove anything that could have been loaded
    // prior to this call.
    //

    task_list_->remove_all();

    //
    // Build the map with the parameters in the toolkit.
    // Note that it may be empty.
    //

    parameter_map_.clear();

    for (size_t i = 0; i < toolkit.parameters.size(); i++)
    {
      toolkit_parameter p = toolkit.parameters[i];
      parameter_map_[p.id] = p;
    }

    //
    // Tell the list of tasks what is the set of available
    // parameters.
    //

    task_list_->set_parameters (parameter_map_);

    // Add the tasks to the list of tasks.

    for (size_t i = 0; i < toolkit.tasks.size(); i++)
    {
      toolkit_task tsk = toolkit.tasks[i];
      task_list_->add(tsk);
    }
    return;
  }
}

ToolkitEditor_tasks_widget::
ToolkitEditor_tasks_widget
(QWidget*                   parent)
    : QWidget(parent)
{
  {
    //
    // We only have a single widget in our interface.
    //

    task_list_ = new ToolkitEditor_task_list_widget();

    // Overall layout.

    QVBoxLayout* stretch_layout = new QVBoxLayout;
    stretch_layout->addStretch();

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(task_list_);

    main_layout->addLayout(stretch_layout);

    setLayout(main_layout);

    // Window title.

    setWindowTitle("Tasks");
  }
}

ToolkitEditor_tasks_widget::
~ToolkitEditor_tasks_widget
(void)
{
  {
    // Intentionally left blank.
  }
}

bool
ToolkitEditor_tasks_widget::
validate_semantics
(void)
{
  {
    toolkit_task         tsk;
    QList<toolkit_task>* tasks;
    set<string>          tasks_set;

    // Get the list of tasks from our task list widget.

    tasks = task_list_->get_list();

    // Check that there is as least ONE task in such list.

    if (tasks->size() == 0)
    {
      QString message = "Tasks: the list of tasks may not be empty.";
      error_list_.push_back(message);

      return false;
    }

    // Look for duplicates.

    tasks_set.clear();

    tasks_set.insert(tasks->at(0).id);

    for (int i = 1; i < tasks->size(); i++)
    {
      set<string>::iterator finder;

      tsk = tasks->at(i);
      finder = tasks_set.find(tsk.id);
      if (finder != tasks_set.end())
      {
        QString message;
        message = "Tasks: repeated task identifier: " + QString::fromStdString(tsk.id);
        error_list_.push_back(message);

        return false; // Repeated tasks.
      }

      tasks_set.insert(tsk.id);
    }

    return true;
  }
}

