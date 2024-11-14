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
    // Read the XML file into a string
      ifstream file(filename.toStdString());

      if (!file.is_open())
      {
        error_list_.push_back("Unable to open the input XML workflow file '" + filename.toStdString() + "'");
        return false;
      }

      string buffer((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
      file.close();

      // Try to parse the XML file. Check if there are any errors.

      xml_document<> doc;

      try
      {
        doc.parse<0>(&buffer[0]);
      }
      catch (...)
      {
        error_list_.push_back("Unable to parse the input XML workflow file '" + filename.toStdString() + "'");
        error_list_.push_back("Check that it is a valid WorkflowMaker workflow file.");

        return false;
      }

      //
      // Access the wfm_type element to guarantee that we're reading
      // a workflow file and no other kind of xml file.
      //

      xml_node<> *workflow = doc.first_node("workflow");
      if (workflow == nullptr)
      {
        error_list_.push_back("Unable to parse the input XML workflow file '" + filename.toStdString() + "'");
        error_list_.push_back("Check that it is a valid WorkflowMaker workflow file.");

        return false;
      }

      xml_node<> *wfm_type = workflow->first_node("wfm_type");
      if (wfm_type == nullptr)
      {
        error_list_.push_back("Unable to parse the input XML workflow file '" + filename.toStdString() + "'");
        error_list_.push_back("Check that it is a valid WorkflowMaker workflow file.");
        return false;
      }

      string wfm_type_value = wfm_type->value();
      std::transform(wfm_type_value.begin(), wfm_type_value.end(), wfm_type_value.begin(), ::toupper);

      if (wfm_type_value != "WORKFLOW")
      {
        error_list_.push_back("Input file '" + filename.toStdString() + "' is not a workflow but a " + wfm_type_value);
        error_list_.push_back("Please, select a valid WorkflowMaker workflow file.");
        return false;
      }

      //
      // Now we know we're parsing a workflow file!
      // Parse the general workflow information.
      //

      wf.wfm_version  = stoi(workflow->first_node("wfm_version")->value());
      wf.id           = workflow->first_node("id")->value();
      wf.description  = workflow->first_node("description")->value();
      wf.toolkit_id   = workflow->first_node("toolkit_id")->value();
      wf.last_repo_id = stoi(workflow->first_node("last_repository_id")->value());
      wf.last_task_id = stoi(workflow->first_node("last_task_id")->value());

      std::transform(wf.id.begin()        , wf.id.end()        , wf.id.begin()        , ::toupper);
      std::transform(wf.toolkit_id.begin(), wf.toolkit_id.end(), wf.toolkit_id.begin(), ::toupper);

      // Parse repositories, if any.

      wf.repos.clear();

      xml_node<> *repositories_node = workflow->first_node("repositories");
      if (repositories_node)
      {
        for (xml_node<> *repository_node = workflow->first_node("repositories")->first_node("repository");
            repository_node;
            repository_node = repository_node->next_sibling())
        {
          WFNode rep;

          rep.id           = repository_node->first_node("id")->value();
          rep.numerical_id = stoi(repository_node->first_node("numerical_id")->value());
          rep.pos.x        = stoi(repository_node->first_node("position")->first_node("x")->value());
          rep.pos.y        = stoi(repository_node->first_node("position")->first_node("y")->value());

          std::transform(rep.id.begin(), rep.id.end(), rep.id.begin(), ::toupper);

          wf.repos.push_back(rep);
        }
      }

      // Parse tasks, if any.

      wf.tasks.clear();

      xml_node<> *tasks_node = workflow->first_node("tasks");
      if (tasks_node)
      {
        for (xml_node<> *task_node = workflow->first_node("tasks")->first_node("task");
             task_node;
             task_node = task_node->next_sibling())
        {
          WFNode tsk;

          tsk.id            = task_node->first_node("id")->value();
          tsk.numerical_id  = stoi(task_node->first_node("numerical_id")->value());
          tsk.sequential_id = stoi(task_node->first_node("sequential_id")->value());
          tsk.pos.x         = stoi(task_node->first_node("position")->first_node("x")->value());
          tsk.pos.y         = stoi(task_node->first_node("position")->first_node("y")->value());

          std::transform(tsk.id.begin(), tsk.id.end(), tsk.id.begin(), ::toupper);

          wf.tasks.push_back(tsk);
        }
      }

      // Parse connections, if any.

      wf.connections.clear();

      xml_node<> *connections_node = workflow->first_node("connections");
      if (connections_node)
      {
        for (xml_node<> *connection_node = workflow->first_node("connections")->first_node("connection");
             connection_node;
             connection_node = connection_node->next_sibling())
        {
          WFConnection conn;
          string       node_type;

          node_type = connection_node->first_node("from")->first_node("type")->value();
          std::transform(node_type.begin(), node_type.end(), node_type.begin(), ::toupper);
          if (node_type == "REPOSITORY") conn.from.type = RepoEndPoint;
          else                           conn.from.type = TaskEndPoint;

          conn.from.endpoint_id = stoi(connection_node->first_node("from")->first_node("numerical_id")->value());
          conn.from.slot        = stoi(connection_node->first_node("from")->first_node("position")->value());

          node_type = connection_node->first_node("to")->first_node("type")->value();
          std::transform(node_type.begin(), node_type.end(), node_type.begin(), ::toupper);
          if (node_type == "REPOSITORY") conn.to.type = RepoEndPoint;
          else                           conn.to.type = TaskEndPoint;

          conn.to.endpoint_id = stoi(connection_node->first_node("to")->first_node("numerical_id")->value());
          conn.to.slot        = stoi(connection_node->first_node("to")->first_node("position")->value());

          wf.connections.push_back(conn);
        }
      }

      // That's all.

      return true;
  }
}

workflow_parser::
workflow_parser
(void)
{
  {
    error_list_.clear();
  }
}
