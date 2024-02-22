/** \file ToolkitEditor_filetype_table_view.hpp
  \brief Implementation file for ToolkitEditor_filetype_table_view.cpp.
*/


#include "ToolkitEditor_filetype_table_view.hpp"

ToolkitEditor_filetype_table_view::
ToolkitEditor_filetype_table_view
(QWidget *parent) :
  QTableView(parent)
{
  {
    //
    // This is to notify whoever might be interested that an item has been
    // selected in the table view.
    //
    // The slot rowSelected() below gets called when the event is detected.
    //

    connect (this, SIGNAL(clicked     (QModelIndex)),
             this, SLOT  (clicked_slot(QModelIndex)));
  }
}

void
ToolkitEditor_filetype_table_view::
clicked_slot
(QModelIndex qi)
{
  {
    emit row_selected(qi.row());
  }
}
