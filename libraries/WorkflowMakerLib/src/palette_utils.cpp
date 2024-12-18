/** \file palette_utils.cpp
\brief Implementation file for palette_utils.hpp.
*/

#include "palette_utils.hpp"
#include <QApplication>

bool
palette_utils::
is_dark_palette
(void)
{
  {
    QPalette currentPalette = QApplication::palette();
    QColor windowColor      = currentPalette.color(QPalette::Window);
    return windowColor.lightness() < 128;  // Dark mode check: If the window color is dark
  }
}

void
palette_utils::
text_and_bg_colors
(int     index,
 QColor& txt_color,
 QColor& bg_color)
{
  {
    bool is_dark_mode = is_dark_palette();

    if (index % 2 == 0)
    {
      // Even index.

      if (is_dark_mode)
      {
        bg_color  = QColor(Qt::black); // Black background for even rows in dark mode.
      }
      else
      {
        bg_color  = QColor(Qt::white); // White background for even rows in light mode
      }
    }
    else
    {
      // Odd index

      if (is_dark_mode)
      {
        bg_color = QColor("#383838");     // Dark gray background for odd rows in dark mode
      }
      else
      {
        bg_color = QColor(Qt::lightGray); // Light gray background for odd rows in light mode
      }
    }

    if (is_dark_mode)
    {
      txt_color = QColor(Qt::white);  // White text for dark background
    }
    else
    {
      txt_color = QColor(Qt::black);  // Black text for light background
    }

  }
}

