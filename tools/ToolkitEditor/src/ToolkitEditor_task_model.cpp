/** \file ToolkitEditor_task_model.hpp
  \brief Implementation file for ToolkitEditor_task_model.cpp.
*/

#include "ToolkitEditor_task_model.hpp"

void
ToolkitEditor_task_model::
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
ToolkitEditor_task_model::
columnCount
(const QModelIndex &parent )
const
{
  {
    Q_UNUSED(parent);
    return 6;
  }
}

QVariant
ToolkitEditor_task_model::
data(const QModelIndex& index,
           int          role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= tasks_->size() || index.row() < 0)
    return QVariant();

  toolkit_task tsk = tasks_->at(index.row());;

  if (role == Qt::DisplayRole || role == Qt::EditRole)
  {
    switch (index.column())
    {
      case 0:
        return QString::fromStdString(tsk.id);
      case 1:
        return QString::fromStdString(tsk.description);
      case 2:
        return QString::fromStdString(tsk.name_of_executable);
      case 3:
        return QString::number(tsk.parameters.size());
      case 4:
        return QString::number(tsk.input_files.size());
      case 5:
        return QString::number(tsk.output_files.size());
      default:
        return QVariant();
    }
  }

  return QVariant();
}

Qt::ItemFlags
ToolkitEditor_task_model::
flags
(const QModelIndex &index)
const
{
  {
    if (!index.isValid())
      return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
  }
}

QVariant
ToolkitEditor_task_model::
headerData
(int             section,
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
        case 2:
          return tr("Name of executable");
        case 3:
          return tr("# parameters");
        case 4:
          return tr("# input files");
        case 5:
          return tr("# output files");
        default:
          return QVariant();
      }
    }
  }

  return QAbstractTableModel::headerData(section, orientation, role);
}

void
ToolkitEditor_task_model::
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
ToolkitEditor_task_model::
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
ToolkitEditor_task_model::
rowCount(const QModelIndex &parent ) const
{
  {
    Q_UNUSED(parent);
    return tasks_->count();
  }
}

bool
ToolkitEditor_task_model::
setData
(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole)
  {
     const int row = index.row();
     toolkit_task tsk = tasks_->at(row);

     switch (index.column()) {
      case 0:
        tsk.id = value.toString().toStdString();
        break;
      case 1:
        tsk.description = value.toString().toStdString();
        break;
      case 2:
        tsk.name_of_executable = value.toString().toStdString();
        break;
      case 3:
        tsk.parameters.clear();
        tsk.parameters = value.value<vector<string>>();
        break;
      case 4:
        tsk.input_files.clear();
        tsk.input_files = value.value<vector<toolkit_file>>();
        break;
      case 5:
        tsk.output_files.clear();
        tsk.output_files = value.value<vector<toolkit_file>>();
        break;
      default:
        return false;
    }

    tasks_->replace(row, tsk);
    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return true;
  }

  return false;
}

ToolkitEditor_task_model::
ToolkitEditor_task_model
(QList<toolkit_task>* tasks,
 QObject*             parent) : QAbstractTableModel(parent)
{
  {
    tasks_ = tasks;
  }
}
