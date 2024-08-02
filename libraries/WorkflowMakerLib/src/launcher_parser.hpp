/** \file launcher_parser.hpp
\brief Parse launcher XML files.
*/

#ifndef LAUNCHER_PARSER_HPP
#define LAUNCHER_PARSER_HPP

#include "rapidxml.hpp"

#include <QString>

#include <algorithm>
#include <vector>
#include <set>
#include <sstream>
#include <fstream>

#include "launcher_structures.hpp"

using namespace std;
using namespace rapidxml;

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
      \param[out] lch The launcher, once parsed and verified, is loaded
                  into this structure.
      \return     An error code:
                  - 0: successful completion.
     */

    bool            parse           (QString& filename, WLLauncher& lch);

    /// \brief Constructor.

                    launcher_parser (void);

  protected:

    /// \brief The list of detected errors.

    vector<string> error_list_;
};

#endif // LAUNCHER_PARSER_HPP
