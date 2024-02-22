/** \file single_parameter_widget.hpp
\brief Dialog to show / collect the data related to a single parameter.
*/

#ifndef SINGLE_PARAMETER_WIDGET_HPP
#define SINGLE_PARAMETER_WIDGET_HPP

#include <QComboBox>
#include <QDoubleValidator>
#include <QGridLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPalette>
#include <QString>
#include <QWidget>

#include <QDebug>

#include "launcher_structures.hpp"

using namespace std;

/// \brief Dialog to show / collect the data related to a single parameter.

class single_parameter_widget : public QWidget
{
    Q_OBJECT

  public:

    /// \brief Check that all the required parameters have
    /// been input in the widget's interface.
    /**
      \return True if all parameters have been set, false
              otherwise.
     */

    bool    check_completeness      (void);


    /// \brief Retrieve the identifier of the parameter.
    /**
      \return The parameter's identifier.
     */

    QString get_id                  (void);

    /// \brief Retrieve the value for the parameter.
    /**
      \return The parameter's value.
     */

    QString get_value               (void);

    /// \brief Constructor.
    /**
      \param[in] bg_color Color to use for the background of the widget.
      \param[in] value The actual value of the parameter to handle.
      \param[in] parent This widget's parent.
     */

            single_parameter_widget (QColor&      bg_color,
                                     WLParameter& value,
                                     QWidget*     parent = nullptr);
  protected:

    /// \brief Line edit:  Parameter description.

    QLineEdit*   param_description_text_;

    /// \brief Line edit: Parameter label.

    QLineEdit*   param_label_text_;

    /// \brief Line edit: Identifier of the task to which the parameter belongs.

    QLineEdit*   param_task_id_text_;

    /// \brief Line edit: Parameter type.

    QLineEdit*   param_type_text_;

    /// \brief Line edit: Parameter value (text version).

    QLineEdit*   param_value_text_;

    /// \brief Combo box: Parameter value (combo version, for booleans).

    QComboBox*   param_value_cb_;
};

#endif // SINGLE_PARAMETER_WIDGET_HPP
