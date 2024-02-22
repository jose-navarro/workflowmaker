/** \file file_table_model.cpp
  \brief Implementation file for file_table_model.hpp.
*/

#include "file_table_model.hpp"

void
file_table_model::
append_row
(toolkit_file& file)
{
  { 
    QString up_file;

    // We'll always use UPPERCASE file types.

    up_file = QString::fromStdString(file.file_type_id).toUpper();
    file.file_type_id = up_file.toStdString();

    // Insert.

    int position = files_->size();
    beginInsertRows(QModelIndex(), position, position);
    files_->append(file);
    endInsertRows();

    return;
  }
}

int
file_table_model::
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
file_table_model::
data(const QModelIndex& index,
           int          role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= files_->size() || index.row() < 0)
    return QVariant();

  toolkit_file file = files_->at(index.row());;

  if (role == Qt::DisplayRole || role == Qt::EditRole)
  {
    switch (index.column())
    {
      case 0:
        return QString::fromStdString(file.file_type_id);          // Identifier
      case 1:
        return QString::fromStdString(file.description); // Description.
      default:
        return QVariant();
    }
  }

  return QVariant();
}

file_table_model::
file_table_model
(QList<toolkit_file>* files,
 QObject*             parent) : QAbstractTableModel(parent)
{
  {
    files_ = files;
  }
}

QVariant
file_table_model::
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
          return tr("Description");
        default:
          return QVariant();
      }
    }
  }

  return QAbstractTableModel::headerData(section, orientation, role);
}

void
file_table_model::
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
file_table_model::
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
file_table_model::
rowCount
(const QModelIndex &parent )
const
{
  {
    Q_UNUSED(parent);
    return files_->count();
  }
}
