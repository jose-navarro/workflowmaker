/** \file launcher_parser.hpp
\brief Parse launcher XML files.
*/

#ifndef LAUNCHER_PARSER_HPP
#define LAUNCHER_PARSER_HPP

#include <QDomDocument>
#include <QFile>
#include <QtXmlPatterns>
#include <QString>

#include <vector>
#include <set>
#include <sstream>

#include "launcher_structures.hpp"
#include "launcher_parser_message_handler.hpp"

using namespace std;

/// \brief Parse toolkit XML files.

class launcher_parser
{
  public:

    /// \brief Return the list of detected errors.
    /**
      \return The list of detected errors. Note that the list
              will be empty if parse() returned true.
     */

    vector<string> error_list       (void);

    /// \brief Parse a launcher, loading its contents in a
    ///        launcher structure.
    /**
      \param[in]  filename The name of the XML file with the definition
                  of the launcher.
      \param[out] wf The launcher, once parsed and verified, is loaded
                  into this structure.
      \return     An error code:
                  - 0: successful completion.
     */

    bool            parse           (QString& filename, WLLauncher& lch);


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

                    launcher_parser (void);

  protected:

    /// \brief The list of detected errors.

    vector<string> error_list_;

    /// \brief Flag stating whether a validating schema is available.

    bool           got_schema_;

    /// \brief The validating schema.

    QXmlSchema     schema_;
};

#endif // LAUNCHER_PARSER_HPP
