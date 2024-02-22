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
(QString& filename, toolkit& tk)
{
  {
    // If we've got a validating schema, try to validate our XML document.

    if (got_schema_)
    {
      QFile file(filename);
      if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
      {
        error_list_.push_back("Unable to open the input file '" + filename.toStdString() + "'");
        return false;
      }

      toolkit_parser_message_handler message_handler;

      QXmlSchemaValidator validator(schema_);
      validator.setMessageHandler(&message_handler);

      if (!validator.validate(&file, QUrl::fromLocalFile(file.fileName())))
      {
        string message;

        message = "'" + filename.toStdString() + "' is not a valid toolkit XML definition file.";
        error_list_.push_back(message);

        message = "  These are the errors detected:";
        error_list_.push_back(message);

        for (size_t i = 0; i < message_handler.error_total(); i++)
        {
          string       scolumn;
          string       sline;
          stringstream ss1;
          stringstream ss2;

          ss1 << message_handler.error_line(i);
          sline = ss1.str();

          ss2 << message_handler.error_column(i);
          scolumn = ss2.str();

          message = "    " + message_handler.error_message(i)
                           + "(line "    + sline
                           + ", column " + scolumn
                           + ")";

          error_list_.push_back(message);
        }

        file.close();
        return false;
      }

      file.close();
    }

    // Open the actual file with the XML data.

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      error_list_.push_back("Unable to open the input file '" + filename.toStdString() + "'");
      return false;
    }

    // Parse our input XML file.

    QDomDocument document;
    int          column;
    QString      error;
    int          line;
    QString      message;

    if(!document.setContent(&file, &error, &line, &column))
    {
      // Unable to load the input file.

      file.close();

      message = "  " + error + ". Line: " + QString::number(line) + ". Column: " + QString::number(column);

      error_list_.push_back("Error parsing '" + filename.toStdString() + "'");
      error_list_.push_back(message.toStdString());

      return false;
    }

    // Get the document's root element

    QDomElement root = document.firstChildElement();

    // Get the identifier and description of the toolkit.

    QDomElement element;

    element = root.firstChildElement("id");
    tk.id = element.text().toUpper().toStdString();

    element = root.firstChildElement("description");
    tk.description = element.text().toStdString();

    // Get the list of parameters.

    tk.parameters.clear();

    QDomNodeList nodes;

    element = root.firstChildElement("parameters");
    nodes = element.elementsByTagName("parameter");

    for(int i = 0; i < nodes.count(); i++)
    {
      QDomNode elm = nodes.at(i);

      if(elm.isElement())
      {
        QDomElement e = elm.toElement();

        QDomElement id          = e.firstChildElement("id");
        QDomElement description = e.firstChildElement("description");
        QDomElement type        = e.firstChildElement("type");

        toolkit_parameter tp;
        tp.id          = id.text().toUpper().toStdString();
        tp.description = description.text().toStdString();
        tp.type        = type.text().toUpper().toStdString();

        tk.parameters.push_back(tp);
      }
    }

    // Get the list of file types

    tk.file_types.clear();

    element = root.firstChildElement("file_types");
    nodes = element.elementsByTagName("file_type");

    for(int i = 0; i < nodes.count(); i++)
    {
      QDomNode elm = nodes.at(i);

      if(elm.isElement())
      {
        QDomElement e = elm.toElement();

        QDomElement id          = e.firstChildElement("id");
        QDomElement description = e.firstChildElement("description");
        QDomElement extension   = e.firstChildElement("extension");

        toolkit_file_type tft;
        tft.id          = id.text().toUpper().toStdString();
        tft.description = description.text().toStdString();
        tft.extension   = extension.text().toStdString();

        tk.file_types.push_back(tft);
      }
    }

    // Get the list of tasks

    tk.tasks.clear();

    element = root.firstChildElement("tasks");
    nodes = element.elementsByTagName("task");

    for(int i = 0; i < nodes.count(); i++)
    {
      QDomNode elm = nodes.at(i);

      if(elm.isElement())
      {
        QDomElement e = elm.toElement();

        QDomElement id          = e.firstChildElement("id");
        QDomElement description = e.firstChildElement("description");
        QDomElement name_of_executable = e.firstChildElement("name_of_executable");

        toolkit_task tt;
        tt.id                 = id.text().toUpper().toStdString();
        tt.description        = description.text().toStdString();
        tt.name_of_executable = name_of_executable.text().toStdString();

        // The parameters. There may be none.

        tt.parameters.clear();

        QDomElement  pars   = e.firstChildElement("parameters");

        if (!pars.isNull())
        {
          QDomNodeList parids = pars.elementsByTagName("parameter_id");

          for (int j = 0; j < parids.count(); j++)
          {
            QDomNode parelm = parids.at(j);

            if (parelm.isElement())
            {
              QDomElement parid = parelm.toElement();
              tt.parameters.push_back(parid.text().toUpper().toStdString());
            }
          }
        }

        // The input files. Again, there may be none.

        tt.input_files.clear();

        QDomElement  ifiles    = e.firstChildElement("input_files");

        if (!ifiles.isNull())
        {
          QDomNodeList ifilelist = ifiles.elementsByTagName("input_file");

          for (int j = 0; j < ifilelist.count(); j++)
          {
            QDomNode ifile = ifilelist.at(j);

            if (ifile.isElement())
            {
              QDomElement ifilelem = ifile.toElement();

              QDomElement file_type_id       = ifilelem.firstChildElement("file_type_id");
              QDomElement description        = ifilelem.firstChildElement("description");
              QDomElement options_file_label = ifilelem.firstChildElement("options_file_label");

              toolkit_file tf;

              tf.file_type_id       = file_type_id.text().toUpper().toStdString();
              tf.description        = description.text().toStdString();
              tf.options_file_label = options_file_label.text().toUpper().toStdString();

              tt.input_files.push_back(tf);
            }
          }
        }

        // The output files.

        tt.output_files.clear();

        QDomElement  ofiles    = e.firstChildElement("output_files");
        QDomNodeList ofilelist = ofiles.elementsByTagName("output_file");

        for (int j = 0; j < ofilelist.count(); j++)
        {
          QDomNode ofile = ofilelist.at(j);

          if (ofile.isElement())
          {
            QDomElement ofilelem = ofile.toElement();

            QDomElement file_type_id       = ofilelem.firstChildElement("file_type_id");
            QDomElement description        = ofilelem.firstChildElement("description");
            QDomElement options_file_label = ofilelem.firstChildElement("options_file_label");

            toolkit_file tf;

            tf.file_type_id       = file_type_id.text().toUpper().toStdString();
            tf.description        = description.text().toStdString();
            tf.options_file_label = options_file_label.text().toUpper().toStdString();

            tt.output_files.push_back(tf);
          }
        }

        // We've parsed the task completely. Add it to our list of tasks.

        tk.tasks.push_back(tt);
      }
    }

    // Close the input file.

    file.close();

    // Validate the toolkit and return.

    return validate_toolkit(tk);

  }
}

bool
toolkit_parser::
set_schema
(QString& path_to_schema)
{
  {
    QUrl schema_url;

    // Transform the path to the schema file to a valid URL.

    schema_url = QUrl::fromLocalFile(path_to_schema);

    // Try to load the schema.

    schema_.load(schema_url);
    if (!schema_.isValid())
    {
      string message;

      message = "Unable to set the validating schema at '" + schema_url.toString().toStdString() + "'";
      error_list_.push_back(message);
      got_schema_ = false;

      return false;
    }

    // We've got a validating schema!

    got_schema_ = true;

    // That's all.

    return true;
  }
}

toolkit_parser::
toolkit_parser
(void)
{
  {
    got_schema_ = false;
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
