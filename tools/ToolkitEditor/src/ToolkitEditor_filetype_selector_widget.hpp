/** \file ToolkitEditor_filetype_selector_widget.cpp
\brief Table-based widget to show a list of file types
       and select one of them.
*/

#ifndef TOOLKITEDITOR_FILETYPE_SELECTOR_WIDGET_HPP
#define TOOLKITEDITOR_FILETYPE_SELECTOR_WIDGET_HPP

#include <QAction>
#include <QDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QSortFilterProxyModel>
#include <QToolBar>
#include <QVBoxLayout>

#include "toolkit_structures.hpp"
#include "ToolkitEditor_filetype_model.hpp"
#include "ToolkitEditor_filetype_table_view.hpp"

#include <string>

using namespace std;

/**
  \brief Table-based widget to show a list of file types and select one of them.

  The implementation of this widget relies on the table view +
  table model concept.

  See classes ToolkitEditor_filetype_table_view (the table view)
  and ToolkitEditor_filetype_model (the model), on which this
  class relies.
*/

class ToolkitEditor_filetype_selector_widget : public QDialog
{
  Q_OBJECT

  public:

    /**
      \brief Retrieve the current, selected file.
      \return The current, selected file.
     */

    toolkit_file file                                   (void);

    /**
      \brief n_elements Return the size (number of elements) in
             the current list of file types.
      \return The size of the list of file types.
     */

    int          n_elements                             (void) const;

    /**
      \brief Constructor.
      \param filetypes The list of available file types.
      \param file The initial values to show.
      \param parent The parent of the widget.
     */

                 ToolkitEditor_filetype_selector_widget (QList<toolkit_file_type>* filetypes,
                                                         toolkit_file&             file,
                                                         QWidget*                  parent = nullptr);

  protected slots:

    /**
      \brief Checks, each time that the description text is changed, whether
             it is possible to enable the accept button.
      \param the_text The text in the description field.
     */

    void         description_text_changed               (const QString& the_text);

    /**
      \brief Replace the text in the options file labe field by uppercase letters.
             Additionally, it checks (every time the text is changed) whether it
             is possible to enable the accept button.
      \param the_text The text currently typed in the identifier field.
     */

    void         options_label_text_changed             (const QString& the_text);

    /**
      \brief Callback to deal with the selection of a file type. Checks whether
             it is possible to enable the accept button.
     */

    void         select_filetype                        (void);

  protected:

    /**
      \brief Looks for some data type in the list of "official" data types.
      \param dtype The data type to search for.
      \return The position of the sought data type in the list of
             "official" data types. Returns -1 if not found.
     */

    int          find_data_type                         (string dtype);

    /**
      \brief Update (on the screen) the total number of elements in the
             list of file types.
     */

    void         update_total_elements                  (void);

  protected slots:

    /**
      \brief Sets the row at position rowIndex as the current one, enabling
             the delete button.
      \param rowIndex The number of the row that has been clicked.
     */

    void              row_selected                           (int rowIndex);

  protected:

    /// \brief Action to remove a file type.

    QAction*                           action_discard_item_;

    /// \brief Action to confirm the selection of a file type.

    QAction*                           action_select_item_;

    /// \brief Index of the current row (the selected one).

    int                                cur_row_;

    /// \brief The description text.

    QLineEdit*                         description_text_;

    ///  \brief The (selected) file.

    toolkit_file                       file_;

    /// \brief The list of available file types.

    QList<toolkit_file_type>*          filetypes_;

    /// \brief The table model.

    ToolkitEditor_filetype_model*      model_;

    /// \brief The options file label text.

    QLineEdit*                         options_label_text_;

    /// \brief The proxy model used to sort data.

    QSortFilterProxyModel*             proxy_model_;

    /// \brief The table view.

    ToolkitEditor_filetype_table_view* table_view_;

    /// \brief A label to show, on screen, the size of the list
    ///        of file types.

    QLabel*                            total_elements_;

};

#endif // TOOLKITEDITOR_FILETYPE_SELECTOR_WIDGET_HPP
