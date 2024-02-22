/** \file ToolkitEditor_parameter_selector_widget.cpp
\brief Table-based widget to show a list of parameters
       and select one of them.
*/

#ifndef TOOLKITEDITOR_PARAMETER_SELECTOR_WIDGET_HPP
#define TOOLKITEDITOR_PARAMETER_SELECTOR_WIDGET_HPP

#include <QAction>
#include <QDialog>
#include <QHeaderView>
#include <QLabel>
#include <QList>
#include <QSortFilterProxyModel>
#include <QToolBar>
#include <QVBoxLayout>

#include "ToolkitEditor_parameter_model.hpp"
#include "ToolkitEditor_parameter_table_view.hpp"

/**
  \brief Table-based widget to show a list of parameters and select one of them.

  The implementation of this widget relies on the table view +
  table model concept.

  See classes ToolkitEditor_parameter_table_view (the table view)
  and ToolkitEditor_parameter_model (the model), on which this
  class relies.
*/

class ToolkitEditor_parameter_selector_widget : public QDialog
{
  Q_OBJECT

  public:

    /**
      \brief n_elements Return the size (number of elements) in
             the current list of parameters.
      \return The size of the list of parameters.
     */

    int               n_elements                              (void) const;

    /**
      \brief Retrieve the current, selected parameters.
      \return The current, selected parameter.
     */

    toolkit_parameter selected_parameter                      (void);

    /**
      \brief Constructor.
      \param parameters The list of available parameters.
      \param parent The parent of the widget.
     */

                      ToolkitEditor_parameter_selector_widget (QList<toolkit_parameter>* parameters,
                                                               QWidget*                  parent = nullptr);

  protected slots:

    /**
      \brief Callback to deal with the selection of a parameter.
     */

    void              select_parameter                        (void);

  protected:

    /**
      \brief Update (on the screen) the total number of elements in the
             list of parameters.
     */

    void              update_total_elements                   (void);

  protected slots:

    /**
      \brief Sets the row at position rowIndex as the current one, enabling
             the delete button.
      \param rowIndex The number of the row that has been clicked.
     */

    void              row_selected                            (int rowIndex);

  protected:

    /// \brief Action to remove a parameter.

    QAction*                            action_discard_item_;

    /// \brief Action to add new parameters.

    QAction*                            action_select_item_;

    /// \brief Index of the current row (the selected one).

    int                                 cur_row_;

    /// \brief The table model.

    ToolkitEditor_parameter_model*      model_;

    ///  \brief The (selected) parameter.

    toolkit_parameter                   parameter_;

    /// \brief The list of available parameters.

    QList<toolkit_parameter>*           parameters_;

    /// \brief The proxy model used to sort data.

    QSortFilterProxyModel*              proxy_model_;

    /// \brief The table view.

    ToolkitEditor_parameter_table_view* table_view_;

    /// \brief A label to show, on screen, the size of the list
    ///        of parameters.

    QLabel*                             total_elements_;

};

#endif // TOOLKITEDITOR_PARAMETER_SELECTOR_WIDGET_HPP
