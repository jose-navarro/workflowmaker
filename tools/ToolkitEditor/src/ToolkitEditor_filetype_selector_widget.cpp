/** \file ToolkitEditor_filetype_selector_widget.hpp
\brief Implementation file for ToolkitEditor_filetype_selector_widget.cpp.
*/

#include "ToolkitEditor_filetype_selector_widget.hpp"

void
ToolkitEditor_filetype_selector_widget::
description_text_changed
(const QString& the_text)
{
  {
    Q_UNUSED(the_text);

    // Check if we should enable the accept button.

    if ((!options_label_text_->text().isEmpty()) &&
        (!description_text_->text().isEmpty())   &&
        (cur_row_ != -1))
      action_select_item_->setEnabled(true);
    else
      action_select_item_->setEnabled(false);
  }
}

toolkit_file
ToolkitEditor_filetype_selector_widget::
file
(void)
{
  {
    return file_;
  }
}

int
ToolkitEditor_filetype_selector_widget::
find_data_type
(string dtype)
{
  {
    int result = -1;

    for (int i = 0; i < filetypes_->size(); i++)
    {
      if (filetypes_->at(i).id == dtype)
      {
        result = i;
        break;
      }
    }

    return result;
  }
}

int
ToolkitEditor_filetype_selector_widget::
n_elements
(void)
const
{
  {
    return filetypes_->size();
  }
}

void
ToolkitEditor_filetype_selector_widget::
options_label_text_changed
(const QString& the_text)
{
  {
    // Change the text in the field to uppercase.

    QString utext = the_text.toUpper();
    options_label_text_->setText(utext);

    // Check if we should enable the accept button.

    if ((!options_label_text_->text().isEmpty()) &&
        (!description_text_->text().isEmpty())   &&
        (cur_row_ != -1))
      action_select_item_->setEnabled(true);
    else
      action_select_item_->setEnabled(false);
  }
}

void
ToolkitEditor_filetype_selector_widget::
row_selected
(int rowIndex)
{
  {
    // Update the current row.

    cur_row_ = rowIndex;

    //
    // We'll enable the accept button only when all the fields
    // have been input.
    //

    action_select_item_->setEnabled(false);

    if (description_text_->text().isEmpty())   return;
    if (options_label_text_->text().isEmpty()) return;

    //
    // We've got all that we need. We may let the user go away
    // if (s)he wants to.
    //

    action_select_item_->setEnabled(true);
  }
}

void
ToolkitEditor_filetype_selector_widget::
select_filetype
(void)
{
  {
    // Check that the row to delete does exist.

    if (cur_row_ < 0)                   return;
    if (cur_row_ >= filetypes_->size()) return;

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

    QModelIndex index = proxy_model_->index(cur_row_, 0);

    // Translate this to an unsorted index in the unsorted list.

    QModelIndex unsorted = proxy_model_->mapToSource(index);

    //
    // Now we retrieve the item from the list, since we know its
    // actual (unsorted) position. We set the other, attached
    // attributes.
    //

    file_.file_type_id = filetypes_->at(unsorted.row()).id;
    file_.description  = description_text_->text().toStdString();
    file_.options_file_label = options_label_text_->text().toStdString();

    // Go away!

    accept();
  }
}

ToolkitEditor_filetype_selector_widget::
ToolkitEditor_filetype_selector_widget
(QList<toolkit_file_type>* filetypes,
 toolkit_file&             file,
 QWidget*                  parent) : QDialog(parent)
{
  {
    // Initialize some members.

    cur_row_   = -1;
    filetypes_ = filetypes;

    // Create the toolbar, including its actions.

    QToolBar* item_list_toolbar = new QToolBar(tr("File types list toolbar"), this);
    item_list_toolbar->setMovable(false);

    QIcon discard_item_icon;
    discard_item_icon.addFile(QString::fromUtf8(":/resources/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
    action_discard_item_ = new QAction(discard_item_icon, tr("Discard"), this);
    action_discard_item_->setEnabled(true);
    connect(action_discard_item_, SIGNAL(triggered()), this, SLOT(close()));
    item_list_toolbar->addAction(action_discard_item_);

    item_list_toolbar->addSeparator();

    QIcon select_item_icon;
    select_item_icon.addFile(QString::fromUtf8(":/resources/confirm.png"), QSize(), QIcon::Normal, QIcon::Off);
    action_select_item_ = new QAction(select_item_icon, tr("Confirm"), this);
    action_select_item_->setEnabled(false);
    connect(action_select_item_, SIGNAL(triggered()), this, SLOT(select_filetype()));
    item_list_toolbar->addAction(action_select_item_);

    item_list_toolbar->addSeparator();

    total_elements_ = new QLabel(tr("Total:") + " 0");
    item_list_toolbar->addWidget(total_elements_);

    // Create the files model that will backing the table view.

    model_ = new ToolkitEditor_filetype_model(filetypes_, this);

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

    table_view_ = new ToolkitEditor_filetype_table_view(this);

    table_view_->setModel(proxy_model_); // proxyModel_ instead of model_!!!
    table_view_->setSortingEnabled(true);
    table_view_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_view_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_view_->verticalHeader()->hide();

    // Create the description and options file label area.

    QLabel* options_label_label_ = new QLabel("Options file label");
    options_label_text_          = new QLineEdit;

    QLabel* description_label_   = new QLabel("Specific description\n(for this task)");
    description_text_            = new QLineEdit;

    QGridLayout* edit_layout = new QGridLayout;

    edit_layout->addWidget(options_label_label_, 0, 0);
    edit_layout->addWidget(options_label_text_,  0, 1);
    edit_layout->addWidget(description_label_,   1, 0);
    edit_layout->addWidget(description_text_,    1, 1);

    // Lay out the toolbar, the table view and the line edit fields.

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(item_list_toolbar);
    main_layout->addWidget(table_view_);
    main_layout->addLayout(edit_layout);
    setLayout(main_layout);

    // Connect events coming from the table view.

    connect(table_view_,         SIGNAL(row_selected(int)),
            this,                SLOT  (row_selected(int)));

    // Connect Value the description & options file label text fields changed.

    connect(description_text_,   SIGNAL(textChanged(const QString&)),
            this,                SLOT  (description_text_changed(const QString&)));

    connect(options_label_text_, SIGNAL(textChanged(const QString&)),
            this,                SLOT  (options_label_text_changed(const QString&)));

    // Set the values on the screen.

    description_text_->setText(QString::fromStdString(file.description));
    options_label_text_->setText(QString::fromStdString(file.options_file_label));

    if (file.file_type_id != "")
    {
      int unsorted_row = find_data_type(file.file_type_id);

      if (unsorted_row != -1)
      {
        QModelIndex index = model_->index(unsorted_row, 0);

        // Translate this to an unsorted index in the unsorted list.

        QModelIndex sorted = proxy_model_->mapFromSource(index);

        // Set the current index of our table view.

        table_view_->setCurrentIndex(sorted);

        // Check if we have to enable the accept button.

        if ((!options_label_text_->text().isEmpty()) &&
            (!description_text_->text().isEmpty()))
          action_select_item_->setEnabled(true);
      }
    }

    // Set the window title.

    setWindowTitle(tr("File type selector"));

    // Resize.

    table_view_->resizeColumnsToContents();
    update_total_elements();

    // Windows flags.

    //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint );
  }
}

void
ToolkitEditor_filetype_selector_widget::
update_total_elements
(void)
{
  {
    int total;

    total = filetypes_->size();

    total_elements_->setText(tr("Total:") + " " + QString::number(total));
  }
}

