/** \file task_list_widget.cpp
\brief Implementation file for task_list_widget.hpp.
*/

#include "task_list_widget.hpp"

void
task_list_widget::
add
(toolkit_task& task)
{
  {
    model_->append_row(task);
    table_view_->resizeColumnsToContents();
  }
}

int
task_list_widget::
n_elements
(void)
const
{
  {
    return tasks_.size();
  }
}

void
task_list_widget::
remove_all
(void)
{
  {
    model_->remove_all_rows();
  }
}

void
task_list_widget::
row_selected
(int rowIndex)
{
  {
    cur_row_ = rowIndex;
    emit (selected_row(cur_row_));
  }
}

task_list_widget::
task_list_widget
(QWidget *parent) : QWidget(parent)
{
  {
    // Initialize some members.

    cur_row_ = -1;

    // Create the files model that will backing the table view.

    model_ = new task_table_model(&tasks_, this);

    // Set up the table view for the points.

    table_view_ = new task_table_view(this);
    table_view_->setModel(model_);
    table_view_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_view_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_view_->verticalHeader()->hide();

    // Lay out the toolbar and table view.

    QGridLayout *layout = new QGridLayout;
    QLabel* banner = new QLabel("Available tasks");
    layout->addWidget(banner,      0, 0, Qt::AlignLeft);
    layout->addWidget(table_view_, 1, 0);
    setLayout(layout);

    // Connect events coming from the table view.

    connect(table_view_, SIGNAL(row_selected(int)), this, SLOT(row_selected(int)));

    // Set the window title.

    setWindowTitle(tr("Tasks"));
  }
}
