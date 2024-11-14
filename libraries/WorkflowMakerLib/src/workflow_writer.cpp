/** \file workflow_writer.cpp
\brief Implementation file for workflow_writer.hpp.
*/

#include "workflow_writer.hpp"

vector<string>
workflow_writer::
error_list
(void)
{
  {
    return error_list_;
  }
}

string
workflow_writer::
to_upper
(string& lower)
{
  {
    QString upper;

    upper = QString::fromStdString(lower).toUpper();
    return upper.toStdString();
  }
}

workflow_writer::
workflow_writer
(void)
{
  {
    // Intentionally left blank.
  }
}

bool
workflow_writer::
write
(WFWorkflow& wf,
 QString&    filename)
{
  {
    string   message;
    ofstream wffile;

    // Try to open the output file.

    wffile.open(filename.toStdString());
    if (!wffile.is_open())
    {
      message = "Error opening '" + filename.toStdString() + "' for writing.";
      error_list_.push_back(message);
      return false;
    }

    //
    // Write the workflow.
    //

    //
    // Header, opening tag. Note that the encoding of the output file
    // depends on the development environment. At the moment this
    // code was written, Visual Studio 2015 produced ISO-8859-1 files,
    // while gcc created UTF-8 ones. So we'll behave differently
    // according to the compiler we're using...
    //

    #ifdef __GNUC__
      // Linux + gcc.
      string header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    #endif

    #ifdef _MSC_VER
      // Windows + MSVC.
      string header = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
    #endif

    wffile << header;
    wffile << "<workflow xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" << endl;
    wffile << "  xsi:noNamespaceSchemaLocation=\"workflow.xsd\">" << endl;

    // Type, version, Identifiers and description.

    wffile << "  <wfm_type>workflow</wfm_type>" << endl;
    wffile << "  <wfm_version>1</wfm_version>" << endl;
    wffile << "  <id>" << to_upper(wf.id) << "</id>" << endl;
    wffile << "  <description>" << wf.description << "</description>" << endl;
    wffile << "  <toolkit_id>" << to_upper(wf.toolkit_id) << "</toolkit_id>" << endl;

    // Last repo & task ids.

    wffile << "  <last_repository_id>" << wf.last_repo_id << "</last_repository_id>" << endl;
    wffile << "  <last_task_id>" << wf.last_task_id << "</last_task_id>" << endl;

    // Repositories, if any.

    if (wf.repos.size() > 0)
    {
      wffile << "  <repositories>" << endl;

      for (size_t i = 0; i < wf.repos.size(); i++)
      {
        WFNode repo = wf.repos[i];

        wffile << "    <repository>" << endl;
        wffile << "      <id>" << to_upper(repo.id) << "</id>" << endl;
        wffile << "      <numerical_id>" << repo.numerical_id << "</numerical_id>" << endl;
        wffile << "      <position>" << endl;
        wffile << "        <x>" << repo.pos.x << "</x>" << endl;
        wffile << "        <y>" << repo.pos.y << "</y>" << endl;
        wffile << "      </position>" << endl;
        wffile << "    </repository>" << endl;
      }

      wffile << "  </repositories>" << endl;
    }

    // Tasks, if any.

    if (wf.tasks.size() > 0)
    {
      wffile << "  <tasks>" << endl;

      for (size_t i = 0; i < wf.tasks.size(); i++)
      {
        WFNode task = wf.tasks[i];

        wffile << "    <task>" << endl;
        wffile << "      <id>" << to_upper(task.id) << "</id>" << endl;
        wffile << "      <numerical_id>" << task.numerical_id << "</numerical_id>" << endl;
        wffile << "      <sequential_id>" << task.sequential_id << "</sequential_id>" << endl;
        wffile << "      <position>" << endl;
        wffile << "        <x>" << task.pos.x << "</x>" << endl;
        wffile << "        <y>" << task.pos.y << "</y>" << endl;
        wffile << "      </position>" << endl;
        wffile << "    </task>" << endl;
      }

      wffile << "  </tasks>" << endl;
    }

    // Connections, if any.

    if (wf.connections.size() > 0)
    {
      wffile << "  <connections>" << endl;

      for (size_t i = 0; i < wf.connections.size(); i++)
      {
        WFConnection conn = wf.connections[i];
        string type1, type2;

        if (conn.from.type == TaskEndPoint) type1 = "task";
        else                                type1 = "repository";

        if (conn.to.type   == TaskEndPoint) type2 = "task";
        else                                type2 = "repository";

        wffile << "    <connection>" << endl;
        wffile << "      <from>" << endl;
        wffile << "        <type>" << type1 << "</type>" << endl;
        wffile << "        <numerical_id>" << conn.from.endpoint_id << "</numerical_id>" << endl;
        wffile << "        <position>" << conn.from.slot << "</position>" << endl;
        wffile << "      </from>" << endl;
        wffile << "      <to>" << endl;
        wffile << "        <type>" << type2 << "</type>" << endl;
        wffile << "        <numerical_id>" << conn.to.endpoint_id << "</numerical_id>" << endl;
        wffile << "        <position>" << conn.to.slot << "</position>" << endl;
        wffile << "      </to>" << endl;
        wffile << "    </connection>" << endl;
      }

      wffile << "  </connections>" << endl;
    }

    // Closing tag.

     wffile << "</workflow>" << endl;


    // Close the options file.

    wffile.close();

    // That's all.

    return true;
  }
}
