#include <fstream>
#include <string>

#include "to_uppercase_options_file_reader.hpp"

using namespace std;

int
convert_to_uppercase
(const string& input_file,
 const string& output_file)
{
  {
    // Open input and output files

    ifstream infile(input_file);

    if (!infile.is_open())
      return 1;

    ofstream outfile(output_file);
    if (!outfile.is_open())
    {
      infile.close();
      return 2;
    }

    // Read each line and convert to uppercase

    string line;
    while (std::getline(infile, line))
    {
      for (char& c : line)
      {
        c = std::toupper(c);
      }
      outfile << line << endl;
    }

    // Close files

    infile.close();
    outfile.close();

    // That's all.

    return 0;
  }
}

int
main
(int   argc,
 char* argv[])
{
  {
    to_uppercase_options             options;
    string                           options_file;
    to_uppercase_options_file_reader options_reader;
    int                              status;
    //
    // Check for correct number of arguments. We expect just one, the
    // name of the options file. We'll ignore anything else.
    //

    if (argc < 2)
      return 1; // 1 means "Failure".

    // Get the name of the options file.

    options_file = argv[1];

    //
    // Now, parse the options file. There, the name of the
    // input and output files are stored. These are saved
    // our "options" structure.
    //

    status = options_reader.parse_file(options_file, options);
    if (status != 0)
      return 1; // 1 means "Failure".

    //
    // Create the output file, with the same contents that the input
    // one, but with everything changed to uppercase.
    //

    status = convert_to_uppercase(options.input_file_name, options.output_filename);

    if (status != 0)
      return 1; // 1 means "Failure".

    // That's all!

    return 0; // 0 means "Success".
  }
}
