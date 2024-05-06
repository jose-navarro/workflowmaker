/** \file ToolkitEditor_parameter_list_widget.cpp
\brief Table-based widget to show, add and remove parameters.
*/

#ifndef TOOLKITEDITOR_PARAMETER_LIST_WIDGET_HPP
#define TOOLKITEDITOR_PARAMETER_LIST_WIDGET_HPP

#include <QAction>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

#include "ToolkitEditor_parameter_model.hpp"
#include "ToolkitEditor_parameter_table_view.hpp"

#include "ToolkitEditor_parameter_selector_widget.hpp"
#include "ToolkitEditor_single_parameter_widget.hpp"
#include "ToolkitEditor_tasks_widget.hpp"

#include <string>
#include <map>

using namespace std;

/**
  \brief Table-based widget to show, add and remove parameters.

  The implementation of this widget relies on the table view +
  table model concept.

  See classes ToolkitEditor_parameter_table_view (the table view)
  and ToolkitEditor_parameter_model (the model), on which this
  class relies.
*/

class ToolkitEditor_parameter_list_widget : public QWidget
{
  Q_OBJECT

  public:

    /**
      \brief add Add a new parameter to the list.
      \param parameter The parameter to add.
     */

    void                      add                                 (toolkit_parameter& parameter);

    /**
      \brief get_list Retrieve the list of parameters that are
             currently shown by the widget.
      \return The current list of parameters.
     */

    QList<toolkit_parameter>* get_list                            (void);

    /**
      \brief n_elements Return the size (number of elements) in
             the current list of parameters.
      \return The size of the list of parameters.
     */

    int                       n_elements                          (void) const;

    /**
      \brief Clear the current list of parameters, so the said
             list becomes empty.
     */

    void                      remove_all                          (void);

    /// \brief Set the pointer to the widget managing the list
    ///        of tasks.
    /**
      \param task_mgr The pointer to the task manager.
     */

    void                      set_task_manager                    (ToolkitEditor_tasks_widget* task_mgr);

    /**
      \brief Constructor.
      \param work_mode Tells the widget how to react when the "add" button is
             clicked: adding a new parameter (set work_mode to 0) or select
             an existing one (work_mode must be anything but 0).
      \param parent The parent of the widget.
     */

                              ToolkitEditor_parameter_list_widget (int      work_mode,
                                                                   QWidget* parent = nullptr);

    /**
      \brief Set the map containing the list of available parameters.
      \param parameter_map The map with the available parameters.

      Call this method to pass the list of available parameters.

      When this map is set, it will NOT be possible to add completely
      new parameters, but only those present in a list. This behaviour
      is the one requested by the TASK editor, since there a list
      of preexisting parameters is built.

      When the map is not set, only completely new parameters (completely
      defined by the user, by hand) may be added. This is the behaviour
      requested by the PARAMETER editor.
     */

     void                     set_parameter_map                   (map<string, toolkit_parameter>& parameter_map);

  protected:

     /**
       \brief Opens a new dialog select among a list of existing parameters.
              If the user accepts the dialog, a new parameter is appended to
              the current list.
      */

     void                      add_existing_item                   (void);

     /**
       \brief Opens a new dialog to provide the information for a
              new parameter. If the user accepts the dialog, the new
              parameter is appended to the current list.
      */

     void                      add_new_item                       (void);


    /**
      \brief Update (on the screen) the total number of elements in the
             list of parameters.
     */

    void                      update_total_elements               (void);

  protected slots:


    /**
      \brief Either opens a dialog to add a completely new item or
             another one to select among a list of preexising items.
             Which one is opened depends on whether there is a list
             of available parameters.
     */

    void                      add_item                            (void);

    /**
      \brief Removes the row currently selected on the screen from
             the list of parameters.
     */

    void                      delete_selected_row                 (void);

    /**
      \brief Opens a new dialog to edit the information of an
             existing parameter. If the user accepts the dialog, the
             parameter is modified.
     */

    void                      edit_item                           (void);

    /**
      \brief Sets the row at position rowIndex as the current one, enabling
             the delete button.
      \param rowIndex The number of the row that has been clicked.
     */

    void                      row_selected                        (int rowIndex);

  protected:

    /// \brief Action to add new parameters.

    QAction*                            action_add_item_;

    /// \brief Action to edit existing parameters.

    QAction*                            action_edit_item_;

    /// \brief Action to remove a parameter.

    QAction*                            action_remove_item_;

    /// \brief Index of the current row (the selected one).

    int                                 cur_row_;

    /// \brief The table model.

    ToolkitEditor_parameter_model*      model_;

    /// \brief Map listing the existing parameters.

    map<string, toolkit_parameter>      parameter_map_;

    ///  \brief The list of parameters.

    QList<toolkit_parameter>            parameters_;

    /// \brief The proxy model used to sort data.

    QSortFilterProxyModel*              proxy_model_;

    /// \brief The table view.

    ToolkitEditor_parameter_table_view* table_view_;

    /// \brief Pointer to the ToolkitEditor_tasks_widget managing the
    ///        tasks.

    ToolkitEditor_tasks_widget*         task_manager_;

    /// \brief A label to show, on screen, the size of the list
    ///        of parameters.

    QLabel*                             total_elements_;

    /// \brief The work mode, that is, add a new parameter or
    ///        select an existing one.

    int                                 work_mode_;

};

#endif // TOOLKITEDITOR_PARAMETER_LIST_WIDGET_HPP
