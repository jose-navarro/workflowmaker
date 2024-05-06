/** \file simple_options_file_parser.hpp
\brief Simple parser for label / value option files.
*/

#ifndef _SIMPLE_OPTIONS_FILE_PARSER_
#define _SIMPLE_OPTIONS_FILE_PARSER_

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

/// \brief Simple parser for label / value option files.
/**
  This class implements a very simple option files parser.

  These option files must be text files. Values are input by means of unique labels.
  The specific format to write a label and its corresponding value is:

  label = value

  All the elements in the previous example (label, equal sign, value) must be written
  in the same line.

  Multiple labels must be written in consecutive lines. For instance:

  label1 = value1
  label2 = value2

  Values may be of type string, integer or double. Additionally, list of integers and doubles
  are allowed. These lists must separate the different integer o double values by means
  of spaces. For instance:

  my_integer_list = 1 2 3 4
  my_double_list  = 1.2 3.4 5.6 7.8 9.0

  When the values are strings, the leading a trailing spaces are removed. Internal
  whitespace, whowever is fully preserved. Therefore, in the following example:

  my_string_value =       this is    a   test

  the actual value processed by this class would be "this is    a   test". Note
  how the leading and trailing spaces have been removed, but the internal
  whitespace has not.

  Labels are case sensitive. Therefore, label "my_value" is not equal to "MY_VALUE" nor to
  "mY_vAlUe".

  Blank lines are ignored. It is possible to write comments; lines whose first character
  is "#" will be considered as comments. For example:

  \# This is a comment.

  To use this class,

  - Declare / instantiate a simple_options_file_parser object.
  - Call method parse() to load the options file into memory.
  - Call the different get_xxx_xxx() methods to retrieve the value
    of the labels of interest, using the proper type (int, double, string,
    int_array or double_array) for the kind of value expected.

*/
class simple_options_file_parser
{
  public:

    /// \brief Retrieve the value of a label as a double value.
    /**
      \param option_label The label identifying the option sought.
      \param option_value The value sought. It is only meaningful
             when the method finishes correctly.
      \return An error code:
        - 0: Successful completion.
        - 1: Label not found. The label in "option_label" does not
             exist in the options file.
        - 2: The value related to the label can not be interpreted
             as a double value.

      This method tries to convert the string representing the value
      to double format. If such string is not a double, an error
      code will be returned.

      Moreover, an error code is also returned if the label sought
      is not found. In this case, it is important to remember that
      labels are case sensitive.
    */

    int      get_option_double           (const string&        option_label,
                                          double&              option_value);

    /// \brief Retrieve the value of a label as a list of double values.
    /**
      \param option_label The label identifying the option sought.
      \param option_value The array of values sought. It is only meaningful
             when the method finishes correctly.
      \return An error code:
        - 0: Successful completion.
        - 1: Label not found. The label in "option_label" does not
             exist in the options file.
        - 2: One or more values in the expected list of doubles
             can not be interpreted as a double value.

      This method tries to convert the string representing a list
      of double values separated by spaces to its array of doubles
      equivalent. If one or more of the components in the string
      cannot be interpreted as a double value, an error code will
      be returned. On the contrary, when such conversion works,
      the output option_value array will contain as many values
      as doubles found in the list. The number of values may
      be obtained by calling method size() of option_value.

      Moreover, an error code is also returned if the label sought
      is not found. In this case, it is important to remember that
      labels are case sensitive.
    */

    int      get_option_double_array     (const string&        option_label,
                                          vector<double>&      option_value);

    /// \brief Retrieve the value of a label as an int value.
    /**
      \param option_label The label identifying the option sought.
      \param option_value The value sought. It is only meaningful
             when the method finishes correctly.
      \return An error code:
        - 0: Successful completion.
        - 1: Label not found. The label in "option_label" does not
             exist in the options file.
        - 2: The value related to the label can not be interpreted
             as an int value.

      This method tries to convert the string representing the value
      to int format. If such string is not an int, an error
      code will be returned.

      Moreover, an error code is also returned if the label sought
      is not found. In this case, it is important to remember that
      labels are case sensitive.
    */

    int      get_option_int              (const string&        option_label,
                                          int&                 option_value);

    /// \brief Retrieve the value of a label as a list of int values.
    /**
      \param option_label The label identifying the option sought.
      \param option_value The array of values sought. It is only meaningful
             when the method finishes correctly.
      \return An error code:
        - 0: Successful completion.
        - 1: Label not found. The label in "option_label" does not
             exist in the options file.
        - 2: One or more values in the expected list of int
             can not be interpreted as an int value.

      This method tries to convert the string representing a list
      of int values separated by spaces to its array of int
      equivalent. If one or more of the components in the string
      cannot be interpreted as an int value, an error code will
      be returned. On the contrary, when such conversion works,
      the output option_value array will contain as many values
      as int found in the list. The number of values may
      be obtained by calling method size() of option_value.

      Moreover, an error code is also returned if the label sought
      is not found. In this case, it is important to remember that
      labels are case sensitive.
    */

    int      get_option_int_array        (const string&        option_label,
                                          vector<int>&         option_value);

    /// \brief Retrieve the value of a label as a string.
    /**
      \param option_label The label identifying the option sought.
      \param option_value The value sought. It is only meaningful
             when the method finishes correctly.
      \return An error code:
        - 0: Successful completion.
        - 1: Label not found. The label in "option_label" does not
             exist in the options file.

      This method never tries to convert the text value in the
      options file, so no errors must be expected on this side.

      However, an error code is also returned if the label sought
      is not found. In this case, it is important to remember that
      labels are case sensitive.
    */
    int      get_option_string           (const string&        option_label,
                                          string&              option_value);

    /// \brief Retrieve the words in a string as separate elements.
    /**
      \param option_label The label identifying the option sought.
      \param option_value The array of values sought. It is only meaningful
             when the method finishes correctly.
      \return An error code:
        - 0: Successful completion.
        - 1: Label not found. The label in "option_label" does not
             exist in the options file.

      This method splits all the words in a string into a list
      of strings. That is, the string "This is a string" is
      returned as the list of separate values "This", "is", "a"
      and "string". Leading and trailing Whitespace, as well as
      that found between words is completely removed.

      An error code is  returned if the label sought
      is not found. In this case, it is important to remember that
      labels are case sensitive.
    */

    int      get_option_string_array     (const string&        option_label,
                                          vector<string>&      option_value);

    /// \brief Load and interpret a simple options file.
    /**
      \param options_file The name of the file containing the options to load.
      \param error_line If an error is detected while loading the options file,
             this parameter will contain the line number where the aforementioned
             error was detected.
      \return An error code:
        - 0: Successful completion.
        - 1: The name of the options file is empty (parameter options_file;
             no error line is set).
        - 2: Error opening the input options file (no error line is set).
        - 3: Missing equal sign ("=") when parsing a label / value pair.
        - 4: Empty label or value found while parsing a label / value pair.
    */
    int      parse                       (const string&        options_file,
                                          int&                 error_line);

    /// \brief Default constructor.

             simple_options_file_parser  (void);

    /// \brief Destructor.

             ~simple_options_file_parser (void);

  protected:

    /// \brief Retrieve the next line with actual data to process.
    /**
      \param file The file to read the lines from.
      \param line The line of data retrieved.
      \param last_line_read Number of the last line read.
      \param line_data Number of the line where a line of data starts
             (may differ from line_read if comments are found). Used
             to report the line when syntax errors are detected
             when processing a label / value pair.
      \return True if a new line has been retrieved, false if
             the end-of-file condition is arised.

      This methods reads complete lines from the input file, skipping
      those that begin with a comment sign (#).

      Normally, data lines (those that contain an statement
      label \= value) take only that line, but in the case of lists
      of values (as in label \= value1 value2 ... valueN) these may
      become too long so lines are split using the backslash character.

      When such event occurs, lines are read consecutively until no
      backslash is found. The set of lines read is then merged
      into a single one removing the backlashes, as if it would
      have been written as a single line in the original input
      file.
    */

    bool     read_data_line              (ifstream&            file,
                                          string&              line,
                                          int&                 last_line_read,
                                          int&                 line_data);

    /// \brief Removes leading and trailing whitespace from a string. Moreover,
    /// internal whitespace is purged (leaving a single occurence between
    /// non-whitespace characters) and replaced by some string selectable
    /// by the user.
    /**
      \param str The string whose whitespace has to be removed, purged and replaced.
      \param fill The string to use to replace the internal whitespace once it's
             been purged.
      \param whitespace Set of characters to be considered as whitespace.

      Examples: assuming that str is set to "   abc d   e  f   ", calling
      this method like this:

      reduce(str, " ", " \t")

      would yield this result: str = "abc d e f". This is the "normal" whay
      to use this method.

      However, calling the method like this:

      reduce(str, "***", " \t")

      would make str look like this: "abc***d***e***f"
    */

    string   reduce                      (const string&        str,
                                          const string&        fill = " ",
                                          const string&        whitespace = " \t");

    /// \brief Splits a string at the points specified by some configurable character.
    /**
      \param txt The string to be split.
      \param strs A vector of strings, containing the tokens resulting
             from the splitting process.
      \param ch The character used as delimiter to find the places where
             the input string has to be split.
      \return The number of elements in output vector strs (that is, the
             number of tokens resulting from the splitting process). May
             be zero.
    */

    int      split                       (const string&        txt,
                                          vector<std::string>& strs,
                                          char                 ch = ' ');

    /// \brief Remove leading and trailing whitespace from a string. It is
    /// possible to define what is "whitespace".
    /**
      \param str The string whose leading and trailing whitespace has to be removed.
      \param whitespace Set of characters to be considered as whitespace.
      \return A string equivalent to the input one, with leading and trailing
             whitespace removed.
    */
    string   trim                        (const string&        str,
                                          const string&        whitespace = " \t");

  protected:

    /// \brief The map holding the label / value pairs loaded from the input file.

    map<string, string> options_;
};

#endif // _SIMPLE_OPTIONS_FILE_PARSER_
