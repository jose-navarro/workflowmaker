/** \file ToolkitEditor_filetype_model.hpp
  \brief Implementation file for ToolkitEditor_filetype_model.cpp.
*/

#include "ToolkitEditor_filetype_model.hpp"

void
ToolkitEditor_filetype_model::
append_row
(toolkit_file_type& filetype)
{
  { 
    QString up_filetype;

    // We'll always use UPPERCASE file type identifiers.

    up_filetype = QString::fromStdString(filetype.id).toUpper();
    filetype.id = up_filetype.toStdString();

    // Insert.

    int position = filetypes_->size();
    beginInsertRows(QModelIndex(), position, position);
    filetypes_->append(filetype);
    endInsertRows();

    return;
  }
}

int
ToolkitEditor_filetype_model::
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
ToolkitEditor_filetype_model::
data(const QModelIndex& index,
           int          role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= filetypes_->size() || index.row() < 0)
    return QVariant();

  toolkit_file_type f = filetypes_->at(index.row());;

  if (role == Qt::DisplayRole || role == Qt::EditRole)
  {
    switch (index.column())
    {
      case 0:
        return QString::fromStdString(f.id);          // Identifier
      case 1:
        return QString::fromStdString(f.extension);   // Default extension.
      case 2:
        return QString::fromStdString(f.description); // Description.
      default:
        return QVariant();
    }
  }

  return QVariant();
}

Qt::ItemFlags
ToolkitEditor_filetype_model::
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
ToolkitEditor_filetype_model::
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
          return tr("Extension");
        case 2:
          return tr("Description");
        default:
          return QVariant();
      }
    }
  }

  //if (role == Qt::InitialSortOrderRole)
  //  return Qt::AscendingOrder;

  return QAbstractTableModel::headerData(section, orientation, role);
}

ToolkitEditor_filetype_model::
ToolkitEditor_filetype_model
(QList<toolkit_file_type>* filetypes,
 QObject*                  parent) : QAbstractTableModel(parent)
{
  {
    filetypes_ = filetypes;
  }
}

void
ToolkitEditor_filetype_model::
remove_all_rows
(void)
{
  {
    int total;

    total = filetypes_->size();
    beginRemoveRows(QModelIndex(), 0, total-1);
    filetypes_->clear();
    endRemoveRows();
  }
}

bool
ToolkitEditor_filetype_model::
removeRow
(int position)
{
  {

    if ((position < 0) || (position >= filetypes_->size()))
      return false;

    beginRemoveRows(QModelIndex(), position, position);
    filetypes_->removeAt(position);
    endRemoveRows();

    return true;
  }
}

int
ToolkitEditor_filetype_model::
rowCount(const QModelIndex &parent ) const
{
  {
    Q_UNUSED(parent);
    return filetypes_->count();
  }
}

bool
ToolkitEditor_filetype_model::
setData
(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole)
  {
     const int row = index.row();
     toolkit_file_type ftyp = filetypes_->at(row);

     switch (index.column()) {
      case 0:
        ftyp.id          = value.toString().toStdString();
        break;
      case 1:
        ftyp.extension   = value.toString().toStdString();
        break;
      case 2:
        ftyp.description = value.toString().toStdString();
        break;
      default:
        return false;
    }

    filetypes_->replace(row, ftyp);
    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return true;
  }

  return false;
}
