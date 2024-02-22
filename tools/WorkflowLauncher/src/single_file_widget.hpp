/** \file single_file_widget.hpp
\brief Dialog to show / collect the data related to a single file.
*/

#ifndef SINGLE_FILE_WIDGET_HPP
#define SINGLE_FILE_WIDGET_HPP

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPalette>
#include <QString>
#include <QWidget>

#include "launcher_structures.hpp"

using namespace std;

/// \brief Dialog to show / collect the data related to a single file.

class single_file_widget : public QWidget
{
    Q_OBJECT

  public:

    /// \brief Check that all the required data have
    /// been input in the widget's interface.
    /**
      \return True if all data have been set, false
              otherwise.
     */

    bool    check_completeness (void);

    /// \brief Retrieve the value for the file.
    /**
      \return The file name value.
     */

    QString get_value          (void);

    /// \brief Constructor.
    /**
      \param[in] bg_color Color to use for the background of the widget.
      \param[in] value The actual file data to handle.
      \param[in] parent This widget's parent.
     */

            single_file_widget (QColor&  bg_color,
                                WLFile&  value,
                                QWidget* parent = nullptr);
  protected:

    /// \brief Line edit: File connection.

    QLineEdit* file_connection_text_;

    /// \brief Line edit:  File description.

    QLineEdit* file_description_text_;

    /// \brief Line edit: File name.

    QLineEdit* file_name_text_;

    /// \brief Line edit: File type.

    QLineEdit* file_type_text_;

};

#endif // SINGLE_FILE_WIDGET_HPP
