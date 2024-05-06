#include <fstream>
#include <string>

#include "cut_tail_options_file_reader.hpp"

using namespace std;

int
cut_tail
(const string& input_file,
 const string& output_file,
       int     max_lines)
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

    //
    // Read at most max_lines lines and write these to
    // the output file.
    //

    int tot_lines_written = 0;

    string line;
    while (std::getline(infile, line))
    {
      outfile << line << endl;
      tot_lines_written++;

      if (tot_lines_written == max_lines)
        break;
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
    cut_tail_options             options;
    string                       options_file;
    cut_tail_options_file_reader options_reader;
    int                          status;

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

    status = cut_tail (options.input_file_name,
                       options.output_filename,
                       options.max_lines);

    if (status != 0)
      return 1; // 1 means "Failure".

    // That's all!

    return 0; // 0 means "Success".
  }
}
