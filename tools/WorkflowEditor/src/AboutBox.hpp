/** \file AboutBox.hpp
\brief About box displaying information about the application.
*/

#ifndef ABOUTBOX_HPP
#define ABOUTBOX_HPP

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QBitmap>
#include <QFont>
#include <QPushButton>

#include <QDebug>

/// \brief About box displaying information about the application.
/**
  This class displays an "about box" describing the name, version, company for
  the application, as well as showing its icon. A link to the web of the
  developer is provided too.
 */

class AboutBox : public QDialog
{
    Q_OBJECT

  public:

    /// \brief Constructor.
    /**
      \param version The application's version.
      \param parent The parent widget.
     */
    explicit AboutBox    (QString& version, QWidget *parent = nullptr);
    
};

#endif // ABOUTBOX_HPP
