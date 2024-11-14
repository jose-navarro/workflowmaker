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
    // Read the XML file into a string

    ifstream file(filename.toStdString());

    if (!file.is_open())
    {
      error_list_.push_back("Unable to open the input XML launcher file '" + filename.toStdString() + "'");
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
      error_list_.push_back("Unable to parse the input XML launcher file '" + filename.toStdString() + "'");
      error_list_.push_back("Check that it is a valid WorkflowMaker launcher file.");
      return false;
    }

    //
    // Access the wfm_type element to guarantee that we're reading
    // a launcher file and no other kind of xml file.
    //

    xml_node<> *launcher = doc.first_node("launcher");
    if (launcher == nullptr)
    {
      error_list_.push_back("Unable to parse the input XML launcher file '" + filename.toStdString() + "'");
      error_list_.push_back("Check that it is a valid WorkflowMaker launcher file.");
      return false;
    }

    xml_node<> *wfm_type = launcher->first_node("wfm_type");
    if (wfm_type == nullptr)
    {
      error_list_.push_back("Unable to parse the input XML launcher file '" + filename.toStdString() + "'");
      error_list_.push_back("Check that it is a valid WorkflowMaker launcher file.");
      return false;
    }

    string wfm_type_value = wfm_type->value();
    std::transform(wfm_type_value.begin(), wfm_type_value.end(), wfm_type_value.begin(), ::toupper);

    if (wfm_type_value != "LAUNCHER")
    {
      error_list_.push_back("Input file '" + filename.toStdString() + "' is not a launcher but a " + wfm_type_value);
      error_list_.push_back("Please, select a valid WorkflowMaker launcher file.");
      return false;
    }

    //
    // Now we know we're parsing a launcher file!
    // Parse the general launcher information.
    //

    lch.wfm_version = stoi(launcher->first_node("wfm_version")->value());
    lch.id          = launcher->first_node("id")->value();
    lch.description = launcher->first_node("description")->value();
    lch.workflow_id = launcher->first_node("workflow_id")->value();

    std::transform(lch.id.begin()         , lch.id.end()         , lch.id.begin(), ::toupper);
    std::transform(lch.workflow_id.begin(), lch.workflow_id.end(), lch.workflow_id.begin(), ::toupper);

    // Parse parameters, if any.

    lch.parameters.clear();

    xml_node<> *parameters_node = launcher->first_node("parameters");
    if (parameters_node)
    {
      for (xml_node<> *parameter_node = launcher->first_node("parameters")->first_node("parameter");
           parameter_node;
           parameter_node = parameter_node->next_sibling())
      {
        WLParameter par;
        par.id         = parameter_node->first_node("id")->value();
        par.value      = parameter_node->first_node("value")->value();
        par.task_id    = parameter_node->first_node("task_id")->value();
        par.task_seqid = parameter_node->first_node("task_sequential_id")->value();

        std::transform(par.id.begin()     , par.id.end()     , par.id.begin()     , ::toupper);
        std::transform(par.task_id.begin(), par.task_id.end(), par.task_id.begin(), ::toupper);

        lch.parameters.push_back(par);
      }
    }

    // Parse repositories, if any.

    lch.repositories.clear();

    xml_node<> *repositories_node = launcher->first_node("repositories");
    if (repositories_node)
    {
      for (xml_node<> *repository_node = launcher->first_node("repositories")->first_node("repository"); repository_node; repository_node = repository_node->next_sibling()) {

        WLRepository rep;

        rep.id   = repository_node->first_node("id")->value();
        rep.nid  = stoi(repository_node->first_node("numerical_id")->value());
        rep.path = repository_node->first_node("path")->value();

        std::transform(rep.id.begin(), rep.id.end(), rep.id.begin(), ::toupper);

        lch.repositories.push_back(rep);
      }
    }

    // Parse connections, if any.

    lch.files.clear();

    xml_node<> *files_node = launcher->first_node("connections");
    if (files_node)
    {
      for (xml_node<> *connection_node = launcher->first_node("connections")->first_node("connection");
           connection_node;
           connection_node = connection_node->next_sibling())
      {
        WLFile file;
        string node_type;

        node_type = connection_node->first_node("from")->first_node("type")->value();
        std::transform(node_type.begin(), node_type.end(), node_type.begin(), ::toupper);

        if (node_type == "REPOSITORY") file.start_node_type = RepoEndPoint;
        else                           file.start_node_type = TaskEndPoint;

        file.start_node_nid  = stoi(connection_node->first_node("from")->first_node("numerical_id")->value());
        file.start_node_slot = stoi(connection_node->first_node("from")->first_node("slot")->value());

        node_type = connection_node->first_node("to")->first_node("type")->value();
        std::transform(node_type.begin(), node_type.end(), node_type.begin(), ::toupper);

        if (node_type == "REPOSITORY") file.end_node_type = RepoEndPoint;
        else                           file.end_node_type = TaskEndPoint;

        file.end_node_nid  = stoi(connection_node->first_node("to")->first_node("numerical_id")->value());
        file.end_node_slot = stoi(connection_node->first_node("to")->first_node("slot")->value());

        file.filename = connection_node->first_node("file_name")->value();

        lch.files.push_back(file);
      }
    }

    // That's all.

    return true;
  }
}

launcher_parser::
launcher_parser
(void)
{
  {
    error_list_.clear();
  }
}
