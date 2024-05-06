/** \file task_table_model.cpp
  \brief Implementation file for task_table_model.hpp.
*/

#include "task_table_model.hpp"

void
task_table_model::
append_row
(toolkit_task& task)
{
  { 
    QString up_task;

    // We'll always use UPPERCASE task identifiers.

    up_task = QString::fromStdString(task.id).toUpper();
    task.id = up_task.toStdString();

    // Insert.

    int position = tasks_->size();
    beginInsertRows(QModelIndex(), position, position);
    tasks_->append(task);
    endInsertRows();

    return;
  }
}

int
task_table_model::
columnCount
(const QModelIndex &parent )
const
{
  {
    Q_UNUSED(parent);
    return 2;
  }
}

QVariant
task_table_model::
data(const QModelIndex& index,
           int          role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= tasks_->size() || index.row() < 0)
    return QVariant();

  toolkit_task tk = tasks_->at(index.row());;

  if (role == Qt::DisplayRole || role == Qt::EditRole)
  {
    switch (index.column())
    {
      case 0:
        return QString::fromStdString(tk.id);          // Identifier
      case 1:
        return QString::fromStdString(tk.description); // Description.
      default:
        return QVariant();
    }
  }

  return QVariant();
}

QVariant
task_table_model::
headerData(int             section,
           Qt::Orientation orientation,
           int             role) const
{
  if (orientation == Qt::Horizontal)
  {
    if (role == Qt::DisplayRole)
    {
      switch (section)
      {
        case 0:
          return tr("Identifier");
        case 1:
          return tr("Description");
        default:
          return QVariant();
      }
    }
  }

  return QAbstractTableModel::headerData(section, orientation, role);
}

task_table_model::
task_table_model
(QList<toolkit_task>* tasks,
 QObject*             parent) : QAbstractTableModel(parent)
{
  {
    tasks_ = tasks;
  }
}

void
task_table_model::
remove_all_rows
(void)
{
  {
    int total;

    total = tasks_->size();
    beginRemoveRows(QModelIndex(), 0, total-1);
    tasks_->clear();
    endRemoveRows();
  }
}

bool
task_table_model::
removeRow
(int position)
{
  {

    if ((position < 0) || (position >= tasks_->size()))
      return false;

    beginRemoveRows(QModelIndex(), position, position);
    tasks_->removeAt(position);
    endRemoveRows();

    return true;
  }
}

int
task_table_model::
rowCount
(const QModelIndex &parent )
const
{
  {
    Q_UNUSED(parent);
    return tasks_->count();
  }
}
