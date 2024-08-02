/** \file toolkit_parser.cpp
\brief Implementation file for toolkit_parser.hpp.
*/

#include "toolkit_parser.hpp"

vector<string>
toolkit_parser::
error_list
(void)
{
  {
    return error_list_;
  }
}

bool
toolkit_parser::
parse
(QString& filename,
 toolkit& tk)
{
  {
    // Read the XML file into a string

    ifstream file(filename.toStdString());

    if (!file.is_open())
    {
      error_list_.push_back("Unable to open the input XML toolkit file '" + filename.toStdString() + "'");
      return false;
    }

    string buffer((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
    file.close();

    // Parse the XML file

    xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    // Access the toolkit element
    xml_node<> *toolkit = doc.first_node("toolkit");

    // Parse the toolkit's general information

    tk.id          = toolkit->first_node("id")->value();
    tk.description = toolkit->first_node("description")->value();

    std::transform(tk.id.begin(), tk.id.end(), tk.id.begin(), ::toupper);

    // Parse the list of unique parameters

    tk.parameters.clear();

    for (xml_node<> *parameter = toolkit->first_node("parameters")->first_node("parameter");
         parameter;
         parameter = parameter->next_sibling())
    {
      toolkit_parameter tp;

      tp.id          = parameter->first_node("id")->value();
      tp.description = parameter->first_node("description")->value();
      tp.type        = parameter->first_node("type")->value();

      std::transform(tp.id.begin()  , tp.id.end()  , tp.id.begin()  , ::toupper);
      std::transform(tp.type.begin(), tp.type.end(), tp.type.begin(), ::toupper);

      tk.parameters.push_back(tp);
    }

    // Parse the list of unique file types

    tk.file_types.clear();

    for (xml_node<> *file_type = toolkit->first_node("file_types")->first_node("file_type");
         file_type;
         file_type = file_type->next_sibling())
    {
      toolkit_file_type tft;

      tft.id          = file_type->first_node("id")->value();
      tft.description = file_type->first_node("description")->value();
      tft.extension   = file_type->first_node("extension")->value();

      std::transform(tft.id.begin(), tft.id.end(), tft.id.begin(), ::toupper);

      tk.file_types.push_back(tft);
    }

    // Parse the list of tasks

    tk.tasks.clear();

    for (xml_node<> *task = toolkit->first_node("tasks")->first_node("task");
         task;
         task = task->next_sibling())
    {
      toolkit_task tt;

      tt.id                 = task->first_node("id")->value();
      tt.description        = task->first_node("description")->value();
      tt.name_of_executable = task->first_node("name_of_executable")->value();

      std::transform(tt.id.begin(), tt.id.end(), tt.id.begin(), ::toupper);

      // Check whether the task has parameters. If so, parse its identifiers.

      tt.parameters.clear();

      xml_node<> *parameters_node = task->first_node("parameters");
      if (parameters_node)
      {
        // Parse parameter identifiers, since we know that we've got at least one parameter.

        for (xml_node<> *parameter_id = task->first_node("parameters")->first_node("parameter_id");
             parameter_id;
             parameter_id = parameter_id->next_sibling())
        {
          string parid = parameter_id->value();
          std::transform(parid.begin(), parid.end(), parid.begin(), ::toupper);
          tt.parameters.push_back(parid);
        }
      }

      // Check whether we have input files, since there may be none.

      tt.input_files.clear();

      xml_node<> *input_files_node = task->first_node("input_files");
      if (input_files_node)
      {
        // Parse the input files, now that we're sure that there's one at least.

        for (xml_node<> *input_file = task->first_node("input_files")->first_node("input_file");
             input_file;
             input_file = input_file->next_sibling())
        {
          toolkit_file tf;

          tf.file_type_id       = input_file->first_node("file_type_id")->value();
          tf.description        = input_file->first_node("description")->value();
          tf.options_file_label = input_file->first_node("options_file_label")->value();

          std::transform(tf.file_type_id.begin(),
                         tf.file_type_id.end(),
                         tf.file_type_id.begin(), ::toupper);

          std::transform(tf.options_file_label.begin(),
                         tf.options_file_label.end(),
                         tf.options_file_label.begin(), ::toupper);

          tt.input_files.push_back(tf);
        }
      }

      // Parse output files. There will be at least one.

      tt.output_files.clear();

      for (xml_node<> *output_file = task->first_node("output_files")->first_node("output_file");
           output_file;
           output_file = output_file->next_sibling())
      {
        toolkit_file tf;

        tf.file_type_id       = output_file->first_node("file_type_id")->value();
        tf.description        = output_file->first_node("description")->value();
        tf.options_file_label = output_file->first_node("options_file_label")->value();

        std::transform(tf.file_type_id.begin(),
                       tf.file_type_id.end(),
                       tf.file_type_id.begin(), ::toupper);

        std::transform(tf.options_file_label.begin(),
                       tf.options_file_label.end(),
                       tf.options_file_label.begin(), ::toupper);

        tt.output_files.push_back(tf);
      }

      tk.tasks.push_back(tt);
    }

    // Validate the toolkit and return.

    return validate_toolkit(tk);
  }
}

toolkit_parser::
toolkit_parser
(void)
{
  {
    error_list_.clear();
  }
}

bool
toolkit_parser::
validate_toolkit
(toolkit& tk)
{
  {
    set<string>           file_types_set;
    set<string>::iterator finder;
    set<string>           parameter_set;
    set<string>           tasks_set;
    set<string>           taskpar_set;
    int                   total_errors;

    total_errors = 0;

    // Check that the identifiers of the parameters are unique.

    parameter_set.clear();
    for (size_t i = 0; i < tk.parameters.size(); i++)
    {
      finder = parameter_set.find(tk.parameters[i].id);
      if (finder != parameter_set.end())
      {
        string message;
        message = "Repeated PARAMETER identifier: " + tk.parameters[i].id;
        error_list_.push_back(message);
        total_errors++;
      }
      else
      {
        parameter_set.insert(tk.parameters[i].id);
      }
    }

    // Check that the identifiers of the file types are unique.

    file_types_set.clear();
    for (size_t i = 0; i < tk.file_types.size(); i++)
    {
      finder = file_types_set.find(tk.file_types[i].id);
      if (finder != file_types_set.end())
      {
        string message;
        message = "Repeated FILE_TYPE identifier: " + tk.file_types[i].id;
        error_list_.push_back(message);
        total_errors++;
      }
      else
      {
        file_types_set.insert(tk.file_types[i].id);
      }
    }

    // Check that the identifiers of the tasks are unique.

    tasks_set.clear();
    for (size_t i = 0; i < tk.tasks.size(); i++)
    {
      finder = tasks_set.find(tk.tasks[i].id);
      if (finder != tasks_set.end())
      {
        string message;
        message = "Repeated TASK identifier: " + tk.tasks[i].id;
        error_list_.push_back(message);
        total_errors++;
      }
      else
      {
        tasks_set.insert(tk.tasks[i].id);
      }
    }

    // Check that the parameters reported by the tasks DO exist.


    for (size_t i = 0; i < tk.tasks.size(); i++)
    {
      for (size_t j = 0; j < tk.tasks[i].parameters.size(); j++)
      {
        string check = tk.tasks[i].parameters[j];

        finder = parameter_set.find(check);
        if (finder == parameter_set.end())
        {
          string message;
          message = "Task with id " + tk.tasks[i].id + " declares NON-EXISTENT PARAMETER: " + check;
          error_list_.push_back(message);
          total_errors++;
        }
      }
    }

    //
    // Check that the parameters reported by the tasks are not repeated
    // (for each task, not across the toolkit).
    //

    for (size_t i = 0; i < tk.tasks.size(); i++)
    {
      taskpar_set.clear();

      for (size_t j = 0; j < tk.tasks[i].parameters.size(); j++)
      {
        string check = tk.tasks[i].parameters[j];

        finder = taskpar_set.find(check);
        if (finder != taskpar_set.end())
        {
          string message;
          message = "Task with id " + tk.tasks[i].id + " declares A REPEATED PARAMETER: " + check;
          error_list_.push_back(message);
          total_errors++;
        }
        else
        {
          taskpar_set.insert(check);
        }
      }
    }

    // Check that the file_types reported by the tasks DO exist.

    for (size_t i = 0; i < tk.tasks.size(); i++)
    {
      for (size_t j = 0; j < tk.tasks[i].input_files.size(); j++)
      {
        string check = tk.tasks[i].input_files[j].file_type_id;

        finder = file_types_set.find(check);
        if (finder == file_types_set.end())
        {
          string message;
          message = "Task with id " + tk.tasks[i].id + " declares NON-EXISTENT INPUT FILE TYPE: " + check;
          error_list_.push_back(message);
          total_errors++;
        }
      }

      for (size_t j = 0; j < tk.tasks[i].output_files.size(); j++)
      {
        string check = tk.tasks[i].output_files[j].file_type_id;

        finder = file_types_set.find(check);
        if (finder == file_types_set.end())
        {
          string message;
          message = "Task with id " + tk.tasks[i].id + " declares NON-EXISTENT OUTPUT FILE TYPE: " + check;
          error_list_.push_back(message);
          total_errors++;
        }
      }
    }

    // That's all.

    return (total_errors == 0);
  }
}
