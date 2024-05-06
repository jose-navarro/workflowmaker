/** \file file_table_view.hpp
  \brief Table view for lists of files.
*/


#ifndef FILE_TABLE_VIEW_HPP
#define FILE_TABLE_VIEW_HPP

#include <QTableView>

/// \brief Table view for lists of files.

class file_table_view : public QTableView
{
    Q_OBJECT

  public:

    /**
      \brief Constructor
      \param[in] parent The parent of the widget.
     */

    explicit file_table_view (QWidget *parent = 0);
    
  signals:

    /**
      \brief Signal stating that a row has been selected.
      \param[in] rowIndex The index of the selected row.
     */

    void     row_selected    (int rowIndex);
    
  public slots:

    /**
      \brief Emits the row_selected signal to tell whoever might
             be interested that a row has been selected.
      \param[in] qi The index of the row selected.
     */

    void     clicked_slot    (QModelIndex qi);
    
};

#endif // FILE_TABLE_VIEW_HPP
