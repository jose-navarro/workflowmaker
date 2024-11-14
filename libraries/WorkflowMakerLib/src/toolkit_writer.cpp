/** \file toolkit_writer.cpp
\brief Implementation file for toolkit_writer.hpp.
*/

#include "toolkit_writer.hpp"

vector<string>
toolkit_writer::
error_list
(void)
{
  {
    return error_list_;
  }
}

string
toolkit_writer::
to_upper
(string& lower)
{
  {
    QString upper;

    upper = QString::fromStdString(lower).toUpper();
    return upper.toStdString();
  }
}

toolkit_writer::
toolkit_writer
(void)
{
  {
    // Intentionally left blank.
  }
}

bool
toolkit_writer::
write
(toolkit& tk,
 QString&  filename)
{
  {
    string   message;
    ofstream ofile;

    // Try to open the output file.

    ofile.open(filename.toStdString());
    if (!ofile.is_open())
    {
      message = "Error opening '" + filename.toStdString() + "' for writing.";
      error_list_.push_back(message);
      return false;
    }

    //
    // Write the file, section by section.
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

    // Header.

    ofile << header << endl;
    ofile << "<toolkit xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"toolkit.xsd\">" << endl;
    ofile << "  <wfm_type>toolkit</wfm_type>"                         << endl;
    ofile << "  <wfm_version>" << tk.wfm_version  << "</wfm_version>" << endl;
    ofile << "  <id>"          << to_upper(tk.id) << "</id>"          << endl;
    ofile << "  <description>" << tk.description  << "</description>" << endl;

    // Parameters.

    ofile << "  <parameters>" << endl;

    for (size_t i = 0; i < tk.parameters.size(); i++)
    {
      ofile << "    <parameter>" << endl;

      ofile << "      <id>"          << to_upper(tk.parameters[i].id)   << "</id>"          << endl;
      ofile << "      <description>" << tk.parameters[i].description    << "</description>" << endl;
      ofile << "      <type>"        << to_upper(tk.parameters[i].type) << "</type>"        << endl;

      ofile << "    </parameter>" << endl;
    }

    ofile << "  </parameters>" << endl;

    // File types

    ofile << "  <file_types>" << endl;

    for (size_t i = 0; i < tk.file_types.size(); i++)
    {
      ofile << "    <file_type>" << endl;

      ofile << "      <id>"          << to_upper(tk.file_types[i].id) << "</id>"          << endl;
      ofile << "      <description>" << tk.file_types[i].description  << "</description>" << endl;
      ofile << "      <extension>"   << tk.file_types[i].extension    << "</extension>"   << endl;

      ofile << "    </file_type>" << endl;
    }

    ofile << "  </file_types>" << endl;

    // Tasks

    ofile << "  <tasks>" << endl;

    for (size_t i = 0; i < tk.tasks.size(); i++)
    {
      ofile << "    <task>" << endl;

      // Header.

      ofile << "      <id>"                 << to_upper(tk.tasks[i].id)       << "</id>"          << endl;
      ofile << "      <description>"        << tk.tasks[i].description        << "</description>" << endl;
      ofile << "      <name_of_executable>" << tk.tasks[i].name_of_executable << "</name_of_executable>" << endl;

      // Parameters. There may be none.

      if (tk.tasks[i].parameters.size() >0)
      {
        ofile << "      <parameters>" << endl;

        for (size_t j = 0; j < tk.tasks[i].parameters.size(); j++)
          ofile << "        <parameter_id>" << to_upper(tk.tasks[i].parameters[j]) << "</parameter_id>" << endl;

        ofile << "      </parameters>" << endl;
      }

      // Input files. Again, there may be none.

      if (tk.tasks[i].input_files.size() > 0)
      {
        ofile << "      <input_files>" << endl;

        for (size_t j = 0; j < tk.tasks[i].input_files.size(); j++)
        {
          ofile << "        <input_file>" << endl;

          ofile << "          <file_type_id>"       << to_upper(tk.tasks[i].input_files[j].file_type_id)       << "</file_type_id>"       << endl;
          ofile << "          <description>"        << tk.tasks[i].input_files[j].description                  << "</description>"        << endl;
          ofile << "          <options_file_label>" << to_upper(tk.tasks[i].input_files[j].options_file_label) << "</options_file_label>" << endl;

          ofile << "        </input_file>" << endl;
        }

        ofile << "      </input_files>" << endl;
      }

      // Output files.

      ofile << "      <output_files>" << endl;

      for (size_t j = 0; j < tk.tasks[i].output_files.size(); j++)
      {
        ofile << "        <output_file>" << endl;

        ofile << "          <file_type_id>"       << to_upper(tk.tasks[i].output_files[j].file_type_id)       << "</file_type_id>"       << endl;
        ofile << "          <description>"        << tk.tasks[i].output_files[j].description                  << "</description>"        << endl;
        ofile << "          <options_file_label>" << to_upper(tk.tasks[i].output_files[j].options_file_label) << "</options_file_label>" << endl;

        ofile << "        </output_file>" << endl;
      }

      ofile << "      </output_files>" << endl;


      // Closing task tag.

      ofile << "    </task>" << endl;
    }

    // Closing tasks tag.

    ofile << "  </tasks>" << endl;


    // Closing toolkit tag.

    ofile << "</toolkit>" << endl;

    // That's all.

    return true;
  }
}
