/** \file ToolkitEditor_parameter_model.hpp
  \brief Implementation file for ToolkitEditor_parameter_model.cpp.
*/

#include "ToolkitEditor_parameter_model.hpp"

void
ToolkitEditor_parameter_model::
append_row
(toolkit_parameter& parameter)
{
  { 
    QString up_param;

    // We'll always use UPPERCASE parameter identifiers.

    up_param = QString::fromStdString(parameter.id).toUpper();
    parameter.id = up_param.toStdString();

    // Insert.

    int position = parameters_->size();
    beginInsertRows(QModelIndex(), position, position);
    parameters_->append(parameter);
    endInsertRows();

    return;
  }
}

int
ToolkitEditor_parameter_model::
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
ToolkitEditor_parameter_model::
data(const QModelIndex& index,
           int          role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= parameters_->size() || index.row() < 0)
    return QVariant();

  toolkit_parameter p = parameters_->at(index.row());;

  if (role == Qt::DisplayRole || role == Qt::EditRole)
  {
    switch (index.column())
    {
      case 0:
        return QString::fromStdString(p.id);          // Identifier
      case 1:
        return QString::fromStdString(p.type);        // Type
      case 2:
        return QString::fromStdString(p.description); // Description.
      default:
        return QVariant();
    }
  }

  return QVariant();
}

Qt::ItemFlags
ToolkitEditor_parameter_model::
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
ToolkitEditor_parameter_model::
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
          return tr("Type");
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
ToolkitEditor_parameter_model::
remove_all_rows
(void)
{
  {
    int total;

    total = parameters_->size();
    beginRemoveRows(QModelIndex(), 0, total-1);
    parameters_->clear();
    endRemoveRows();
  }
}

bool
ToolkitEditor_parameter_model::
removeRow
(int position)
{
  {

    if ((position < 0) || (position >= parameters_->size()))
      return false;

    beginRemoveRows(QModelIndex(), position, position);
    parameters_->removeAt(position);
    endRemoveRows();

    return true;
  }
}

int
ToolkitEditor_parameter_model::
rowCount(const QModelIndex &parent ) const
{
  {
    Q_UNUSED(parent);
    return parameters_->count();
  }
}

bool
ToolkitEditor_parameter_model::
setData
(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole)
  {
     const int row = index.row();
     toolkit_parameter par = parameters_->at(row);

     switch (index.column()) {
      case 0:
        par.id = value.toString().toStdString();
        break;
      case 1:
        par.type = value.toString().toStdString();
        break;
      case 2:
        par.description = value.toString().toStdString();
        break;
      default:
        return false;
    }

    parameters_->replace(row, par);
    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return true;
  }

  return false;
}

ToolkitEditor_parameter_model::
ToolkitEditor_parameter_model
(QList<toolkit_parameter>* parameters,
 QObject*                  parent) : QAbstractTableModel(parent)
{
  {
    parameters_ = parameters;
  }
}
