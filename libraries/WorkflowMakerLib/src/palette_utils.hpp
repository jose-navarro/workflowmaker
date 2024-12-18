/** \file palette_utils.hpp
\brief Color palettes-related utilities.
*/

#ifndef PALETTEUTILS_HPP
#define PALETTEUTILS_HPP

#include <QPalette>
#include <QColor>

/// \brief Color palettes-related utilities.

class palette_utils
{
  public:

    /// \brief Check if the current color palette is dark.
    /**
      \return True if the color palette currently in use is dark,
              false otherwhise.

      "Dark" in this context means that lightness of the current window
      is less that 128.
     */

    static bool is_dark_palette    (void);

    /// \brief Retrieve the text & background colors for dark or light
    /// themes. The background color is alternated for even or odd
    /// indices. Useful for lists of records.

    static void text_and_bg_colors (int index, QColor& txt_color, QColor& bg_color);
};

#endif // PALETTEUTILS_HPP

