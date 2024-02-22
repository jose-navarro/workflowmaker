/** \file ToolkitEditor_parameter_list_widget.hpp
\brief Implementation file for ToolkitEditor_parameter_list_widget.cpp.
*/

#include "ToolkitEditor_parameter_list_widget.hpp"

void
ToolkitEditor_parameter_list_widget::
add
(toolkit_parameter& parameter)
{
  {
    model_->append_row(parameter);
    table_view_->resizeColumnsToContents();
    update_total_elements();
  }
}

void
ToolkitEditor_parameter_list_widget::
add_item
(void)
{
  {
    if (work_mode_ == 0)
      add_new_item();
    else
      add_existing_item();
  }
}

void
ToolkitEditor_parameter_list_widget::
add_existing_item
(void)
{
  {
    map<string, toolkit_parameter>::iterator it;
    toolkit_parameter                        par;
    QList<toolkit_parameter>                 the_parameters;

    // Check whether there are any parameters in the list!

    if (parameter_map_.size() == 0)
    {
      QString     message;
      QMessageBox msgBox;

      message  = "There exist no parameters yet.\n";
      message += "Please, add at least one file type in the Parameters tab.";

      msgBox.setText(message);
      msgBox.setWindowTitle("No data present");
      msgBox.setIcon(QMessageBox::Warning);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();

      return;
    }

    // Well, the map is not empty. We may go on.

    the_parameters.clear();

    for (it = parameter_map_.begin(); it != parameter_map_.end(); it++)
    {
      par = it->second;
      the_parameters.append(par);
    }

    ToolkitEditor_parameter_selector_widget dialog(&the_parameters);

    if (dialog.exec())
    {
      par = dialog.selected_parameter();
      model_->append_row(par);
      table_view_->resizeColumnsToContents();
      update_total_elements();
    }
  }
}

void
ToolkitEditor_parameter_list_widget::
add_new_item
(void)
{
  {
    toolkit_parameter par;

    par.id = par.description = "";
    par.type = "INTEGER";

    ToolkitEditor_single_parameter_widget dialog(par, 0);

    if (dialog.exec())
    {
      par = dialog.parameter();
      model_->append_row(par);
      table_view_->resizeColumnsToContents();
      update_total_elements();
    }
  }
}

void
ToolkitEditor_parameter_list_widget::
delete_selected_row
(void)
{
  {
    // Check that the row to delete does exist.

    if (cur_row_ < 0)                   return;
    if (cur_row_ >= parameters_.size()) return;

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
    int row_to_delete = unsorted.row();

    string par_to_delete = parameters_.at(row_to_delete).id;

    //
    // Now we know what's the row to delete.
    // If we are in work mode 0 (creating new parameters, not
    // adding these to a task) we must be cautions,
    // since the parameter that has been selected
    // by the user might be in use by some tasks.
    //
    // Let's check if this is so (only if work mode is 0!!!)
    //

    if (work_mode_ == 0)
    {
      QStringList affected_tasks = task_manager_->list_of_tasks_for_parameter(par_to_delete);

      if (affected_tasks.size() != 0)
      {
        // There are tasks using this parameter. We cannot proceed.

        QString     message;
        QMessageBox msgBox;

        message  = "It is not possible to delete this parameter.\n";
        message += "The task(s) below already use it.\n";
        message += "If you really want to remove this parameter,\n";
        message += "remove it first from the task(s) in the list below.\n\n";

        for (int i = 0; i < affected_tasks.size(); i++)
        {
          message = message + "  - " + affected_tasks[i] + "\n";
        }

        message += "\n";

        msgBox.setText(message);
        msgBox.setWindowTitle("Parameter already in use");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

        return;
      }
    }

    //
    // Now we may delete the parameter from the list, since we know
    // the real position affected and no tasks use it.
    //

    model_->removeRow(row_to_delete);

    // Reset things.

    table_view_->clearSelection();
    table_view_->resizeColumnsToContents();
    cur_row_ = -1;
    action_remove_item_->setEnabled(false);
    if (work_mode_ == 0) action_edit_item_->setEnabled(false);
    update_total_elements();
  }
}

void
ToolkitEditor_parameter_list_widget::
edit_item
(void)
{
  {
    toolkit_parameter par;

    // Check that the row to edit does exist.

    if (cur_row_ < 0)                   return;
    if (cur_row_ >= parameters_.size()) return;

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
    int unsorted_row = unsorted.row();

    //
    // Now we may edit the parameter from the list, since we know
    // the real position affected.
    //

    par = parameters_.at(unsorted_row);

    ToolkitEditor_single_parameter_widget dialog(par, 1);

    if (dialog.exec())
    {
      //
      // The user accepted the changes.
      // Retrieve the new parameter data and update the model, column by column.
      //

      par = dialog.parameter();

      // Identifier

      index = proxy_model_->index(cur_row_, 0);
      unsorted = proxy_model_->mapToSource(index);
      model_->setData(unsorted, QString::fromStdString(par.id), Qt::EditRole);

      // Type

      index = proxy_model_->index(cur_row_, 1);
      unsorted = proxy_model_->mapToSource(index);
      model_->setData(unsorted, QString::fromStdString(par.type), Qt::EditRole);

      // Description.

      index = proxy_model_->index(cur_row_, 2);
      unsorted = proxy_model_->mapToSource(index);
      model_->setData(unsorted, QString::fromStdString(par.description), Qt::EditRole);
    }
  }
}

QList<toolkit_parameter>*
ToolkitEditor_parameter_list_widget::
get_list
(void)
{
  {
    return &parameters_;
  }
}

int
ToolkitEditor_parameter_list_widget::
n_elements
(void)
const
{
  {
    return parameters_.size();
  }
}

void
ToolkitEditor_parameter_list_widget::
remove_all
(void)
{
  {
    model_->remove_all_rows();
  }
}

void
ToolkitEditor_parameter_list_widget::
row_selected
(int rowIndex)
{
  {
    action_remove_item_->setEnabled(true);
    if (work_mode_ == 0) action_edit_item_->setEnabled(true);
    cur_row_ = rowIndex;
  }
}

void
ToolkitEditor_parameter_list_widget::
set_parameter_map
(map<string, toolkit_parameter>& parameter_map)
{
  {    
    parameter_map_ = parameter_map;
  }
}

void
ToolkitEditor_parameter_list_widget::
set_task_manager
(ToolkitEditor_tasks_widget* task_mgr)
{
  {
    task_manager_ = task_mgr;
  }
}

ToolkitEditor_parameter_list_widget::
ToolkitEditor_parameter_list_widget
(int      work_mode,
 QWidget* parent) : QWidget(parent)
{
  {
    // Initialize some members.

    cur_row_      = -1;
    task_manager_ = nullptr;
    work_mode_    = work_mode;

    // Create the toolbar, including its actions.

    QToolBar* item_list_toolbar = new QToolBar(tr("Parameters list toolbar"), this);
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

    if (work_mode_ == 0)
    {
      QIcon edit_item_icon;
      edit_item_icon.addFile(QString::fromUtf8(":/resources/edit.png"), QSize(), QIcon::Normal, QIcon::Off);
      action_edit_item_ = new QAction(edit_item_icon, tr("Edit"), this);
      action_edit_item_->setEnabled(false);
      connect(action_edit_item_, SIGNAL(triggered()), this, SLOT(edit_item()));
      item_list_toolbar->addAction(action_edit_item_);

      item_list_toolbar->addSeparator();
    }

    total_elements_ = new QLabel(tr("Total:") + " 0");
    item_list_toolbar->addWidget(total_elements_);


    // Create the files model that will backing the table view.

    model_ = new ToolkitEditor_parameter_model(&parameters_, this);

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

    table_view_ = new ToolkitEditor_parameter_table_view(this);
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

    setWindowTitle(tr("Parameters"));

    // Windows flags.

    //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint );

  }
}

void
ToolkitEditor_parameter_list_widget::
update_total_elements
(void)
{
  {
    int total;

    total = parameters_.size();

    total_elements_->setText(tr("Total:") + " " + QString::number(total));
  }
}

