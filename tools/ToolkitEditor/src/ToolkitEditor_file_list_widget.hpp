/** \file ToolkitEditor_file_list_widget.cpp
\brief Table-based widget to show, add and remove files.
*/

#ifndef TOOLKITEDITOR_FILE_LIST_WIDGET_HPP
#define TOOLKITEDITOR_FILE_LIST_WIDGET_HPP

#include <QAction>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

#include "ToolkitEditor_file_model.hpp"
#include "ToolkitEditor_file_table_view.hpp"

#include "ToolkitEditor_filetype_selector_widget.hpp"

/**
  \brief Table-based widget to show, add and remove files.

  The implementation of this widget relies on the table view +
  table model concept.

  See classes ToolkitEditor_file_table_view (the table view)
  and ToolkitEditor_file_model (the model), on which this
  class relies.
*/

class ToolkitEditor_file_list_widget : public QWidget
{
  Q_OBJECT

  public:

    /**
      \brief add Add a new file to the list.
      \param file The file to add.
     */

    void                 add                            (toolkit_file& file);

    /**
      \brief get_list Retrieve the list of files that are
             currently shown by the widget.
      \return The current list of files.
     */

    QList<toolkit_file>* get_list                       (void);

    /**
      \brief n_elements Return the size (number of elements) in
             the current list of files.
      \return The size of the list of files.
     */

    int                  n_elements                     (void) const;

    /**
      \brief Clear the current list of files, so the said
             list becomes empty.
     */

    void                 remove_all                     (void);

    /**
      \brief Set the list of available file types.
      \param filetypes The list of available file types.
     */

    void                 set_file_types                 (QList<toolkit_file_type>* filetypes);

    /**
      \brief Constructor.
      \param parent The parent of the widget.
     */

                         ToolkitEditor_file_list_widget (QWidget *parent = nullptr);

  protected:

    /**
      \brief Update (on the screen) the total number of elements in the
             list of files.
     */

    void                 update_total_elements          (void);

  protected slots:

    /**
      \brief Opens a new dialog to provide the information for a
             new file. If the user accepts the dialog, the new
             file is appended to the current list.
     */

    void                 add_item                       (void);

    /**
      \brief Removes the row currently selected on the screen from
             the list of files.
     */

    void                 delete_selected_row            (void);

    /**
      \brief Opens a new dialog to edit the information of an
             existing file. If the user accepts the dialog, the
             file is modified.
     */

    void                 edit_item                      (void);

    /**
      \brief Sets the row at position rowIndex as the current one, enabling
             the delete button.
      \param rowIndex The number of the row that has been clicked.
     */

    void                 row_selected                   (int rowIndex);

  protected:

    /// \brief Action to add new files.

    QAction*                       action_add_item_;

    /// \brief Action to edit an existing file.

    QAction*                       action_edit_item_;

    /// \brief Action to remove a file.

    QAction*                       action_remove_item_;

    /// \brief Index of the current row (the selected one).

    int                            cur_row_;

    ///  \brief The list of files.

    QList<toolkit_file>            files_;

    /// \brief The list of available file types.

    QList<toolkit_file_type>*      filetypes_;

    /// \brief The table model.

    ToolkitEditor_file_model*      model_;

    /// \brief The proxy model used to sort data.

    QSortFilterProxyModel*         proxy_model_;

    /// \brief The table view.

    ToolkitEditor_file_table_view* table_view_;

    /// \brief A label to show, on screen, the size of the list
    ///        of files.

    QLabel*                        total_elements_;

};

#endif // TOOLKITEDITOR_FILE_LIST_WIDGET_HPP
