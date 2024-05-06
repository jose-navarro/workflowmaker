/** \file ToolkitEditor_parameters_widget.cpp
\brief Implementation file for ToolkitEditor_parameters_widget.hpp
*/

#include "ToolkitEditor_parameters_widget.hpp"

bool
ToolkitEditor_parameters_widget::
check_completeness
(void)
{
  bool result;

  {
    result = (parameter_list_->n_elements() > 0);
    if (!result)
    {
      QString message = "Parameters: the list of parameters may not be empty.";
      error_list_.push_back(message);
    }
  }

  return result;
}

void
ToolkitEditor_parameters_widget::
copy_toolkit_values
(toolkit& tk)
{
  {
    QList<toolkit_parameter>* params;

    params = parameter_list_->get_list();
    tk.parameters.clear();

    for (int i = 0; i < params->size(); i++)
    {
      toolkit_parameter p = params->at(i);
      tk.parameters.push_back(p);
    }
  }
}

QVector<QString>
ToolkitEditor_parameters_widget::
error_list
(void)
{
  {
    return error_list_;
  }
}

QList<toolkit_parameter>*
ToolkitEditor_parameters_widget::
get_current_parameter_list
(void)
{
  {
    return parameter_list_->get_list();
  }
}

void
ToolkitEditor_parameters_widget::
reset_error_list
(void)
{
  {
    error_list_.clear();
  }
}

void
ToolkitEditor_parameters_widget::
set_task_manager
(ToolkitEditor_tasks_widget* task_mgr)
{
  {
    parameter_list_->set_task_manager(task_mgr);
  }
}

void
ToolkitEditor_parameters_widget::
set_values
(toolkit& toolkit)
{
  {
    //
    // First of all, remove anything that could have been loaded
    // prior to this call.
    //

    parameter_list_->remove_all();

    // Add the new data.

    for (size_t i = 0; i < toolkit.parameters.size(); i++)
    {
      toolkit_parameter p = toolkit.parameters[i];
      parameter_list_->add(p);
    }
    return;
  }
}

ToolkitEditor_parameters_widget::
ToolkitEditor_parameters_widget
(QWidget*                   parent)
    : QWidget(parent)
{
  {
    //
    // We only have a single widget in our interface.
    //

    parameter_list_ = new ToolkitEditor_parameter_list_widget(0);

    // Overall layout.

    QVBoxLayout* stretch_layout = new QVBoxLayout;
    stretch_layout->addStretch();

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(parameter_list_);

    main_layout->addLayout(stretch_layout);

    setLayout(main_layout);

    // Window title.

    setWindowTitle("Parameters");
  }
}

ToolkitEditor_parameters_widget::
~ToolkitEditor_parameters_widget
(void)
{
  {
    // Intentionally left blank.
  }
}

bool
ToolkitEditor_parameters_widget::
validate_semantics
(void)
{
  {
    toolkit_parameter         p;
    QList<toolkit_parameter>* params;
    set<string>               params_set;

    // Get the list of parameters from our parameter list widget.

    params = parameter_list_->get_list();

    // Check that there is as least ONE parameter in such list.

    if (params->size() == 0)
    {
      QString message = "Parameters: the list of parameters may not be empty.";
      error_list_.push_back(message);

      return false;
    }

    // Look for duplicates.

    params_set.clear();

    params_set.insert(params->at(0).id);

    for (int i = 1; i < params->size(); i++)
    {
      set<string>::iterator finder;

      p = params->at(i);
      finder = params_set.find(p.id);
      if (finder != params_set.end())
      {
        QString message;
        message = "Parameters: repeated parameter identifier: " + QString::fromStdString(p.id);
        error_list_.push_back(message);

        return false; // Repeated parameters.
      }

      params_set.insert(p.id);
    }

    return true;
  }
}

