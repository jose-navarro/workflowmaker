/** \file toolkit_parser.hpp
\brief Parse toolkit XML files.
*/

#ifndef TOOLKIT_PARSER_HPP
#define TOOLKIT_PARSER_HPP

#include "rapidxml.hpp"

#include <QString>

#include <algorithm>
#include <vector>
#include <set>
#include <sstream>
#include <fstream>

#include "toolkit_structures.hpp"

using namespace std;
using namespace rapidxml;

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
};

#endif // TOOLKIT_PARSER_HPP
