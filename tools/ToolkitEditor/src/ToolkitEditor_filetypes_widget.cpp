/** \file ToolkitEditor_filetypes_widget.cpp
\brief Implementation file for ToolkitEditor_filetypes_widget.hpp
*/

#include "ToolkitEditor_filetypes_widget.hpp"

bool
ToolkitEditor_filetypes_widget::
check_completeness
(void)
{
  bool result;

  {
    result = (filetype_list_->n_elements() > 0);
    if (!result)
    {
      QString message = "File types: the list of file types may not be empty.";
      error_list_.push_back(message);
    }
  }

  return result;
}

void
ToolkitEditor_filetypes_widget::
copy_toolkit_values
(toolkit& tk)
{
  {
    QList<toolkit_file_type>* filetypes;

    filetypes = filetype_list_->get_list();

    tk.file_types.clear();

    for (int i = 0; i < filetypes->size(); i++)
    {
      toolkit_file_type ftyp = filetypes->at(i);
      tk.file_types.push_back(ftyp);
    }
  }
}

QVector<QString>
ToolkitEditor_filetypes_widget::
error_list
(void)
{
  {
    return error_list_;
  }
}

QList<toolkit_file_type>*
ToolkitEditor_filetypes_widget::
get_current_file_types_list
(void)
{
  {
    return filetype_list_->get_list();
  }
}

void
ToolkitEditor_filetypes_widget::
reset_error_list
(void)
{
  {
    error_list_.clear();
  }
}

void
ToolkitEditor_filetypes_widget::
set_task_manager
(ToolkitEditor_tasks_widget* task_mgr)
{
  {
    filetype_list_->set_task_manager(task_mgr);
  }
}

void
ToolkitEditor_filetypes_widget::
set_values
(toolkit& toolkit)
{
  {
    //
    // First of all, remove anything that could have been loaded
    // prior to this call.
    //

    filetype_list_->remove_all();

    // Add the new data.

    for (size_t i = 0; i < toolkit.file_types.size(); i++)
    {
      toolkit_file_type ftyp = toolkit.file_types[i];
      filetype_list_->add(ftyp);
    }
    return;
  }
}

ToolkitEditor_filetypes_widget::
ToolkitEditor_filetypes_widget
(QWidget* parent)
    : QWidget(parent)
{
  {
    //
    // We only have a single widget in our interface.
    //

    filetype_list_ = new ToolkitEditor_filetype_list_widget();

    // Overall layout.

    QVBoxLayout* stretch_layout = new QVBoxLayout;
    stretch_layout->addStretch();

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(filetype_list_);

    main_layout->addLayout(stretch_layout);

    setLayout(main_layout);

    // Window title.

    setWindowTitle("File types");
  }
}

ToolkitEditor_filetypes_widget::
~ToolkitEditor_filetypes_widget
(void)
{
  {
    // Intentionally left blank.
  }
}

bool
ToolkitEditor_filetypes_widget::
validate_semantics
(void)
{
  {
    toolkit_file_type         ftyp;
    QList<toolkit_file_type>* ftypes;
    set<string>               ftypes_set;

    // Get the list of file types from our file types list widget.

    ftypes = filetype_list_->get_list();

    // Check that there is as least ONE file type in such list.

    if (ftypes->size() == 0)
    {
      QString message = "File types: the list of file types may not be empty.";
      error_list_.push_back(message);

      return false;
    }

    // Look for duplicates.

    ftypes_set.clear();

    ftypes_set.insert(ftypes->at(0).id);

    for (int i = 1; i < ftypes->size(); i++)
    {
      set<string>::iterator finder;

      ftyp = ftypes->at(i);
      finder = ftypes_set.find(ftyp.id);
      if (finder != ftypes_set.end())
      {
        QString message;
        message = "File types: repeated file type identifier: " + QString::fromStdString(ftyp.id);
        error_list_.push_back(message);

        return false; // Repeated parameters.
      }

      ftypes_set.insert(ftyp.id);
    }

    return true;
  }
}

