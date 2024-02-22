/** \file launcher_writer.hpp
\brief Write launcher XML files.
*/


#ifndef LAUNCHER_WRITER_HPP
#define LAUNCHER_WRITER_HPP

#include <QString>

#include <fstream>
#include <string>
#include <vector>

#include "launcher_structures.hpp"

using namespace std;

/// \brief Write launcher XML files.

class launcher_writer
{
  public:

    /// \brief Return the list of detected errors.
    /**
      \return The list of detected errors. Note that the list
              will be empty if write() returned true.
     */

    vector<string> error_list      (void);

    /// \brief Constructor.

                   launcher_writer (void);

    /// \brief Write a launcher.
    /**
      \param[in] launcher The launcher to write.
      \param[in] filename The name of the file where the launcher
                 will be saved.
      \return True if the workflow is saved correctly, false otherwise.

      Call error_list() to retrieve the list of errors detected by this
      method when false is returned.
     */

    bool           write           (WLLauncher& launcher, QString& filename);

  protected:

    /// \brief Change case to uppercase.
    /**
      \param lower The string whose case is about to be changed.
      \return The uppercase version of the input string.
     */

    string         to_upper        (string& lower);

  protected:

    /// \brief The list of detected errors.

    vector<string> error_list_;
};

#endif // LAUNCHER_WRITER_HPP
