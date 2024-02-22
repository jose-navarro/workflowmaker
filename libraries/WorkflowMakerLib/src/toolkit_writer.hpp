/** \file toolkit_writer.hpp
\brief Write toolkit XML files.
*/


#ifndef TOOLKIT_WRITER_HPP
#define TOOLKIT_WRITER_HPP

#include <QString>

#include <fstream>
#include <string>
#include <vector>

#include "toolkit_structures.hpp"

using namespace std;

/// \brief Write toolkit XML files

class toolkit_writer
{
  public:

    /// \brief Return the list of detected errors.
    /**
      \return The list of detected errors. Note that the list
              will be empty if write() returned true.
     */

    vector<string> error_list     (void);

    /// \brief Constructor.

                   toolkit_writer (void);

    /// \brief Write a toolkit.
    /**
      \param[in] tk The toolkit to write.
      \param[in] filename The name of the file where the toolkit
                 will be saved.
      \return True if the toolkit is saved correctly, false otherwise.

      Call error_list() to retrieve the list of errors detected by this
      method when false is returned.
     */

    bool           write          (toolkit& tk, QString& filename);

  protected:

    /// \brief Change case to uppercase.
    /**
      \param lower The string whose case is about to be changed.
      \return The uppercase version of the input string.
     */

    string         to_upper       (string& lower);

  protected:

    /// \brief The list of detected errors.

    vector<string> error_list_;
};

#endif // TOOLKIT_WRITER_HPP
