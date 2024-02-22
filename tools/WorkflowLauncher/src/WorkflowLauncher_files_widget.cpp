/** \file WorkflowLauncher_files_widget.cpp
\brief Implementation file for WorkflowLauncher_files_widget.hpp
*/

#include "WorkflowLauncher_files_widget.hpp"

bool
WorkflowLauncher_files_widget::
check_completeness
(void)
{
  bool result;
  {
    result = true;

    for (size_t i = 0; i < wfiles_.size(); i++)
      result = result & wfiles_[i]->check_completeness();

    if (!result)
    {
      QString message = "Files: one or more file names have not been set.";
      error_list_.push_back(message);
    }
  }

  return result;
}

QVector<QString>
WorkflowLauncher_files_widget::
error_list
(void)
{
  {
    return error_list_;
  }
}

vector<WLFile>
WorkflowLauncher_files_widget::
get_current_file_list
(void)
{
  {
    for (size_t i = 0; i < wfiles_.size(); i++)
    {
      single_file_widget* fw = wfiles_[i];
      string val = fw->get_value().toStdString();
      files_[i].filename = val;
    }

    return files_;
  }
}

void
WorkflowLauncher_files_widget::
reset_error_list
(void)
{
  {
    error_list_.clear();
  }
}

void
WorkflowLauncher_files_widget::
set_values
(vector<WLFile>& files)
{
  {
    //
    // First of all, remove anything that could have been loaded
    // prior to this call. Then copy the input parameter to our
    // own member.
    //

    files_.clear();
    wfiles_.clear();

    files_ = files;

    //
    // Add the new data. Alternate the colors for each parameter to
    // increase visibility.
    //

    vector<QColor> colors;
    colors.push_back(Qt::white);
    colors.push_back(Qt::lightGray);

    for (size_t i = 0; i < files_.size(); i++)
    {
      QColor bc = colors[i%2];
      single_file_widget* rw = new single_file_widget(bc, files_[i]);
      container_layout_->addWidget(rw);

      wfiles_.push_back(rw);
    }

    container_layout_->addStretch();
  }
}

WorkflowLauncher_files_widget::
WorkflowLauncher_files_widget
(QWidget* parent)
  : QWidget(parent)
{
  {

    files_.clear();
    wfiles_.clear();

    QScrollArea* scroll_area       = new QScrollArea(this);
    QVBoxLayout* v_layout          = new QVBoxLayout(this);
    QWidget*     container         = new QWidget();
                 container_layout_ = new QVBoxLayout();

    container->setLayout(container_layout_);

    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(container);
    scroll_area->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    v_layout->setMargin(0);
    v_layout->addWidget(scroll_area);

  }
}

WorkflowLauncher_files_widget::
~WorkflowLauncher_files_widget
(void)
{
  {
    // Intentionally left blank.
  }
}


