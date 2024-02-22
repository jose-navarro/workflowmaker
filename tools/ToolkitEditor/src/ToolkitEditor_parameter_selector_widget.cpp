/** \file ToolkitEditor_parameter_selector_widget.hpp
\brief Implementation file for ToolkitEditor_parameter_selector_widget.cpp.
*/

#include "ToolkitEditor_parameter_selector_widget.hpp"

int
ToolkitEditor_parameter_selector_widget::
n_elements
(void)
const
{
  {
    return parameters_->size();
  }
}

void
ToolkitEditor_parameter_selector_widget::
row_selected
(int rowIndex)
{
  {
    action_select_item_->setEnabled(true);
    cur_row_ = rowIndex;
  }
}

void
ToolkitEditor_parameter_selector_widget::
select_parameter
(void)
{
  {
    // Check that the row to delete does exist.

    if (cur_row_ < 0)                    return;
    if (cur_row_ >= parameters_->size()) return;

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
    // actual (unsorted) position.
    //

    parameter_ = parameters_->at(unsorted.row());

    // Go away!

    accept();
  }
}

toolkit_parameter
ToolkitEditor_parameter_selector_widget::
selected_parameter
(void)
{
  {
    return parameter_;
  }
}

ToolkitEditor_parameter_selector_widget::
ToolkitEditor_parameter_selector_widget
(QList<toolkit_parameter>* parameters,
 QWidget *parent) : QDialog(parent)
{
  {
    // Initialize some members.

    cur_row_ = -1;

    parameters_ = parameters;

    // Create the toolbar, including its actions.

    QToolBar* item_list_toolbar = new QToolBar(tr("Parameters list toolbar"), this);
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
    connect(action_select_item_, SIGNAL(triggered()), this, SLOT(select_parameter()));
    item_list_toolbar->addAction(action_select_item_);

    item_list_toolbar->addSeparator();

    total_elements_ = new QLabel(tr("Total:") + " 0");
    item_list_toolbar->addWidget(total_elements_);

    // Create the files model that will backing the table view.

    model_ = new ToolkitEditor_parameter_model(parameters_, this);

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

    setWindowTitle(tr("Parameter selector"));

    // Resize.

    table_view_->resizeColumnsToContents();
    update_total_elements();

    // Windows flags.

    //setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint );
  }
}

void
ToolkitEditor_parameter_selector_widget::
update_total_elements
(void)
{
  {
    int total;

    total = parameters_->size();

    total_elements_->setText(tr("Total:") + " " + QString::number(total));
  }
}

