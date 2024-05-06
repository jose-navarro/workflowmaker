/** \file task_info_panel.cpp
  \brief Implementation file for task_info_panel.hpp.
*/


#include "task_info_panel.hpp"

void
task_info_panel::
selected_row
(int row)
{
  {
    // Reset the contents of the input and output files panels.

    files_input_->remove_all();
    files_output_->remove_all();

    // Add the input files for the new task.

    for (size_t i = 0; i < tk_.tasks[row].input_files.size(); i++)
    {
      files_input_->add(tk_.tasks[row].input_files[i]);
    }

    // Add the output files for the new task.

    for (size_t i = 0; i < tk_.tasks[row].output_files.size(); i++)
    {
      files_output_->add(tk_.tasks[row].output_files[i]);
    }

    // Tell our clients that a new row has been selected.

    emit row_selected(row);
  }
}

void
task_info_panel::
set_toolkit
(toolkit& tk)
{
  {
    // Copy the toolkit to our own member.

    tk_ = tk;

    // Set the tasks, input & output files panels.

    for (size_t i = 0; i < tk_.tasks.size(); i++)
    {
      tasks_->add(tk_.tasks[i]);
    }

    //
    // Set the input & output files for the current row.
    // We'll call our selected_row to do it.
    //

    selected_row(0);

    //
    // Make the necessary connections to react when a row
    // in the list of tasks is clicked. This will take
    // care of future clicks on the tasks panel.
    //

    connect(tasks_, SIGNAL(selected_row(int)), this, SLOT(selected_row(int)));

  }
}

task_info_panel::
task_info_panel
(QWidget *parent)
  : QWidget{parent}
{
  {
    QString title;

    // Create the several components of the interface.

    tasks_        = new task_list_widget;
    title         = "Input files for the selected task";
    files_input_  = new file_list_widget(title);
    title         = "Output files for the selected task";
    files_output_ = new file_list_widget(title);

    // Lay out the toolbar and table view.

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(tasks_,        0, 0);
    layout->addWidget(files_input_,  1, 0);
    layout->addWidget(files_output_, 2, 0);

    // Make sure we've got enough room to show all our components.

    setMinimumWidth(350);

    setLayout(layout);
  }
}
