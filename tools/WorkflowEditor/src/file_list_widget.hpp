/** \file file_list_widget.hpp
\brief Table-based widget to show a list of files.
*/

#ifndef FILE_LIST_WIDGET_HPP
#define FILE_LIST_WIDGET_HPP

#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QWidget>

#include "file_table_model.hpp"
#include "file_table_view.hpp"

#include "toolkit_structures.hpp"

/**
  \brief Table-based widget to show a list of files.

  The implementation of this widget relies on the table view +
  table model concept.

  See classes file_table_view (the table view) and file_table_model
  (the model), on which this class relies.
  */

class file_list_widget : public QWidget
{
  Q_OBJECT

  public:

    /**
      \brief add Add a new file to the list.
      \param[in] file The file to add.
     */

    void                 add                   (toolkit_file& file);

    /**
      \brief Constructor.
      \param[in] title The title for the list of files.
      \param[in] parent The parent of the widget.
     */

                         file_list_widget      (QString& title,
                                                QWidget* parent = nullptr);

    /**
      \brief n_elements Return the size (number of elements) in
             the current list of file types.
      \return The size of the list of file types.
     */

    int                  n_elements            (void) const;

    /**
      \brief Clear the current list of file types, so the said
             list becomes empty.
     */

    void                 remove_all            (void);

  protected:


    /// \brief Index of the current row (the selected one).

    int                    cur_row_;

    ///  \brief The list of file_types.

    QList<toolkit_file>    files_;

    /// \brief The table model.

    file_table_model*      model_;

    /// \brief The table view.

    file_table_view*       table_view_;

};

#endif // FILE_LIST_WIDGET_HPP
