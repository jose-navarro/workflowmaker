/** \file launcher_parser.cpp
\brief Implementation file for launcher_parser.hpp.
*/

#include "launcher_parser.hpp"

vector<string>
launcher_parser::
error_list
(void)
{
  {
    return error_list_;
  }
}

bool
launcher_parser::
parse
(QString&    filename,
 WLLauncher& lch)
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

      launcher_parser_message_handler message_handler;

      QXmlSchemaValidator validator(schema_);
      validator.setMessageHandler(&message_handler);

      if (!validator.validate(&file, QUrl::fromLocalFile(file.fileName())))
      {

        string message;

        message = "'" + filename.toStdString() + "' is not a valid launcher XML definition file.";
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
    lch.id = element.text().toUpper().toStdString();

    element = root.firstChildElement("description");
    lch.description = element.text().toStdString();

    element = root.firstChildElement("workflow_id");
    lch.workflow_id = element.text().toStdString();

    // Get the list of parameters, if any.

    lch.parameters.clear();

    QDomNodeList nodes;

    element = root.firstChildElement("parameters");
    if (!element.isNull())
    {
      nodes = element.elementsByTagName("parameter");

      for(int i = 0; i < nodes.count(); i++)
      {
        QDomNode elm = nodes.at(i);

        if(elm.isElement())
        {
          QDomElement  e     = elm.toElement();

          QDomElement  id    = e.firstChildElement("id");
          QDomElement  value = e.firstChildElement("value");
          QDomElement  t_id  = e.firstChildElement("task_id");
          QDomElement  t_sid = e.firstChildElement("task_sequential_id");

          WLParameter par;

          par.id         = id.text().toUpper().toStdString();
          par.value      = value.text().toStdString();
          par.task_id    = t_id.text().toUpper().toStdString();
          par.task_seqid = t_sid.text().toStdString();

          lch.parameters.push_back(par);
        }
      }
    }

    // Get the list of repositories, if any.

    lch.repositories.clear();

    element = root.firstChildElement("repositories");
    if (!element.isNull())
    {
      nodes = element.elementsByTagName("repository");

      for(int i = 0; i < nodes.count(); i++)
      {
        QDomNode elm = nodes.at(i);

        if(elm.isElement())
        {
          QDomElement  e = elm.toElement();

          QDomElement  id           = e.firstChildElement("id");
          QDomElement  numerical_id = e.firstChildElement("numerical_id");
          QDomElement  path         = e.firstChildElement("path");

          WLRepository rep;

          rep.id   = id.text().toUpper().toStdString();
          rep.nid  = numerical_id.text().toInt();
          rep.path = path.text().toStdString();

          lch.repositories.push_back(rep);
        }
      }
    }

    // Get the list of connections (files), if any.

    lch.files.clear();

    element = root.firstChildElement("connections");
    if (!element.isNull())
    {
      nodes = element.elementsByTagName("connection");

      for(int i = 0; i < nodes.count(); i++)
      {
        QDomNode elm = nodes.at(i);

        if(elm.isElement())
        {
          WLFile      file;
          QString     node_type;

          QDomElement e        = elm.toElement();
          QDomElement from     = e.firstChildElement("from");
          QDomElement to       = e.firstChildElement("to");
          QDomElement filename = e.firstChildElement("file_name");

          QDomElement type     = from.firstChildElement("type");
          QDomElement nid      = from.firstChildElement("numerical_id");
          QDomElement slot     = from.firstChildElement("slot");

          node_type = type.text().toUpper();
          if (node_type == "REPOSITORY") file.start_node_type = RepoEndPoint;
          else                           file.start_node_type = TaskEndPoint;

          file.start_node_nid  = nid.text().toInt();
          file.start_node_slot = slot.text().toInt();

          type  = to.firstChildElement("type");
          nid   = to.firstChildElement("numerical_id");
          slot  = to.firstChildElement("slot");

          node_type = type.text().toUpper();
          if (node_type == "REPOSITORY") file.end_node_type = RepoEndPoint;
          else                           file.end_node_type = TaskEndPoint;

          file.end_node_nid  = nid.text().toInt();
          file.end_node_slot = slot.text().toInt();

          file.filename = filename.text().toStdString();

          lch.files.push_back(file);
        }
      }
    }

    // Close the input file.

    file.close();

    // That's all

    return true;
  }
}

bool
launcher_parser::
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

launcher_parser::
launcher_parser
(void)
{
  {
    got_schema_ = false;
  }
}
