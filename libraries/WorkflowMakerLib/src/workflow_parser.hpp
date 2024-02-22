/** \file workflow_parser.hpp
\brief Parse workflow XML files.
*/

#ifndef WORKFLOW_PARSER_HPP
#define WORKFLOW_PARSER_HPP

#include <QDomDocument>
#include <QFile>
#include <QtXmlPatterns>
#include <QString>

#include <vector>
#include <set>
#include <sstream>

#include "workflow_structures.hpp"
#include "workflow_parser_message_handler.hpp"

using namespace std;

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


    /// \brief Set the path to the validating schema.
    /**
      \param[in] path_to_schema
      \return True if the schema is set, false otherwise.

      Call this method if a validating schema is available.
      Setting a schema means that the parse() method will be
      safer to use, since not only the syntactic validity
      will be checked; the structure of the xml file to read
      will be compared with the one defined by the schema.
     */

    bool            set_schema      (QString& path_to_schema);

    /// \brief Constructor.

                    workflow_parser (void);

  protected:

    /// \brief The list of detected errors.

    vector<string> error_list_;

    /// \brief Flag stating whether a validating schema is available.

    bool           got_schema_;

    /// \brief The validating schema.

    QXmlSchema     schema_;
};

#endif // WORKFLOW_PARSER_HPP
