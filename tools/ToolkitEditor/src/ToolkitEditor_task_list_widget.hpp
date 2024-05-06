/** \file ToolkitEditor_task_list_widget.cpp
\brief Table-based widget to show, add, edit and remove tasks.
*/

#ifndef TOOLKITEDITOR_TASK_LIST_WIDGET_HPP
#define TOOLKITEDITOR_TASK_LIST_WIDGET_HPP

#include <QWidget>
#include <QStringList>
#include <QAction>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>
#include <QToolBar>
#include <QLabel>
#include <QHeaderView>

#include "ToolkitEditor_task_model.hpp"
#include "ToolkitEditor_task_table_view.hpp"
#include "ToolkitEditor_single_task_widget.hpp"

#include <set>
#include <map>

using namespace std;

/**
  \brief Table-based widget to show, add, edit and remove tasks.

  The implementation of this widget relies on the table view +
  table model concept.

  See classes ToolkitEditor_task_table_view (the table view)
  and ToolkitEditor_task_model (the model), on which this
  class relies.
*/

class ToolkitEditor_task_list_widget : public QWidget
{
  Q_OBJECT

  public:

    /**
      \brief add Add a new task to the list.
      \param task The task to add.
     */

    void                 add                            (toolkit_task& task);

    /**
      \brief get_list Retrieve the list of tasks that are
             currently shown by the widget.
      \return The current list of tasks.
     */

    QList<toolkit_task>* get_list                       (void);

    /**
      \brief n_elements Return the size (number of elements) in
             the current list of tasks.
      \return The size of the list of tasks.
     */

    int                  n_elements                     (void) const;

    /**
      \brief Clear the current list of tasks, so the said
             list becomes empty.
     */

    void                 remove_all                     (void);

    /**
      \brief Set the list of available file types.
      \param filetypes_list The list of available file types.
     */

    void                 set_file_types                 (QList<toolkit_file_type>* filetypes_list);

    /**
      \brief Set the map containing the available parameters in the toolkit.
      \param parameter_map The map with the parameters.
     */

    void                 set_parameters                 (map<string, toolkit_parameter>& parameter_map);

    /**
      \brief Get the list of task identifiers using a given file type.
      \param filetype The filetype to search for.
      \return The list of tasks using the sought file type.

      The output list may be empty if no task is using the file type sought.
     */

    QStringList          list_of_tasks_for_filetype     (string& filetype);

    /**
      \brief Get the list of task identifiers using a given parameter.
      \param parameter The parameter to search for.
      \return The list of tasks using the sought parameter.

      The output list may be empty if no task is using the parameter sought.
     */

    QStringList          list_of_tasks_for_parameter     (string& parameter);

    /**
      \brief Constructor.
      \param parent The parent of the widget.
     */

                         ToolkitEditor_task_list_widget (QWidget *parent = nullptr);

  protected:

    /**
      \brief Update (on the screen) the total number of elements in the
             list of tasks.
     */

    void                 update_total_elements          (void);

  protected slots:

    /**
      \brief Opens a new dialog to provide the information for a
             new task. If the user accepts the dialog, the new
             task is appended to the current list.
     */

    void                 add_item                       (void);

    /**
      \brief Removes the row currently selected on the screen from
             the list of tasks.
     */

    void                 delete_selected_row            (void);

    /**
      \brief Opens a new dialog to edit the information of an
             existing task. If the user accepts the dialog, the
             task is modified.
     */

    void                 edit_item                      (void);

    /**
      \brief Sets the row at position rowIndex as the current one, enabling
             the delete button.
      \param rowIndex The number of the row that has been clicked.
     */

    void                 row_selected                   (int rowIndex);

  protected:

    /// \brief Action to add new tasks.

    QAction*                       action_add_item_;

    /// \brief Action to edit existing tasks.

    QAction*                       action_edit_item_;

    /// \brief Action to remove a task.

    QAction*                       action_remove_item_;

    /// \brief Index of the current row (the selected one).

    int                            cur_row_;

    /// \brief List of available file types.

    QList<toolkit_file_type>*      filetypes_list_;

    /// \brief The table model.

    ToolkitEditor_task_model*      model_;

    /// \brief The map with the parameters available in the toolkit.

    map<string, toolkit_parameter> parameter_map_;

    /// \brief The proxy model used to sort data.

    QSortFilterProxyModel*         proxy_model_;

    /// \brief The table view.

    ToolkitEditor_task_table_view* table_view_;

    ///  \brief The list of tasks.

    QList<toolkit_task>            tasks_;

    /// \brief A label to show, on screen, the size of the list
    ///        of tasks.

    QLabel*                        total_elements_;

};

#endif // TOOLKITEDITOR_TASK_LIST_WIDGET_HPP
