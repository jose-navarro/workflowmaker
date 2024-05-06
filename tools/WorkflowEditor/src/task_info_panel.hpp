/** \file task_info_panel.hpp
  \brief Widget showing a list of tasks and, for each of these, their
         input and output files.
*/

#ifndef TASK_INFO_PANEL_HPP
#define TASK_INFO_PANEL_HPP

#include <QGridLayout>
#include <QWidget>

#include "task_list_widget.hpp"
#include "file_list_widget.hpp"

#include <QDebug>

/// \brief Widget showing a list of tasks and, for each of these, their
///        input and output files.

class task_info_panel : public QWidget
{
    Q_OBJECT

  public:

    /// \brief Set the toolkit to show (which includes the definitions
    ///        of the tasks and their input and output files).

    void     set_toolkit    (toolkit& tk);

    /// \brief Constructor.
    /**
      \param[in] parent Pointer to the parent widget.
     */

    explicit task_info_panel(QWidget *parent = nullptr);

  signals:

    /// \brief Signals that a row has been selected.
    /**
      \param[in] row The number of the row that has been selected.
     */

    void     row_selected    (int row);

  protected slots:

    /// \brief React when a row is selected in the tasks panel.
    /**
      \param[in] row The number of the row that has been selected.
     */

    void     selected_row    (int row);

  protected:

    /// \brief The list of input files for the current task.

    file_list_widget* files_input_;

    /// \brief The list of output files for the current task.

    file_list_widget* files_output_;

    /// \brief The list of available tasks.

    task_list_widget* tasks_;

    /// \brief The toolkit the task panel is showing.

    toolkit           tk_;

};

#endif // TASK_INFO_PANEL_HPP
