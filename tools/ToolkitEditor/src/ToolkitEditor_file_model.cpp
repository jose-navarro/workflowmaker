/** \file ToolkitEditor_file_model.hpp
  \brief Implementation file for ToolkitEditor_file_model.cpp.
*/

#include "ToolkitEditor_file_model.hpp"

void
ToolkitEditor_file_model::
append_row
(toolkit_file& file)
{
  { 
    QString up_param;

    // We'll always use UPPERCASE parameter identifiers.

    up_param = QString::fromStdString(file.file_type_id).toUpper();
    file.file_type_id = up_param.toStdString();

    // Insert.

    int position = files_->size();
    beginInsertRows(QModelIndex(), position, position);
    files_->append(file);
    endInsertRows();

    return;
  }
}

int
ToolkitEditor_file_model::
columnCount
(const QModelIndex &parent )
const
{
  {
    Q_UNUSED(parent);
    return 3;
  }
}

QVariant
ToolkitEditor_file_model::
data(const QModelIndex& index,
           int          role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= files_->size() || index.row() < 0)
    return QVariant();

  toolkit_file fil = files_->at(index.row());;

  if (role == Qt::DisplayRole || role == Qt::EditRole)
  {
    switch (index.column())
    {
      case 0:
        return QString::fromStdString(fil.file_type_id);
      case 1:
        return QString::fromStdString(fil.options_file_label);
      case 2:
        return QString::fromStdString(fil.description);
      default:
        return QVariant();
    }
  }

  return QVariant();
}

Qt::ItemFlags
ToolkitEditor_file_model::
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
ToolkitEditor_file_model::
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
          return tr("File type");
        case 1:
          return tr("Options file label");
        case 2:
          return tr("Description");
        default:
          return QVariant();
      }
    }
  }

  return QAbstractTableModel::headerData(section, orientation, role);
}

void
ToolkitEditor_file_model::
remove_all_rows
(void)
{
  {
    int total;

    total = files_->size();
    beginRemoveRows(QModelIndex(), 0, total-1);
    files_->clear();
    endRemoveRows();
  }
}

bool
ToolkitEditor_file_model::
removeRow
(int position)
{
  {

    if ((position < 0) || (position >= files_->size()))
      return false;

    beginRemoveRows(QModelIndex(), position, position);
    files_->removeAt(position);
    endRemoveRows();

    return true;
  }
}

int
ToolkitEditor_file_model::
rowCount(const QModelIndex &parent ) const
{
  {
    Q_UNUSED(parent);
    return files_->count();
  }
}

bool
ToolkitEditor_file_model::
setData
(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole)
  {
     const int row = index.row();
     toolkit_file fil = files_->at(row);

     switch (index.column()) {
      case 0:
        fil.file_type_id = value.toString().toStdString();
        break;
      case 1:
        fil.options_file_label = value.toString().toStdString();
        break;
      case 2:
        fil.description = value.toString().toStdString();
        break;
      default:
        return false;
    }

    files_->replace(row, fil);
    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return true;
  }

  return false;
}

ToolkitEditor_file_model::
ToolkitEditor_file_model
(QList<toolkit_file>* files,
 QObject*             parent) : QAbstractTableModel(parent)
{
  {
    files_ = files;
  }
}
