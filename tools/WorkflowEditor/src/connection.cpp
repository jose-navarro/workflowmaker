/** \file connection.cpp
\brief Implementation file for connection.hpp.
*/

#include "connection.hpp"

//
// The next file is included here instead of in our .hpp file
// to avoid circular inclusion (diagramitem.hpp includes
// connection.hpp).
//

#include "diagramitem.hpp"

void
connection::
compute_endpoints
(QPointF& start, QPointF& end)
{
  {
    QPointF start_point;
    QPointF end_point;

    // Get item coordinates for both endpoints.

    start_item_->subnode_coordinates(start_node_, start_point);
    end_item_->subnode_coordinates(end_node_, end_point);

    // Transform item to scene coordinates.

    start = start_item_->mapToScene(start_point);
    end   = end_item_->mapToScene(end_point);
  }
}

connection::
connection
(subnode_data&  startNode,
 subnode_data&  endNode,
 QGraphicsItem* parent)
  : QGraphicsLineItem(parent), end_node_(endNode), start_node_(startNode)
{
  {
    start_item_ = start_node_.item;
    end_item_   = end_node_.item;

    line_color_ = Qt::black;

    start_item_->update_output_connection(start_node_, +1);
    end_item_->update_input_connection(end_node_, Connected);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(line_color_, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  }
}

DiagramItem*
connection::
endItem
(void)
{
  {
    return end_item_;
  }
}

subnode_data
connection::
endNode
(void)
{
  {
    return end_node_;
  }
}

void
connection::
paint
(      QPainter*                 painter,
 const QStyleOptionGraphicsItem*        ,
       QWidget*)
{
  {
    QPointF start, end;

    //if (start_item_->collidesWithItem(end_item_))
    //  return;

    // Get the two endpoints in scene coordinates.

    compute_endpoints(start, end);

    // Set the line, using the coordinates above.

    setLine(QLineF(start, end));

    QPen myPen = pen();
    myPen.setColor(line_color_);
    painter->setPen(myPen);
    painter->setBrush(line_color_);

    painter->drawLine(line());

    if (isSelected())
    {
      painter->setPen(QPen(line_color_, 1, Qt::DashLine));
      QLineF myLine = line();
      myLine.translate(0, 4.0);
      painter->drawLine(myLine);
      myLine.translate(0,-8.0);
      painter->drawLine(myLine);
    }
  }
}

void
connection::
setColor
(const QColor &color)
{
  {
    line_color_ = color;
  }
}

DiagramItem*
connection::
startItem
(void)
{
  {
    return start_item_;
  }
}

subnode_data
connection::
startNode
(void)
{
  {
    return start_node_;
  }
}

int
connection::
type
(void)
const
{
  {
    return Type;
  }
}

void
connection::
updatePosition
(void)
{
  {
    QPointF start, end;

    compute_endpoints(start, end);
    QLineF line(start, end);
    setLine(line);
  }
}
