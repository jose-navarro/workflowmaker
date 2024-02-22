/** \file toolkit_parser.hpp
\brief Parse toolkit XML files.
*/

#ifndef TOOLKIT_PARSER_HPP
#define TOOLKIT_PARSER_HPP

#include <QDomDocument>
#include <QFile>
#include <QtXmlPatterns>
#include <QString>

#include <vector>
#include <set>
#include <sstream>

#include "toolkit_structures.hpp"
#include "toolkit_parser_message_handler.hpp"

#include <QDebug>

using namespace std;

/// \brief Parse toolkit XML files.

class toolkit_parser
{
  public:

    /// \brief Return the list of detected errors.
    /**
      \return The list of detected errors. Note that the list
              will be empty if parse() returned true.
     */

    vector<string> error_list      (void);

    /// \brief Parse a toolkit, loading its contents in a
    ///        toolkit structure.
    /**
      \param[in]  filename The name of the XML file with the definition
                  of the toolkit.
      \param[out] tk The toolkit, once parsed and verified, is loaded
                  into this structure.
      \return     An error code:
                  - 0: successful completion.
     */

    bool            parse          (QString& filename, toolkit& tk);


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

    bool            set_schema     (QString& path_to_schema);

    /// \brief Constructor.

                    toolkit_parser (void);

  protected:

    /// \brief Validate a toolkit stored in a toolkit structure.
    /**
      \param[in] tk The toolkit to validate.
      \return    True if the tookit is valid, false otherwise.

      When errors are detected, these are written to the
      protected member error_list_;
     */

    bool validate_toolkit (toolkit& tk);

  protected:

    /// \brief The list of detected errors.

    vector<string> error_list_;

    /// \brief Flag stating whether a validating schema is available.

    bool           got_schema_;

    /// \brief The validating schema.

    QXmlSchema     schema_;
};

#endif // TOOLKIT_PARSER_HPP
