/** \file launcher_parser_message_handler.hpp
\brief Error handler for XML validation against the launcher schema.
*/

#ifndef LAUNCHER_PARSER_MESSAGE_HANDLER_HPP
#define LAUNCHER_PARSER_MESSAGE_HANDLER_HPP

#include <QXmlStreamReader>
#include <QAbstractMessageHandler>
#include <QString>

#include <vector>
#include <string>

using namespace std;

/// \brief Handle schema validation errors for launcher XML files.

class launcher_parser_message_handler : public QAbstractMessageHandler
{
  public:

    /**
      \brief Return the text column number where an error was detected for
             error number "at".
      \param[in] at The number of the error sought.
      \return The text column number where and error was detected for error
             number "at".
     */

    int    error_column                    (size_t at) const;

    /**
      \brief Return the text line number where an error was detected for
             error number "at".
      \param[in] at The number of the error sought.
      \return The text line number where and error was detected for error
             number "at".
     */

    int    error_line                      (size_t at) const;

    /**
      \brief Return the textual description of error number "at".
      \param[in] at The number of the error sought.
      \return The textual description of error number "at".
     */

    string error_message                   (size_t at) const;

    /**
      \brief Total number of errors detected.
      \return The total number of errors detected.
     */

    size_t error_total                     (void)      const;

    /**
      \brief Constructor.
      */

           launcher_parser_message_handler (void);

  protected:

    /**
      \brief Re-implementation of parent class method. Takes care of receiving
             and handling the incoming error messages.
      \param[in] type Kind of error.
      \param[in] description Textual description of the error.
      \param[in] identifier Identifier.
      \param[in] sourceLocation Location (such as line and column in the parsed
                 text file) of the error.
     */

    virtual
    void    handleMessage                  (      QtMsgType        type,
                                            const QString&         description,
                                            const QUrl&            identifier,
                                            const QSourceLocation& sourceLocation);


private:

    /// \brief Array storing the error column numbers.

    vector<int>    error_column_;

    /// \brief Array storing the textual description of the errors.

    vector<string> error_description_;

    /// \brief Array storing the error line numbers.

    vector<int>    error_line_;
};

#endif // LAUNCHER_PARSER_MESSAGE_HANDLER_HPP
