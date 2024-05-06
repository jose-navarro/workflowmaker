/** \file single_repository_widget.hpp
\brief Dialog to show / collect the data related to a single repository.
*/

#ifndef SINGLE_REPOSITORY_WIDGET_HPP
#define SINGLE_REPOSITORY_WIDGET_HPP

#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPalette>
#include <QPushButton>
#include <QString>
#include <QWidget>

#include "launcher_structures.hpp"

using namespace std;

/// \brief Dialog to show / collect the data related to a single repository.

class single_repository_widget : public QWidget
{
    Q_OBJECT

  public:

    /// \brief Check that all the required data have
    /// been input in the widget's interface.
    /**
      \return True if all data have been set, false
              otherwise.
     */

    bool    check_completeness       (void);

    /// \brief Retrieve the identifier for the repository.
    /**
      \return The repository's identifier.
     */

    QString get_id                   (void);

    /// \brief Retrieve the value for the repository.
    /**
      \return The path assigned to the repository.
     */

    QString get_value                (void);

    /// \brief Constructor.
    /**
      \param[in] bg_color Color to use for the background of the widget.
      \param[in] value The actual value of the repository to handle.
      \param[in] parent This widget's parent.
     */

            single_repository_widget (QColor&       bg_color,
                                      WLRepository& value,
                                      QWidget*      parent = nullptr);

  protected slots:

    /// \brief Browse to identify the path to assign to the repository.

    void    on_browse_path           (void);

  protected:

    /// \brief Line edit:  Repository identifier.

    QLineEdit*   repo_id_text_;

    /// \brief Line edit: Repository path.

    QLineEdit*   repo_path_text_;
};

#endif // SINGLE_REPOSITORY_WIDGET_HPP
