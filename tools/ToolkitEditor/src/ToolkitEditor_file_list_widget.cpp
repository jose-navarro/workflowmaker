/** \file ToolkitEditor_file_list_widget.hpp
\brief Implementation file for ToolkitEditor_file_list_widget.cpp.
*/

#include "ToolkitEditor_file_list_widget.hpp"

void
ToolkitEditor_file_list_widget::
add
(toolkit_file& file)
{
  {
    model_->append_row(file);
    table_view_->resizeColumnsToContents();
    update_total_elements();
  }
}

void
ToolkitEditor_file_list_widget::
add_item
(void)
{
  {
    if ((filetypes_ == nullptr) || (filetypes_->isEmpty()))
    {
      QString     message;
      QMessageBox msgBox;

      message  = "There exist no file types yet.\n";
      message += "Please, add at least one file type in the File types tab.";

      msgBox.setText(message);
      msgBox.setWindowTitle("No data present");
      msgBox.setIcon(QMessageBox::Warning);
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();

      return;
    }

    toolkit_file empty_file;
    ToolkitEditor_filetype_selector_widget dialog(filetypes_, empty_file);

    if (dialog.exec())
    {
      toolkit_file fil = dialog.file();
      model_->append_row(fil);
      table_view_->resizeColumnsToContents();
      update_total_elements();
    }
  }
}

void
ToolkitEditor_file_list_widget::
delete_selected_row
(void)
{
  {
    // Check that the row to delete does exist.

    if (cur_row_ < 0)              return;
    if (cur_row_ >= files_.size()) return;

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
    // Now we may delete the filename from the list, since we know
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
ToolkitEditor_file_list_widget::
edit_item
(void)
{
  {
    toolkit_file fil;

    // Check that the row to edit does exist.

    if (cur_row_ < 0)              return;
    if (cur_row_ >= files_.size()) return;

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

    fil = files_.at(unsorted_row);

    ToolkitEditor_filetype_selector_widget dialog(filetypes_, fil);

    if (dialog.exec())
    {
      //
      // The user accepted the changes.
      // Retrieve the new parameter data and update the model, column by column.
      //

      fil = dialog.file();

      // File type identifier

      index = proxy_model_->index(cur_row_, 0);
      unsorted = proxy_model_->mapToSource(index);
      model_->setData(unsorted, QString::fromStdString(fil.file_type_id), Qt::EditRole);

      // Options file label

      index = proxy_model_->index(cur_row_, 1);
      unsorted = proxy_model_->mapToSource(index);
      model_->setData(unsorted, QString::fromStdString(fil.options_file_label), Qt::EditRole);

      // Description.

      index = proxy_model_->index(cur_row_, 2);
      unsorted = proxy_model_->mapToSource(index);
      model_->setData(unsorted, QString::fromStdString(fil.description), Qt::EditRole);
    }
  }
}

QList<toolkit_file>*
ToolkitEditor_file_list_widget::
get_list
(void)
{
  {
    return &files_;
  }
}

int
ToolkitEditor_file_list_widget::
n_elements
(void)
const
{
  {
    return files_.size();
  }
}

void
ToolkitEditor_file_list_widget::
remove_all
(void)
{
  {
    model_->remove_all_rows();
  }
}

void
ToolkitEditor_file_list_widget::
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
ToolkitEditor_file_list_widget::
set_file_types
(QList<toolkit_file_type>* filetypes)
{
  {
    filetypes_ = filetypes;
  }
}

ToolkitEditor_file_list_widget::
ToolkitEditor_file_list_widget
(QWidget *parent) : QWidget(parent)
{
  {
    // Initialize some members.

    cur_row_   = -1;
    filetypes_ = nullptr;

    // Create the toolbar, including its actions.

    QToolBar* item_list_toolbar = new QToolBar(tr("Files list toolbar"), this);
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

    model_ = new ToolkitEditor_file_model(&files_, this);

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

    table_view_ = new ToolkitEditor_file_table_view(this);
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

    setWindowTitle(tr("Files"));

    // Windows flags.

    //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint );

  }
}

void
ToolkitEditor_file_list_widget::
update_total_elements
(void)
{
  {
    int total;

    total = files_.size();

    total_elements_->setText(tr("Total:") + " " + QString::number(total));
  }
}

