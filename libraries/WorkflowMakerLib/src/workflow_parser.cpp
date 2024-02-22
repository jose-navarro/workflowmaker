/** \file workflow_parser.cpp
\brief Implementation file for workflow_parser.hpp.
*/

#include "workflow_parser.hpp"

vector<string>
workflow_parser::
error_list
(void)
{
  {
    return error_list_;
  }
}

bool
workflow_parser::
parse
(QString&    filename,
 WFWorkflow& wf)
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

      workflow_parser_message_handler message_handler;

      QXmlSchemaValidator validator(schema_);
      validator.setMessageHandler(&message_handler);

      if (!validator.validate(&file, QUrl::fromLocalFile(file.fileName())))
      {

        string message;

        message = "'" + filename.toStdString() + "' is not a valid workflow XML definition file.";
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
    wf.id = element.text().toUpper().toStdString();

    element = root.firstChildElement("description");
    wf.description = element.text().toStdString();

    element = root.firstChildElement("toolkit_id");
    wf.toolkit_id = element.text().toStdString();

    element = root.firstChildElement("last_repository_id");
    wf.last_repo_id = element.text().trimmed().toInt();

    element = root.firstChildElement("last_task_id");
    wf.last_task_id = element.text().trimmed().toInt();

    // Get the list of repositories, if any.

    wf.repos.clear();

    QDomNodeList nodes;

    element = root.firstChildElement("repositories");
    if (!element.isNull())
    {
      nodes = element.elementsByTagName("repository");

      for(int i = 0; i < nodes.count(); i++)
      {
        QDomNode elm = nodes.at(i);

        if(elm.isElement())
        {
          QDomElement e = elm.toElement();

          QDomElement id           = e.firstChildElement("id");
          QDomElement numerical_id = e.firstChildElement("numerical_id");
          QDomElement pos          = e.firstChildElement("position");
          QDomElement x            = pos.firstChildElement("x");
          QDomElement y            = pos.firstChildElement("y");

          WFNode       rep;

          rep.id           = id.text().toUpper().toStdString();
          rep.numerical_id = numerical_id.text().toInt();
          rep.pos.x        = x.text().toInt();
          rep.pos.y        = y.text().toInt();

          wf.repos.push_back(rep);
        }
      }
    }

    // Get the list of tasks, if any.

    wf.tasks.clear();

    element = root.firstChildElement("tasks");
    if (!element.isNull())
    {
      nodes = element.elementsByTagName("task");

      for(int i = 0; i < nodes.count(); i++)
      {
        QDomNode elm = nodes.at(i);

        if(elm.isElement())
        {
          QDomElement e = elm.toElement();

          QDomElement id            = e.firstChildElement("id");
          QDomElement numerical_id  = e.firstChildElement("numerical_id");
          QDomElement sequential_id = e.firstChildElement("sequential_id");
          QDomElement pos           = e.firstChildElement("position");
          QDomElement x             = pos.firstChildElement("x");
          QDomElement y             = pos.firstChildElement("y");

          WFNode       tsk;

          tsk.id            = id.text().toUpper().toStdString();
          tsk.numerical_id  = numerical_id.text().toInt();
          tsk.sequential_id = sequential_id.text().toInt();
          tsk.pos.x         = x.text().toInt();
          tsk.pos.y         = y.text().toInt();

          wf.tasks.push_back(tsk);
        }
      }
    }

    // Get the list of connections, if any.

    wf.connections.clear();

    element = root.firstChildElement("connections");
    if (!element.isNull())
    {
      nodes = element.elementsByTagName("connection");

      for(int i = 0; i < nodes.count(); i++)
      {
        QDomNode elm = nodes.at(i);

        if(elm.isElement())
        {
          WFConnection conn;
          QString      node_type;

          QDomElement e    = elm.toElement();
          QDomElement from = e.firstChildElement("from");
          QDomElement to   = e.firstChildElement("to");

          QDomElement type = from.firstChildElement("type");
          QDomElement nid  = from.firstChildElement("numerical_id");
          QDomElement pos  = from.firstChildElement("position");

          node_type = type.text().toUpper();
          if (node_type == "REPOSITORY") conn.from.type = RepoEndPoint;
          else                           conn.from.type = TaskEndPoint;

          conn.from.endpoint_id = nid.text().toInt();
          conn.from.slot        = pos.text().toInt();

          type = to.firstChildElement("type");
          nid  = to.firstChildElement("numerical_id");
          pos  = to.firstChildElement("position");

          node_type = type.text().toUpper();
          if (node_type == "REPOSITORY") conn.to.type = RepoEndPoint;
          else                           conn.to.type = TaskEndPoint;

          conn.to.endpoint_id = nid.text().toInt();
          conn.to.slot        = pos.text().toInt();

          wf.connections.push_back(conn);
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
workflow_parser::
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

workflow_parser::
workflow_parser
(void)
{
  {
    got_schema_ = false;
  }
}
