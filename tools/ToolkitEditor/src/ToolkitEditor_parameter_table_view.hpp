/** \file ToolkitEditor_parameter_table_view.cpp
  \brief Table view for lists of parameters.
*/


#ifndef TOOLKIT_EDITOR_PARAMETER_TABLE_VIEW_HPP
#define TOOLKIT_EDITOR_PARAMETER_TABLE_VIEW_HPP

#include <QTableView>

/// \brief Table view for lists of parameters.

class ToolkitEditor_parameter_table_view : public QTableView
{
    Q_OBJECT

  public:

    /**
      \brief Constructor
      \param parent The parent of the widget.
     */

    explicit ToolkitEditor_parameter_table_view (QWidget *parent = 0);
    
  signals:

    /**
      \brief Signal stating that a row has been selected.
      \param rowIndex The index of the selected row.
     */

    void     row_selected                         (int rowIndex);
    
  public slots:

    /**
      \brief Emits the row_selected signal to tell whoever might
             be interested that a row has been selected.
      \param qi The index of the row selected.
     */

    void     clicked_slot                         (QModelIndex qi);
    
};

#endif // TOOLKIT_EDITOR_PARAMETER_TABLE_VIEW_HPP
