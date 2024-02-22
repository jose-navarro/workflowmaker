/** \file simple_options_file_parser.cpp
\brief Implementation file for simple_options_file_parser.hpp
*/

#include "simple_options_file_parser.hpp"

int
simple_options_file_parser::
get_option_double
(const string& option_label,
 double&       option_value)
{
  {
    map<string, string>::iterator finder;
    double                        result;
    string                        svalue;

    finder = options_.find(option_label);
    if (finder == options_.end()) return 1; // Label not found.

    svalue = finder->second;

    stringstream convert (svalue);
    if (!(convert >> result)) return 2;

    option_value = result;

    return 0;
  }
}

int
simple_options_file_parser::
get_option_double_array
(const string&   option_label,
 vector<double>& option_value)
{
  {
    double                        dvalue;
    size_t                        found;
    map<string, string>::iterator finder;
    int                           i;
    vector<string>                items;
    int                           size;
    string                        svalue;
    string                        trimmed_value;
    string                        value;

    finder = options_.find (option_label);
    if (finder == options_.end ()) return 1; // Label not found.

    // Get the value.

    svalue = finder->second;

    //
    // Purge whitespace (no leading nor trailing whitespace, only
    // one, single blank character between words).
    //

    trimmed_value = reduce(svalue);

    // Check if we have a single word only.

    found = trimmed_value.find (" ");
    if (found == string::npos)
    {
      size = 1;
      items.clear ();
      items.push_back (trimmed_value);
    }
    else
    {
      size = split (trimmed_value, items);
    }


    option_value.clear();

    for (i = 0; i < size; i++)
    {
      value = items[i];

      stringstream convert (value);
      if (!(convert >> dvalue)) return 2;

      option_value.push_back(dvalue);
    }

    return 0;
  }
}

int
simple_options_file_parser::
get_option_int
(const string& option_label,
 int&          option_value)
{
  {
    map<string, string>::iterator finder;
    int                   result;
    string                svalue;

    finder = options_.find (option_label);
    if (finder == options_.end ()) return 1; // Label not found.

    svalue = finder->second;

    stringstream convert (svalue);
    if (!(convert >> result)) return 2;

    option_value = result;

    return 0;

  }
}

int
simple_options_file_parser::
get_option_int_array
(const string& option_label,
 vector<int>&  option_value)
{
  {
    map<string, string>::iterator finder;
    size_t                        found;
    int                           i;
    vector<string>                items;
    int                           ivalue;
    int                           size;
    string                        svalue;
    string                        trimmed_value;
    string                        value;

    finder = options_.find (option_label);
    if (finder == options_.end ()) return 1; // Label not found.

    // Get the value.

    svalue = finder->second;

    //
    // Purge whitespace (no leading nor trailing whitespace, only
    // one, single blank character between words).
    //

    trimmed_value = reduce (svalue);

    // Check if we have a single word only.

    found = trimmed_value.find (" ");
    if (found == string::npos)
    {
      size = 1;
      items.clear();
      items.push_back(trimmed_value);
    }
    else
    {
      size = split (trimmed_value, items);
    }

    option_value.clear();

    for (i = 0; i < size; i++)
    {
      value = items[i];

      stringstream convert (value);
      if (!(convert >> ivalue)) return 2;

      option_value.push_back (ivalue);
    }

    return 0;
  }
}

int
simple_options_file_parser::
get_option_string
(const string& option_label,
 string&       option_value)
{
  {
    map<string, string>::iterator finder;

    finder = options_.find (option_label);
    if (finder == options_.end ()) return 1; // Label not found.

    option_value = finder->second;

    return 0;

  }
}

int
simple_options_file_parser::
get_option_string_array
(const string&   option_label,
 vector<string>& option_value)
{
  {
    map<string, string>::iterator finder;
    size_t                        found;
    int                           size;
    string                        svalue;
    string                        trimmed_value;

    finder = options_.find (option_label);
    if (finder == options_.end ()) return 1; // Label not found.

    // Get the value.

    svalue = finder->second;

    //
    // Purge whitespace (no leading nor trailing whitespace, only
    // one, single blank character between words).
    //

    trimmed_value = reduce (svalue);

    option_value.clear();

    // Check if we have a single word only.

    found = trimmed_value.find (" ");
    if (found == string::npos)
    {
      size = 1;
      option_value.clear ();
      option_value.push_back (trimmed_value);
    }
    else
    {
      size = split (trimmed_value, option_value);
    }

    return 0;
  }
}

int
simple_options_file_parser::
parse
(const string& options_file,
 int&          error_line)
{
  {
    ifstream cfgfile;
    size_t   equal_pos;
    string   id;
    string   line;
    int      line_read;
    string   left;
    string   right;
    string   value;

    line_read = 0;
    options_.clear ();

    if (options_file.empty ()) return 1; // Empty name of options file.

    cfgfile.open (options_file.c_str ());
    if (!cfgfile.is_open ()) return 2;  // Unable to open options file.

    while (read_data_line (cfgfile, line, line_read, error_line))
    {
      // Look for the equal sign.

      equal_pos = (int)line.find_first_of ("=");
      if (equal_pos == string::npos)
      {
        cfgfile.close ();
        return 3; // Error parsing. No equal sign.
      }

      //
      // Get the raw left and right sides using the equal sign
      // position as the center.
      //

      left = line.substr (0, equal_pos - 1);
      right = line.substr (equal_pos + 1);

      //
      // Remove all leading, trailing whitespace from left and right.
      // Moreover, purge the whitespace inside the identifier, leaving
      // just one blank between non-blank characters.
      //

      id    = reduce (left);
      value = trim   (right);

      // Check that both the identifier and the value are not empty.

      if (id.empty () || value.empty ())
      {
        cfgfile.close ();
        return 4; // Error parsing. Empty keyword or value.
      }

      // Add our new pair to our map.

      options_[id] = value;
    }

    // Housekeeping.

    cfgfile.close ();
    error_line = 0;

    // We're over. No errors!

    return 0;
  }
}

bool
simple_options_file_parser::
read_data_line
(ifstream& file, string& line, int& last_line_read, int& line_data)
{
  {
    char   c;
    bool   data_found;
    size_t len;
    string line1, line2;
    int    line_read;
    size_t pos_nonempty;
    size_t pos_slash;
    string trimmed_value;

    data_found =false;
    line_read  = last_line_read;
    line2      = "";

    while (getline (file, line1))
    {
      line_read++;

      trimmed_value = reduce(line1);

      // Deal with different line endings (Linux vs. Windows).

      len = trimmed_value.length();
      if (len > 0)
      {
        c = trimmed_value[len-1];
        if (c == '\r') trimmed_value = trimmed_value.substr(0, len-1);
      }

      // We must skip empty lines.

      pos_nonempty= trimmed_value.find_first_not_of (" \t");

      if (pos_nonempty != string::npos)
      {
        //
        // Non-empty line. Look now for a comment line. We must skip these
        // only when they precede the data line we're looking for.
        //

        if (trimmed_value.at (pos_nonempty) == '#')
        {
          //
          // We have a comment line. If we had already detected data before,
          // it would mean that we've gone past the data line and found the NEXT
          // comment line. We must therefore break at this very point.
          //

          if (data_found) break;
        }
        else
        {
          // This is a data line.

          if (!data_found) line_data = line_read;

          data_found = true;

          //
          // Look for the ending backlash - which would mean that we'll have
          // a multi-line line. But only when it is the very last character
          // in our input line! Otherwise, we might misinterpret windows-like
          // paths, since these use backslashes to separate subdirectories...
          //

          len = trimmed_value.length();
          pos_slash = trimmed_value.find_last_of ("\\");


          if ((pos_slash != string::npos) && (pos_slash == (len-1)))
          {
            //
            // We found it. Concatinate the text just read (but the ending backslash)
            // and go for more.
            //

            line2 = line2 + " " + trimmed_value.substr (0, pos_slash);
          }
          else
          {
            //
            // No backslash. Append the line just read and break, since no more
            // data lines must be sought for this entry.
            //

            line2 = line2 + " " + trimmed_value;
            break;
          }
        }
      }
    }

    // Update the line numbers.

    last_line_read = line_read;

    // If line2 is empty, it means that we've reached the end of the file.

    if (line2.empty ()) return false;

    // Copy our data to the output parameter and return.

    line = line2;
    return true;
  }
}

string
simple_options_file_parser::
reduce
(const string& str,
 const string& fill,
 const string& whitespace)
{
  string result;
  {
    size_t begin_space;
    size_t end_space;
    size_t new_start;
    size_t range;

    // Trim first

    result = trim (str, whitespace);

    // Replace sub ranges

    begin_space = result.find_first_of (whitespace);

    while (begin_space != string::npos)
    {
      end_space = result.find_first_not_of (whitespace, begin_space);
      range = end_space - begin_space;

      result.replace (begin_space, range, fill);

      new_start = begin_space + fill.length ();
      begin_space = result.find_first_of (whitespace, new_start);
    }
  }
  return result;
}

simple_options_file_parser::
simple_options_file_parser
(void)
{
  {
    options_.clear();
  }
}


simple_options_file_parser::
~simple_options_file_parser
(void)
{
  {
    options_.clear();
  }
}

int
simple_options_file_parser::
split
(const string&        txt,
 vector<std::string>& strs,
 char                 ch)
{
  {
    size_t initial_pos;
    size_t last;
    size_t pos;
    size_t size;

    size = txt.size();
    pos  = txt.find (ch);
    initial_pos = 0;
    strs.clear ();

    // Decompose statement

    while (pos != string::npos)
    {
      strs.push_back (txt.substr (initial_pos, pos - initial_pos + 1));
      initial_pos = pos + 1;

      pos = txt.find (ch, initial_pos);
    }

    // Add the last one

    last = pos;
    if (size < last) last = size;

    strs.push_back (txt.substr (initial_pos, last - initial_pos + 1));

    return (int)strs.size ();
  }
}

string
simple_options_file_parser::
trim
(const string& str,
 const string& whitespace)
{
  {
    size_t str_begin;
    size_t str_end;
    size_t str_range;

    str_begin = str.find_first_not_of (whitespace);

    if (str_begin == string::npos) return ""; // no content

    str_end   = str.find_last_not_of (whitespace);
    str_range = str_end - str_begin + 1;

    return str.substr (str_begin, str_range);
  }
}
