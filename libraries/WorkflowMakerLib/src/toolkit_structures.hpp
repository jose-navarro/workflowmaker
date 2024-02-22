/** \file toolkit_structures.hpp
\brief Definition of all the data structures making a toolkit.
*/


#ifndef TOOLKIT_STRUCTURES_HPP
#define TOOLKIT_STRUCTURES_HPP

#include <string>
#include <vector>

using namespace std;

/// \brief Describes a single parameter.

struct toolkit_parameter
{
  string id;                              /**< Unique identifier used to tell
                                               apart several parameters.
                                               It should be descriptive enough to
                                               let users ascertain its purpose. */

  string description;                     /**< Describes in detail the meaning of
                                               the parameter. */

  string type;                            /**< Kind (integer, real, boolean,
                                               string) of data that the parameter
                                               will hold. */
};

/// \brief Describes a file (either for input or output purposes).

struct toolkit_file
{
  string file_type_id;                    /**< Kind of file. Use to tell apart,
                                               for instance, text, image or audio
                                               files. See toolkit_file_type for
                                               details. */

  string description;                     /**< Description of the contents /
                                               purpose of the file. */

  string options_file_label;              /**< Label that a task will look for
                                               in an options file to read the name
                                               of the file itself. */
};

/// \brief Define file types.
/**
  Files may store very different kinds of information, such as images,
  sounds, plain text, etc.

  With this struct it is possible to define as many kinds of files
  as the toolkit require to tell these apart.
 */

struct toolkit_file_type
{
  string id;                              /**< Unique identifier to tell apart
                                               different file types.
                                               It should be meaningful enough as
                                               to serve as a mnemotecnic of the purpose
                                               of the file type. */

  string description;                     /**< Detailed description for this
                                               kind of file. */

  string extension;                       /**< The usual extension for this kind
                                               of files. */

};

/// \brief Software component (executable) able to perform some kind of
///        task.

struct toolkit_task
{
  string id;                              /**< Unique identifier for the task. */

  string description;                     /**< Detailed description, stating what
                                               is the purpose of the task. */

  string name_of_executable;              /**< Name of the executable file implementing
                                               the task. Add no ".exe" extension to
                                               this name. */

  vector<string> parameters;              /**< List of parameter identifiers .*/

  vector<toolkit_file> input_files;       /**< List of input files (list of
                                               toolkit_file structures). */

  vector<toolkit_file> output_files;      /**< List of output files (list of
                                               toolkit_file structures). */

  bool operator <(const toolkit_task& other) const
  {
    return (this->id < other.id);
  }
};

/// \brief Definition of a toolkit, including the available parameters,
///        file types and tasks.

struct toolkit
{
    string id;                            /**< Toolkit identifier. */

    string description;                   /**< Description of toolkit's purpose. */

    vector<toolkit_parameter> parameters; /**< List of parameters in the toolkit. */

    vector<toolkit_file_type> file_types; /**< List of file types in the toolkit. */

    vector<toolkit_task> tasks;           /**< List of tasks in the toolkit. */
};

#endif // TOOLKIT_STRUCTURES_HPP
