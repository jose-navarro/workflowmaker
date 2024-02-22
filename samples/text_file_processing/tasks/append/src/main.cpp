#include <fstream>
#include <string>

#include "append_options_file_reader.hpp"

using namespace std;

int
append_lines
(const string& input_file1,
 const string& input_file2,
 const string& output_file)
{
  {
    // Open input and output files

    ifstream infile1(input_file1);

    if (!infile1.is_open())
      return 1;

    ifstream infile2(input_file2);

    if (!infile2.is_open())
    {
      infile1.close();
      return 2;
    }

    ofstream outfile(output_file);
    if (!outfile.is_open())
    {
      infile1.close();
      infile2.close();
      return 3;
    }

    // Copy the lines from the first input file.

    string line;

    while (getline(infile1, line))
      outfile << line << endl;

    // Copy the lines from the second input file.

    while (getline(infile2, line))
      outfile << line << endl;

    // Close files

    infile1.close();
    infile2.close();
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
    append_options             options;
    string                     options_file;
    append_options_file_reader options_reader;
    int                        status;

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

    status = append_lines(options.input_file_name_1,
                          options.input_file_name_2,
                          options.output_filename);

    if (status != 0)
      return 1; // 1 means "Failure".

    // That's all!

    return 0; // 0 means "Success".
  }
}
