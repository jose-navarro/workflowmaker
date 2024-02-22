#include <fstream>
#include <string>

#include "interleave_options_file_reader.hpp"

using namespace std;

int
interleave_lines
(const string& input_file1,
 const string& input_file2,
 const string& output_file,
       bool    copy_queue)
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

    //
    // Read lines from both files simultaneously,
    // interleaving these in the output file.
    // Stop when no more lines exist in any of
    // the two files.
    //

    string line1;
    string line2;

    while (getline(infile1, line1) && getline(infile2, line2))
    {
      outfile << line1 << endl;
      outfile << line2 << endl;

      line1 = "";
      line2 = "";
    }

    //
    // If there are any remaining lines in any of the input
    // files, copy these if parameter copy_queue says so.
    // Otherwise, we're over.
    //

    if (copy_queue)
    {
      //
      // First, check that there was no remaining line pending to write
      // that had been read in the loop above.
      //

      if (line1 != "")
        outfile << line1 << endl;
      else if (line2 != "")
        outfile << line2 << endl;

      //
      // Now, take care of any possible remaining lines in any
      // of the input files.
      //

      if (getline(infile1, line1))
      {
        outfile << line1 << endl;
        while (getline(infile1, line1))
          outfile << line1 << endl;
      }
      else if (getline(infile2, line2))
      {
        outfile << line2 << endl;
        while (getline(infile2, line2))
          outfile << line2 << endl;
      }
    }

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
    interleave_options             options;
    string                         options_file;
    interleave_options_file_reader options_reader;
    int                            status;
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

    status = interleave_lines(options.input_file_name_1,
                              options.input_file_name_2,
                              options.output_filename,
                              options.copy_queue);

    if (status != 0)
      return 1; // 1 means "Failure".

    // That's all!

    return 0; // 0 means "Success".
  }
}
