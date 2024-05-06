/** \file connection.hpp
\brief Class modeling a connection between two items.
*/


#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <QGraphicsLineItem>
#include <QPainter>
#include <QPen>
#include <QtMath>
#include "WFMEditor_structures.hpp"

class DiagramItem;

/// \brief Class modeling a connection between two items.

class connection : public QGraphicsLineItem
{
  public:

    enum { Type = UserType + 4 };

    /// \brief Constructor.
    /**
      \param[in] startNode The information describing the starting endpoint of the
                 connection.
      \param[in] endNode The information describing the final endpoint of the
                 connection.
      \param[in] parent The parent of this item.

      Note that both parameters, startNode and endNode contain information about:
        - The DiagramItem items to which these endpoints correspond.
        - The type of connector (input / output).
        - The slot number (for instance, the first, third, ..., slot).
     */
                 connection        (subnode_data&  startNode, subnode_data& endNode,
                                    QGraphicsItem* parent = nullptr);

    /// \brief Retrieve the DiagramItem related to the final endpoint of the
    ///        connection.
    /**
      \return A pointer to the DiagramItem related to the final endpoint of
              the connection.
     */

    DiagramItem* endItem           (void);

    /// \brief Retrieve the subnode data related to the final endpoint of the
    ///        connection.
    /**
      \return A copy of the subnode data related to the final endpoint of
              the connection.

      The subnode data contains information about the actual DiagramItem
      involved in the connection, whether this is an input or output one,
      and the specific slot of the task / repository where the connection
      starts or ends.
     */

    subnode_data endNode           (void);

    /// \brief Set the color used to draw the connection.
    /**
      \param[in] color The color to use.
     */

    void         setColor          (const QColor &color);

    /// \brief Retrieve the DiagramItem related to the initial endpoint of the
    ///        connection.
    /**
      \return A pointer to the DiagramItem related to the initial endpoint of
              the connection.
     */

    DiagramItem* startItem         (void);

    /// \brief Retrieve the subnode data related to the initial endpoint of the
    ///        connection.
    /**
      \return A copy of the subnode data related to the initial endpoint of
              the connection.

      The subnode data contains information about the actual DiagramItem
      involved in the connection, whether this is an input or output one,
      and the specific slot of the task / repository where the connection
      starts or ends.
     */

    subnode_data startNode         (void);

    /// \brief Returns the type of the item as an int.
    /**
      \return The type of the item.

      This method overrides the one from the parent of this class.
     */

    int          type              (void) const override;

    /// \brief Draws the line representing the connection taking into account
    ///        the positions of its two endpoints.

    void         updatePosition    (void);

  protected:

    /// \brief Compute the scene coordinates of both the initial and final
    ///        enpoints, that is, the two DiagramItems that the used decided
    ///        to connect drawing a line.
    /**
      \param[out] start Scene coordinates of the initial endpoints.
      \param[out] end Scene coordinates of the final endpoint.

      Note: these coordinates belong to the center of the specific slots
      of the task / repository where the connection starts and ends.
     */

    void         compute_endpoints (QPointF& start, QPointF& end);

    /// \brief Paint the contents of the item in local (item's) coordinates.
    /**
      \param[in] painter The painter that will take care of painting the item.
      \param[in] option  Style options for the item, such as its state, exposed area
                         and its level-of-detail hints.
      \param[in] widget  Optional. If provided, it points to the widget that is being
                         painted on; otherwise, it is 0. For cached painting, widget
                         is always 0.
     */

    void         paint             (QPainter* painter, const QStyleOptionGraphicsItem* option,
                                    QWidget*  widget = nullptr) override;

  private:

    /// \brief Pointer to the current connection's final endpoint DiagramItem.

    DiagramItem* end_item_;

    /// \brief Detailed information (DiagramItem, type of connection, slot) about
    ///        the current connection's initial endpoint.

    subnode_data end_node_;

    /// \brief Color to use when drawing the lines representing the connections.

    QColor       line_color_;

    /// \brief Pointer to the current connection's initial endpoint DiagramItem.

    DiagramItem* start_item_;

    /// \brief Detailed information (DiagramItem, type of connection, slot) about
    ///        the current connection's final endpoint.

    subnode_data start_node_;
};

#endif // CONNECTION_HPP
