/** \file task_table_model.hpp
  \brief Table model for lists of tasks.
*/

#ifndef TASK_TABLE_MODEL_HPP
#define TASK_TABLE_MODEL_HPP

#include <QAbstractTableModel>
#include <QList>

#include "toolkit_structures.hpp"

/// \brief Table view model for lists of tasks.

class task_table_model : public QAbstractTableModel
{
  public:

    /**
      \brief Add a new row.
      \param[in] task The task to add in the newly created row.
     */

    void          append_row       (toolkit_task& task);

    /**
      \brief Return the number of columns under the given parent.
      \param[in] parent The parent whose columns are about to be tallied.
      \return The number of columns in the model.
     */

    int           columnCount      (const QModelIndex &parent ) const override;

    /**
      \brief Returns the data stored under the given role for the item referred to by the index.
      \param[in] index The index.
      \param[in] role The role.
      \return The data stored under the given role for the item referred to by the index.
     */

    QVariant      data             (const QModelIndex &index, int role) const override;

    /**
      \brief Returns the data for the given role and section in the header with the specified orientation.
      \param[in] section
      \param[in] orientation
      \param[in] role
      \return The data for the given role and section in the header with the specified orientation.

      For horizontal headers, the section number corresponds to the column number.
      Similarly, for vertical headers, the section number corresponds to the row number.
     */

    QVariant      headerData       (int section, Qt::Orientation orientation, int role) const override;

    /**
      \brief Constructor.
      \param[in] tasks Pointer to the list of file types to handle.
      \param[in] parent The parent widget.
     */

                  task_table_model (QList<toolkit_task>* tasks, QObject *parent=0);

    /**
      \brief Remove all the rows in the model.
     */

    void          remove_all_rows  (void);

    /**
      \brief Remove a single row given its position.
      \param[in] position The position of the row to delete.
      \return True if the row is deleted, false otherwise.

      This method fails (returns false) if the index of the
      column to delete is out of range.

     */

    bool          removeRow        (int position);

    /**
      \brief Return the number of rows under the given parent.
      \param[in] parent The parent whose rows are about to be tallied.
      \return The number of rows in the model.
     */

    int           rowCount         (const QModelIndex &parent ) const override;

  protected:

    /// \brief The list of file types to handle.

    QList<toolkit_task>* tasks_;
};

#endif // TASK_TABLE_MODEL_HPP
