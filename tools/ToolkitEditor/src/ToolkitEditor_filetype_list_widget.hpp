/** \file ToolkitEditor_filetype_list_widget.cpp
\brief Table-based widget to show, add and remove file types.
*/

#ifndef TOOLKITEDITOR_FILETYPE_LIST_WIDGET_HPP
#define TOOLKITEDITOR_FILETYPE_LIST_WIDGET_HPP

#include <QAction>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QStringList>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

#include "ToolkitEditor_filetype_model.hpp"
#include "ToolkitEditor_filetype_table_view.hpp"

#include "ToolkitEditor_single_filetype_widget.hpp"
#include "ToolkitEditor_tasks_widget.hpp"

/**
  \brief Table-based widget to show, add and remove file types.

  The implementation of this widget relies on the table view +
  table model concept.

  See classes ToolkitEditor_filetype_table_view (the table view)
  and ToolkitEditor_filetype_model (the model), on which this
  class relies.
*/

class ToolkitEditor_filetype_list_widget : public QWidget
{
  Q_OBJECT

  public:

    /**
      \brief add Add a new file type to the list.
      \param filetype The file type to add.
     */

    void                      add                                (toolkit_file_type& filetype);

    /**
      \brief get_list Retrieve the list of file types that are
             currently shown by the widget.
      \return The current list of parameters.
     */

    QList<toolkit_file_type>* get_list                           (void);

    /**
      \brief n_elements Return the size (number of elements) in
             the current list of file types.
      \return The size of the list of file types.
     */

    int                       n_elements                         (void) const;

    /**
      \brief Clear the current list of file types, so the said
             list becomes empty.
     */

    void                      remove_all                         (void);

    /// \brief Set the pointer to the widget managing the list
    ///        of tasks.
    /**
      \param task_mgr The pointer to the task manager.
     */

    void                      set_task_manager                   (ToolkitEditor_tasks_widget* task_mgr);

    /**
      \brief Constructor.
      \param parent The parent of the widget.
     */

                              ToolkitEditor_filetype_list_widget (QWidget *parent = nullptr);

  protected:

    /**
      \brief Update (on the screen) the total number of elements in the
             list of file types.
     */

    void                      update_total_elements              (void);

  protected slots:

    /**
      \brief Opens a new dialog to provide the information for a
             new file type. If the user accepts the dialog, the new
             file type is appended to the current list.
     */

    void                      add_item                            (void);

    /**
      \brief Removes the row currently selected on the screen from
             the list of file types.
     */

    void                      delete_selected_row                 (void);

    /**
      \brief Opens a new dialog to edit the information of an
             existing file type. If the user accepts the dialog, the
             file type is modified.
     */

    void                      edit_item                           (void);

    /**
      \brief Sets the row at position rowIndex as the current one, enabling
             the delete button.
      \param rowIndex The number of the row that has been clicked.
     */

    void                      row_selected                        (int rowIndex);

  protected:

    /// \brief Action to add new file types.

    QAction*                           action_add_item_;

    /// \brief Action to edit an existing file type.

    QAction*                           action_edit_item_;

    /// \brief Action to remove a file type.

    QAction*                           action_remove_item_;

    /// \brief Index of the current row (the selected one).

    int                                cur_row_;

    ///  \brief The list of file_types.

    QList<toolkit_file_type>           filetypes_;

    /// \brief The table model.

    ToolkitEditor_filetype_model*      model_;

    /// \brief The proxy model used to sort data.

    QSortFilterProxyModel*             proxy_model_;

    /// \brief The table view.

    ToolkitEditor_filetype_table_view* table_view_;

    /// \brief Pointer to the ToolkitEditor_tasks_widget managing the
    ///        tasks.

    ToolkitEditor_tasks_widget*        task_manager_;

    /// \brief A label to show, on screen, the size of the list
    ///        of file types.

    QLabel*                            total_elements_;

};

#endif // TOOLKITEDITOR_FILETYPE_LIST_WIDGET_HPP
