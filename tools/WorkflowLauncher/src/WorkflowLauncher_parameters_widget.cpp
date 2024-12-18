/** \file WorkflowLauncher_parameters_widget.cpp
\brief Implementation file for WorkflowLauncher_parameters_widget.hpp
*/

#include "WorkflowLauncher_parameters_widget.hpp"

bool
WorkflowLauncher_parameters_widget::
check_completeness
(void)
{
  bool result;
  {
    result = true;

    for (size_t i = 0; i < wparams_.size(); i++)
      result = result & wparams_[i]->check_completeness();

    if (!result)
    {
      QString message = "Parameters: one or more values have not been set.";
      error_list_.push_back(message);
    }
  }

  return result;
}

QVector<QString>
WorkflowLauncher_parameters_widget::
error_list
(void)
{
  {
    return error_list_;
  }
}

vector<WLParameter>
WorkflowLauncher_parameters_widget::
get_current_parameter_list
(void)
{
  {
    for (size_t i = 0; i < wparams_.size(); i++)
    {
      single_parameter_widget* pw = wparams_[i];
      string val = pw->get_value().toStdString();
      params_[i].value = val;
    }

    return params_;
  }
}

void
WorkflowLauncher_parameters_widget::
reset_error_list
(void)
{
  {
    error_list_.clear();
  }
}

void
WorkflowLauncher_parameters_widget::
set_values
(vector<WLParameter>& params)
{
  {
    //
    // First of all, remove anything that could have been loaded
    // prior to this call. Then copy the input parameter to our
    // own member.
    //

    params_.clear();
    wparams_.clear();

    params_ = params;

    // Add the new data.

    //vector<QColor> colors;
    //colors.push_back(Qt::white);
    //colors.push_back(Qt::lightGray);

    for (size_t i = 0; i < params_.size(); i++)
    {
      QColor tc;
      QColor bc;

      // Alternate the colors for each parameter to increase visibility.

      palette_utils::text_and_bg_colors(i, tc, bc);

      // Create the new parameter widget.

      single_parameter_widget* pw = new single_parameter_widget(tc, bc, params_[i]);
      container_layout_->addWidget(pw);

      wparams_.push_back(pw);
    }

    container_layout_->addStretch();
  }
}

WorkflowLauncher_parameters_widget::
WorkflowLauncher_parameters_widget
(QWidget* parent)
  : QWidget(parent)
{
  {

    params_.clear();
    wparams_.clear();

    QScrollArea* scroll_area       = new QScrollArea(this);
    QVBoxLayout* v_layout          = new QVBoxLayout(this);
    QWidget*     container         = new QWidget();
                 container_layout_ = new QVBoxLayout();

    container->setLayout(container_layout_);

    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(container);
    scroll_area->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    v_layout->setContentsMargins(0, 0, 0, 0);
    v_layout->addWidget(scroll_area);

  }
}

WorkflowLauncher_parameters_widget::
~WorkflowLauncher_parameters_widget
(void)
{
  {
    // Intentionally left blank.
  }
}


