/** \file launcher_parser_message_handler.cpp
\brief Implementation file for launcher_parser_message_handler.hpp.
*/

#include "launcher_parser_message_handler.hpp"

int
launcher_parser_message_handler::
error_column
(size_t at)
const
{
  {
    return error_column_[at];
  }
}

int
launcher_parser_message_handler::
error_line
(size_t at)
const
{
  {
    return error_line_[at];
  }
}

string
launcher_parser_message_handler::
error_message
(size_t at)
const
{
  {
    return error_description_[at];
  }
}

size_t
launcher_parser_message_handler::
error_total
(void)
const
{
  {
    return error_description_.size();
  }
}

void
launcher_parser_message_handler::
handleMessage
(      QtMsgType        type,
 const QString&         description,
 const QUrl&            identifier,
 const QSourceLocation& sourceLocation)
{
  {
    Q_UNUSED(type);
    Q_UNUSED(identifier);

    QXmlStreamReader xml(description);
    QString text;
    while (!xml.atEnd())
      if (xml.readNext() == QXmlStreamReader::Characters)
        text += xml.text();

    // Copy the error data to our internal members.

    error_description_.push_back(text.toStdString());
    error_column_.push_back(sourceLocation.column());
    error_line_.push_back(sourceLocation.line());
  }
}

launcher_parser_message_handler::
launcher_parser_message_handler (void)
    : QAbstractMessageHandler(nullptr)
{
  {
    error_column_.clear();
    error_description_.clear();
    error_line_.clear();
  }
}
