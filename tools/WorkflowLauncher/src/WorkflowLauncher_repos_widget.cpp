/** \file WorkflowLauncher_repos_widget.cpp
\brief Implementation file for WorkflowLauncher_repos_widget.hpp
*/

#include "WorkflowLauncher_repos_widget.hpp"

bool
WorkflowLauncher_repos_widget::
check_completeness
(void)
{
  bool result;
  {
    result = true;

    for (size_t i = 0; i < wrepos_.size(); i++)
      result = result & wrepos_[i]->check_completeness();

    if (!result)
    {
      QString message = "Repositories: one or more paths have not been set.";
      error_list_.push_back(message);
    }
  }

  return result;
}

QVector<QString>
WorkflowLauncher_repos_widget::
error_list
(void)
{
  {
    return error_list_;
  }
}

vector<WLRepository>
WorkflowLauncher_repos_widget::
get_current_repo_list
(void)
{
  {
    for (size_t i = 0; i < wrepos_.size(); i++)
    {
      single_repository_widget* rw = wrepos_[i];
      string val = rw->get_value().toStdString();

      repos_[i].path = val;
    }

    return repos_;
  }
}

void
WorkflowLauncher_repos_widget::
reset_error_list
(void)
{
  {
    error_list_.clear();
  }
}

void
WorkflowLauncher_repos_widget::
set_values
(vector<WLRepository>& repos)
{
  {
    //
    // First of all, remove anything that could have been loaded
    // prior to this call. Then copy the input parameter to our
    // own member.
    //

    repos_.clear();
    wrepos_.clear();

    repos_ = repos;

    // Add the new data.

    //vector<QColor> colors;
    //colors.push_back(Qt::white);
    //colors.push_back(Qt::lightGray);

    for (size_t i = 0; i < repos_.size(); i++)
    {
      QColor tc;
      QColor bc;

      // Alternate the colors for each parameter to increase visibility.

      palette_utils::text_and_bg_colors(i, tc, bc);

      // Create the new repository widget.

      single_repository_widget* rw = new single_repository_widget(tc, bc, repos_[i]);
      container_layout_->addWidget(rw);

      wrepos_.push_back(rw);
    }

    container_layout_->addStretch();
  }
}

WorkflowLauncher_repos_widget::
WorkflowLauncher_repos_widget
(QWidget* parent)
  : QWidget(parent)
{
  {

    repos_.clear();
    wrepos_.clear();

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

WorkflowLauncher_repos_widget::
~WorkflowLauncher_repos_widget
(void)
{
  {
    // Intentionally left blank.
  }
}


