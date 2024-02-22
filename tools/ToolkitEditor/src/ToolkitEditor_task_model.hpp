/** \file ToolkitEditor_task_model.cpp
  \brief Table view model for lists of tasks.
*/

#ifndef TOOLKITEDITOR_TASK_MODEL_HPP
#define TOOLKITEDITOR_TASK_MODEL_HPP

#include <QAbstractTableModel>
#include <QStringList>
#include <QList>
#include <QString>

#include "toolkit_structures.hpp"

#include <vector>
#include <string>

using namespace std;

/*
 The next two Q_DECLARE_METATYPE statements are provided
 to make possible the conversion from QVariant to the respective
 vector types indicated.
 */

Q_DECLARE_METATYPE(vector<string>)
Q_DECLARE_METATYPE(vector<toolkit_file>)

/// \brief Table view model for lists of tasks.

class ToolkitEditor_task_model : public QAbstractTableModel
{
  public:

    /**
      \brief Add a new row.
      \param task The task to add in the new row.
     */

    void          append_row               (toolkit_task& task);

    /**
      \brief Return the number of columns under the given parent.
      \param parent The parent whose columns are about to be tallied.
      \return The number of columns in the model.
     */

    int           columnCount              (const QModelIndex& parent ) const override;

    /**
      \brief Returns the data stored under the given role for the item referred to by the index.
      \param index The index.
      \param role The role.
      \return The data stored under the given role for the item referred to by the index.
     */

    QVariant      data                     (const QModelIndex& index,
                                                  int          role) const override;

    /**
      \brief Returns the item flags for the given index.
      \param index The index whose flags must be returned.
      \return The flags for the given index.

      This method is overriden to make the model editable.
     */

    Qt::ItemFlags flags                    (const QModelIndex&   index) const override;

    /**
      \brief Returns the data for the given role and section in the header with the specified orientation.
      \param section
      \param orientation
      \param role
      \return The data for the given role and section in the header with the specified orientation.

      For horizontal headers, the section number corresponds to the column number.
      Similarly, for vertical headers, the section number corresponds to the row number.
     */

    QVariant      headerData               (int                  section,
                                            Qt::Orientation      orientation,
                                            int                  role) const override;

    /**
      \brief Remove all the rows in the model.
     */

    void          remove_all_rows          (void);

    /**
      \brief Remove a single row given its position.
      \param position The position of the row to delete.
      \return True if the row is deleted, false otherwise.

      This method fails (returns false) if the index of the
      column to delete is out of range.

     */

    bool          removeRow                (int                  position);

    /**
      \brief Return the number of rows under the given parent.
      \param parent The parent whose rows are about to be tallied.
      \return The number of rows in the model.
     */

    int           rowCount                        (const QModelIndex&   parent ) const override;

    /**
      \brief Sets the role data for the item at index to value.
      \param index The index.
      \param value The value.
      \param role The role.
      \return True if the data is set, false otherwise.

      This method is overriden to make the model editable.
     */

    bool          setData                  (const QModelIndex&   index,
                                            const QVariant&      value,
                                                  int            role = Qt::EditRole) override;

    /**
      \brief Constructor.
      \param tasks Pointer to the list of tasks to handle.
      \param parent The parent widget.
     */

                  ToolkitEditor_task_model (QList<toolkit_task>* tasks,
                                            QObject*             parent = nullptr);

  protected:

    /// \brief The list of tasks to handle.

    QList<toolkit_task>* tasks_;
};

#endif // TOOLKITEDITOR_TASK_MODEL_HPP
