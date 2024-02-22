/** \file workflow_writer.hpp
\brief Write workflow XML files.
*/


#ifndef WORKFLOW_WRITER_HPP
#define WORKFLOW_WRITER_HPP

#include <QString>

#include <fstream>
#include <string>
#include <vector>

#include "workflow_structures.hpp"

using namespace std;

/// \brief Write workflow XML files

class workflow_writer
{
  public:

    /// \brief Return the list of detected errors.
    /**
      \return The list of detected errors. Note that the list
              will be empty if write() returned true.
     */

    vector<string> error_list      (void);

    /// \brief Constructor.

                   workflow_writer (void);

    /// \brief Write a workflow.
    /**
      \param[in] wf The workflow to write.
      \param[in] filename The name of the file where the workflow
                 will be saved.
      \return True if the workflow is saved correctly, false otherwise.

      Call error_list() to retrieve the list of errors detected by this
      method when false is returned.
     */

    bool           write           (WFWorkflow& wf, QString& filename);

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

#endif // WORKFLOW_WRITER_HPP
