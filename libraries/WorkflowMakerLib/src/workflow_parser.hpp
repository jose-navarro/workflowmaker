/** \file workflow_parser.hpp
\brief Parse workflow XML files.
*/

#ifndef WORKFLOW_PARSER_HPP
#define WORKFLOW_PARSER_HPP

#include "rapidxml.hpp"

#include <QString>

#include <algorithm>
#include <vector>
#include <set>
#include <sstream>
#include <fstream>

#include "workflow_structures.hpp"

using namespace std;
using namespace rapidxml;

/// \brief Parse toolkit XML files.

class workflow_parser
{
  public:

    /// \brief Return the list of detected errors.
    /**
      \return The list of detected errors. Note that the list
              will be empty if parse() returned true.
     */

    vector<string> error_list       (void);

    /// \brief Parse a workflow, loading its contents in a
    ///        workflow structure.
    /**
      \param[in]  filename The name of the XML file with the definition
                  of the workflow.
      \param[out] wf The workflow, once parsed and verified, is loaded
                  into this structure.
      \return     An error code:
                  - 0: successful completion.
     */

    bool            parse           (QString& filename, WFWorkflow& wf);


    /// \brief Constructor.

                    workflow_parser (void);

  protected:

    /// \brief The list of detected errors.

    vector<string> error_list_;
};

#endif // WORKFLOW_PARSER_HPP
