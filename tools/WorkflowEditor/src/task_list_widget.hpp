/** \file task_list_widget.hpp
\brief Table-based widget to show a list of tasks.
*/

#ifndef TASK_LIST_WIDGET_HPP
#define TASK_LIST_WIDGET_HPP

#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QWidget>

#include "task_table_model.hpp"
#include "task_table_view.hpp"

#include "toolkit_structures.hpp"

/**
  \brief Table-based widget to show a list of tasks.

  The implementation of this widget relies on the table view +
  table model concept.

  See classes task_table_view (the table view) and task_table_model
  (the model), on which this class relies.
  */

class task_list_widget : public QWidget
{
  Q_OBJECT

  public:

    /**
      \brief add Add a new task to the list.
      \param[in] task The task to add.
     */

    void                 add                   (toolkit_task& task);

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


    /**
      \brief Constructor.
      \param[in] parent The parent of the widget.
     */

                         task_list_widget      (QWidget *parent = nullptr);

  signals:

    /// \brief Signal that a row has been selected.
    /**
      \param[in] rowIndex The number of the selected row.
     */

    void                 selected_row          (int rowIndex);

  protected slots:

    /**
      \brief Sets the row at position rowIndex as the current one, enabling
             the delete button.
      \param[in] rowIndex The number of the row that has been clicked.
     */

    void                 row_selected          (int rowIndex);

  protected:


    /// \brief Index of the current row (the selected one).

    int                 cur_row_;

    /// \brief The table model.

    task_table_model*   model_;

    /// \brief The table view.

    task_table_view*    table_view_;

    ///  \brief The list of tasks.

    QList<toolkit_task> tasks_;


};

#endif // TASK_LIST_WIDGET_HPP
