/** \file ToolkitEditor_task_list_widget.hpp
\brief Implementation file for ToolkitEditor_task_list_widget.cpp.
*/

#include "ToolkitEditor_task_list_widget.hpp"

void
ToolkitEditor_task_list_widget::
add
(toolkit_task& task)
{
  {
    model_->append_row(task);
    table_view_->resizeColumnsToContents();
    update_total_elements();
  }
}

void
ToolkitEditor_task_list_widget::
add_item
(void)
{
  {
    toolkit_task tk;

    tk.id = tk.description = tk.name_of_executable = "";
    tk.parameters.clear();
    tk.input_files.clear();
    tk.output_files.clear();

    ToolkitEditor_single_task_widget dialog(tk, parameter_map_, filetypes_list_);
    dialog.setMinimumWidth(900);

    if (dialog.exec())
    {
      tk = dialog.task();
      model_->append_row(tk);
      table_view_->resizeColumnsToContents();
      update_total_elements();
    }
  }
}

void
ToolkitEditor_task_list_widget::
delete_selected_row
(void)
{
  {
    // Check that the row to delete does exist.

    if (cur_row_ < 0)              return;
    if (cur_row_ >= tasks_.size()) return;

    //
    // Data is shown sorted on screen (this is the task of the
    // proxy model. This means that cur_row_ holds the number of
    // the rown as shown on the screen.
    //
    // However, data is stored in an unsorted list. So we must
    // translate this cur_row_ on screen (proxy) index to the
    // corresponding position in the unsorted list. The proxy
    // model will help us to do this.
    //

    // Get an "index" representation of our int cur_row_.

    QModelIndex index = proxy_model_->index(cur_row_,0);

    // Translate this to an unsorted index in the unsorted list.

    QModelIndex unsorted = proxy_model_->mapToSource(index);

    //
    // Now we may delete the task from the list, since we know
    // the real position affected.
    //

    model_->removeRow(unsorted.row());

    // Reset things.

    table_view_->clearSelection();
    table_view_->resizeColumnsToContents();
    cur_row_ = -1;
    action_remove_item_->setEnabled(false);
    action_edit_item_->setEnabled(false);
    update_total_elements();
  }
}

void
ToolkitEditor_task_list_widget::
edit_item
(void)
{
  {
    toolkit_task tk;

    // Check that the row to edit does exist.

    if (cur_row_ < 0)              return;
    if (cur_row_ >= tasks_.size()) return;

    //
    // Data is shown sorted on screen (this is the task of the
    // proxy model. This means that cur_row_ holds the number of
    // the rown as shown on the screen.
    //
    // However, data is stored in an unsorted list. So we must
    // translate this cur_row_ on screen (proxy) index to the
    // corresponding position in the unsorted list. The proxy
    // model will help us to do this.
    //

    // Get an "index" representation of our int cur_row_.

    QModelIndex index = proxy_model_->index(cur_row_,0);

    // Translate this to an unsorted index in the unsorted list.

    QModelIndex unsorted = proxy_model_->mapToSource(index);

    //
    // Now we may edit the task from the list, since we know
    // the real position affected.
    //

    int unsorted_row = unsorted.row();
    tk = tasks_.at(unsorted_row);

    ToolkitEditor_single_task_widget dialog(tk, parameter_map_, filetypes_list_);
    dialog.setMinimumWidth(900);

    if (dialog.exec())
    {
      //
      // The user accepted the changes.
      // Retrieve the new task data and update the model, column by column.
      //

      tk = dialog.task();

      // Identifier

      index = proxy_model_->index(cur_row_, 0);
      unsorted = proxy_model_->mapToSource(index);
      model_->setData(unsorted, QString::fromStdString(tk.id), Qt::EditRole);

      // Description.

      index = proxy_model_->index(cur_row_, 1);
      unsorted = proxy_model_->mapToSource(index);
      model_->setData(unsorted, QString::fromStdString(tk.description), Qt::EditRole);

      // Executable.

      index = proxy_model_->index(cur_row_, 2);
      unsorted = proxy_model_->mapToSource(index);
      model_->setData(unsorted, QString::fromStdString(tk.name_of_executable), Qt::EditRole);

      // Parameters. Note the use of a QVariant.

      index = proxy_model_->index(cur_row_, 3);
      unsorted = proxy_model_->mapToSource(index);
      QVariant vparams;
      vparams.setValue(tk.parameters);
      model_->setData(unsorted, vparams, Qt::EditRole);

      // Input files. Note the use of a QVariant.

      index = proxy_model_->index(cur_row_, 4);
      unsorted = proxy_model_->mapToSource(index);
      QVariant vifiles;
      vifiles.setValue(tk.input_files);
      model_->setData(unsorted, vifiles, Qt::EditRole);

      // Output files. Note the use of a QVariant.

      index = proxy_model_->index(cur_row_, 5);
      unsorted = proxy_model_->mapToSource(index);
      QVariant vofiles;
      vofiles.setValue(tk.output_files);
      model_->setData(unsorted, vofiles, Qt::EditRole);
    }
  }
}

QList<toolkit_task>*
ToolkitEditor_task_list_widget::
get_list
(void)
{
  {
    return &tasks_;
  }
}

QStringList
ToolkitEditor_task_list_widget::
list_of_tasks_for_filetype
(string& filetype)
{
  {
    QStringList result;

    for (int task_idx = 0; task_idx < tasks_.size(); task_idx++)
    {
      bool found_for_this_task = false;

      toolkit_task tk = tasks_[task_idx];

      for (size_t ifile_idx = 0; ifile_idx < tk.input_files.size(); ifile_idx++)
      {
        string filtyp = tk.input_files[ifile_idx].file_type_id;
        if (filtyp == filetype)
        {
          found_for_this_task = true;
          result.push_back(QString::fromStdString(tk.id));
          break;
        }
      }

      if (!found_for_this_task)
      {
        for (size_t ofile_idx = 0; ofile_idx < tk.output_files.size(); ofile_idx++)
        {
          string filtyp = tk.output_files[ofile_idx].file_type_id;
          if (filtyp == filetype)
          {
            result.push_back(QString::fromStdString(tk.id));
            break;
          }
        }
      }
    }

    return result;
  }
}

QStringList
ToolkitEditor_task_list_widget::
list_of_tasks_for_parameter
(string& parameter)
{
  {
    QStringList result;

    for (int task_idx = 0; task_idx < tasks_.size(); task_idx++)
    {
      toolkit_task tk = tasks_[task_idx];

      for (size_t par_idx = 0; par_idx < tk.parameters.size(); par_idx++)
      {
        string par = tk.parameters[par_idx];
        if (par == parameter)
        {
          result.push_back(QString::fromStdString(tk.id));
          break;
        }
      }
    }

    return result;
  }
}

int
ToolkitEditor_task_list_widget::
n_elements
(void)
const
{
  {
    return tasks_.size();
  }
}

void
ToolkitEditor_task_list_widget::
remove_all
(void)
{
  {
    model_->remove_all_rows();
  }
}

void
ToolkitEditor_task_list_widget::
row_selected
(int rowIndex)
{
  {
    action_remove_item_->setEnabled(true);
    action_edit_item_->setEnabled(true);
    cur_row_ = rowIndex;
  }
}

void
ToolkitEditor_task_list_widget::
set_file_types
(QList<toolkit_file_type>* filetypes_list)
{
  {
    filetypes_list_ = filetypes_list;
  }
}

void
ToolkitEditor_task_list_widget::
set_parameters
(map<string, toolkit_parameter>& parameter_map)
{
  {
    parameter_map_ = parameter_map;
  }
}

ToolkitEditor_task_list_widget::
ToolkitEditor_task_list_widget
(QWidget *parent) : QWidget(parent)
{
  {
    // Initialize some members.

    cur_row_        = -1;
    filetypes_list_ = nullptr;

    // Create the toolbar, including its actions.

    QToolBar* item_list_toolbar = new QToolBar(tr("Task list toolbar"), this);
    item_list_toolbar->setMovable(false);

    QIcon remove_item_icon;
    remove_item_icon.addFile(QString::fromUtf8(":/resources/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
    action_remove_item_ = new QAction(remove_item_icon, tr("Remove"), this);
    action_remove_item_->setEnabled(false);
    connect(action_remove_item_, SIGNAL(triggered()), this, SLOT(delete_selected_row()));
    item_list_toolbar->addAction(action_remove_item_);

    item_list_toolbar->addSeparator();

    QIcon new_item_icon;
    new_item_icon.addFile(QString::fromUtf8(":/resources/add.png"), QSize(), QIcon::Normal, QIcon::Off);
    action_add_item_ = new QAction(new_item_icon, tr("Add"), this);
    action_add_item_->setEnabled(true);
    connect(action_add_item_, SIGNAL(triggered()), this, SLOT(add_item()));
    item_list_toolbar->addAction(action_add_item_);

    item_list_toolbar->addSeparator();

    QIcon edit_item_icon;
    edit_item_icon.addFile(QString::fromUtf8(":/resources/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
    action_edit_item_ = new QAction(edit_item_icon, tr("Edit"), this);
    action_edit_item_->setEnabled(false);
    connect(action_edit_item_, SIGNAL(triggered()), this, SLOT(edit_item()));
    item_list_toolbar->addAction(action_edit_item_);

    item_list_toolbar->addSeparator();

    total_elements_ = new QLabel(tr("Total:") + " 0");
    item_list_toolbar->addWidget(total_elements_);


    // Create the files model that will backing the table view.

    model_ = new ToolkitEditor_task_model(&tasks_, this);

    //
    // Create a proxy model, to sort the data by ascending identifier.
    // This "proxy model" is just a filter between the table view and the
    // data model itself. Thanks to this proxy, the information is shown
    // as ordered data in the view, even if the data is UNSORTED in the
    // files data structures.
    //

    proxy_model_ = new QSortFilterProxyModel(this);
    proxy_model_->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxy_model_->setDynamicSortFilter(true);
    proxy_model_->setSortLocaleAware(true);
    proxy_model_->setSourceModel(model_); // Sort the data in model_

    // Set up the table view for the points.

    table_view_ = new ToolkitEditor_task_table_view(this);
    table_view_->setModel(proxy_model_); // proxyModel_ instead of model_!!!
    table_view_->setSortingEnabled(true);
    table_view_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_view_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_view_->verticalHeader()->hide();

    // Lay out the toolbar and table view.

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(item_list_toolbar);
    main_layout->addWidget(table_view_);
    setLayout(main_layout);

    // Connect events coming from the table view.

    connect(table_view_, SIGNAL(row_selected(int)), this, SLOT(row_selected(int)));

    // Set the window title.

    setWindowTitle(tr("Tasks"));

    // Windows flags.

    //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint );

  }
}

void
ToolkitEditor_task_list_widget::
update_total_elements
(void)
{
  {
    int total;

    total = tasks_.size();

    total_elements_->setText(tr("Total:") + " " + QString::number(total));
  }
}

