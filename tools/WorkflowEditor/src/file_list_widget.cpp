/** \file file_list_widget.cpp
\brief Implementation file for task_list_widget.hpp.
*/

#include "file_list_widget.hpp"

void
file_list_widget::
add
(toolkit_file& file)
{
  {
    model_->append_row(file);
    table_view_->resizeColumnsToContents();
  }
}

file_list_widget::
file_list_widget
(QString& title,
 QWidget* parent) : QWidget(parent)
{
  {
    // Initialize some members.

    cur_row_ = -1;

    // Create the files model that will backing the table view.

    model_ = new file_table_model(&files_, this);

    // Set up the table view for the points.

    table_view_ = new file_table_view(this);
    table_view_->setModel(model_);
    table_view_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_view_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_view_->verticalHeader()->hide();

    // Lay out the toolbar and table view.

    QGridLayout *layout = new QGridLayout;
    QLabel* banner = new QLabel(title);
    layout->addWidget(banner,      0, 0, Qt::AlignLeft);
    layout->addWidget(table_view_, 1, 0);
    setLayout(layout);

    // Set the window title.

    setWindowTitle(title);
  }
}

int
file_list_widget::
n_elements
(void)
const
{
  {
    return files_.size();
  }
}

void
file_list_widget::
remove_all
(void)
{
  {
    model_->remove_all_rows();
  }
}
