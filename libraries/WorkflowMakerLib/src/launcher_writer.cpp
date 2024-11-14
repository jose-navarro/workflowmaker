/** \file launcher_writer.cpp
\brief Implementation file for launcher_writer.hpp.
*/

#include "launcher_writer.hpp"

vector<string>
launcher_writer::
error_list
(void)
{
  {
    return error_list_;
  }
}

string
launcher_writer::
to_upper
(string& lower)
{
  {
    QString upper;

    upper = QString::fromStdString(lower).toUpper();
    return upper.toStdString();
  }
}

launcher_writer::
launcher_writer
(void)
{
  {
    // Intentionally left blank.
  }
}

bool
launcher_writer::
write
(WLLauncher& launcher,
 QString&    filename)
{
  {
    string   message;
    ofstream lchfile;

    // Try to open the output file.

    lchfile.open(filename.toStdString());
    if (!lchfile.is_open())
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

    lchfile << header;
    lchfile << "<launcher xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" << endl;
    lchfile << "  xsi:noNamespaceSchemaLocation=\"launcher.xsd\">" << endl;

    // Type, version, identifiers and description.

    lchfile << "  <wfm_type>launcher</wfm_type>"                                       << endl;
    lchfile << "  <wfm_version>1</wfm_version>"                                        << endl;
    lchfile << "  <id>"          << to_upper(launcher.id)          << "</id>"          << endl;
    lchfile << "  <description>" << launcher.description           << "</description>" << endl;
    lchfile << "  <workflow_id>" << to_upper(launcher.workflow_id) << "</workflow_id>" << endl;

    // Parameters, if any.

    if (launcher.parameters.size() > 0)
    {
      lchfile << "  <parameters>" << endl;

      for (size_t i = 0; i < launcher.parameters.size(); i++)
      {
        WLParameter param = launcher.parameters[i];

        lchfile << "    <parameter>" << endl;
        lchfile << "      <id>"                 << to_upper(param.id) << "</id>"                 << endl;
        lchfile << "      <value>"              << param.value        << "</value>"              << endl;
        lchfile << "      <task_id>"            << param.task_id      << "</task_id>"            << endl;
        lchfile << "      <task_sequential_id>" << param.task_seqid   << "</task_sequential_id>" << endl;
        lchfile << "    </parameter>"                                                            << endl;
      }

      lchfile << "  </parameters>" << endl;

    }

    // Repositories, if any.

    if (launcher.repositories.size() > 0)
    {
      lchfile << "  <repositories>" << endl;

      for (size_t i = 0; i < launcher.repositories.size(); i++)
      {
        WLRepository repo = launcher.repositories[i];

        lchfile << "    <repository>" << endl;
        lchfile << "      <id>"           << to_upper(repo.id) << "</id>"           << endl;
        lchfile << "      <numerical_id>" << repo.nid          << "</numerical_id>" << endl;
        lchfile << "      <path>"         << repo.path         << "</path>"         << endl;
        lchfile << "    </repository>" << endl;
      }

      lchfile << "  </repositories>" << endl;
    }

    // Connections (files, in fact), if any.

    if (launcher.files.size() > 0)
    {
      lchfile << "  <connections>" << endl;

      for (size_t i = 0; i < launcher.files.size(); i++)
      {
        WLFile file = launcher.files[i];
        string type1, type2;

        if (file.start_node_type == TaskEndPoint) type1 = "task";
        else                                      type1 = "repository";

        if (file.end_node_type   == TaskEndPoint) type2 = "task";
        else                                      type2 = "repository";

        lchfile << "    <connection>"                                                    << endl;
        lchfile << "      <from>"                                                        << endl;
        lchfile << "        <type>"         << type1                << "</type>"         << endl;
        lchfile << "        <numerical_id>" << file.start_node_nid  << "</numerical_id>" << endl;
        lchfile << "        <slot>"         << file.start_node_slot << "</slot>"         << endl;
        lchfile << "      </from>"                                                       << endl;
        lchfile << "      <to>"                                                          << endl;
        lchfile << "        <type>"         << type2                << "</type>"         << endl;
        lchfile << "        <numerical_id>" << file.end_node_nid    << "</numerical_id>" << endl;
        lchfile << "        <slot>"         << file.end_node_slot   << "</slot>"         << endl;
        lchfile << "      </to>"                                                         << endl;
        lchfile << "      <file_name>"      << file.filename        << "</file_name>"    << endl;
        lchfile << "    </connection>"                                                   << endl;
      }

      lchfile << "  </connections>" << endl;
    }

    // Closing tag.

     lchfile << "</launcher>" << endl;


    // Close the options file.

    lchfile.close();

    // That's all.

    return true;
  }
}
