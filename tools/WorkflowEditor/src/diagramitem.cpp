/** \file diagramitem.cpp
\brief Implementation file for diagramitem.hpp.
*/


#include "diagramitem.hpp"
#include "qgraphicsview.h"

void
DiagramItem::
add_connection
(connection *conn)
{
  {
    connections_.append(conn);
  }
}

void
DiagramItem::
contextMenuEvent
(QGraphicsSceneContextMenuEvent *event)
{
  {
    scene()->clearSelection();
    setSelected(true);
    context_menu_->exec(event->screenPos());
  }
}

DiagramItem::
DiagramItem
(DiagramType          diagramType,
 toolkit_task&        node_data,
 QMenu*               contextMenu,
 QGraphicsItem*       parent)
  : QGraphicsPolygonItem(parent), context_menu_(contextMenu), diagram_type_(diagramType)
{
  {
    num_id_ = -1;

    switch (diagram_type_)
    {
      case Task:

        qreal w1, w2, w3, width, height, half_width, half_height;
        qreal x, y;
        int   t_inputs, t_outputs;

        d_      = 20.0;

        t_inputs  = (int)node_data.input_files.size();
        t_outputs = (int)node_data.output_files.size();

        // Compute the width of the several elements making the step.

        w1 = 2 * t_inputs  * d_;  // The input bullets.
        w2 = 8 * d_;              // The text box will be at least 8*d.
        w3 = 2 * t_outputs * d_;  // The output bullets.

        //
        // The actual width of the step is determined by the width of
        // its widest element.
        //

        width                 = w1;
        if (w2 > width) width = w2;
        if (w3 > width) width = w3;

        // Now, compute the height.

        height = 3 * (2 * d_); // Three strips, equal heigh each.

        // Convenient auxiliary values.

        half_width   = width  / 2.0;
        half_height  = height / 2.0;

        // Define the outer frame.

        polygon_ << QPointF(-half_width, -half_height) << QPointF( half_width, -half_height)
                 << QPointF( half_width,  half_height) << QPointF(-half_width,  half_height)
                 << QPointF(-half_width, -half_height);

        // Fill our node_data_ structure.

        node_data_.type               = diagram_type_;
        node_data_.id                 = node_data.id;
        node_data_.description        = node_data.description;
        node_data_.name_of_executable = node_data.name_of_executable;

        node_data_.parameters.clear();

        for (size_t i = 0; i < node_data.parameters.size(); i++)
        {
          node_data_.parameters.push_back(node_data.parameters[i]);
        }

        node_data_.input_files.clear();
        node_data_.input_status.clear();

        for (size_t i = 0; i < node_data.input_files.size(); i++)
        {
          node_data_.input_files.push_back(node_data.input_files[i]);
          node_data_.input_status.push_back(Disconnected);
        }

        node_data_.output_files.clear();
        node_data_.output_connections.clear();

        for (size_t i = 0; i < node_data.output_files.size(); i++)
        {
          node_data_.output_files.push_back(node_data.output_files[i]);
          node_data_.output_connections.push_back(0);
        }

        node_data_.dim.radius       = d_ / 2;
        node_data_.dim.h_strip_up   = 2 * d_;
        node_data_.dim.h_strip_mid  = 2 * d_;
        node_data_.dim.h_strip_down = 2 * d_;
        node_data_.dim.height       = height;
        node_data_.dim.width        = width;


        x = -half_width + ((width - w1) / 2) - d_;
        y = -half_height + d_;

        node_data_.pos.input_files.clear();

        for (int i = 0; i < t_inputs; i++)
        {
          x += (2 * d_);
          node_data_.pos.input_files.push_back(QPointF(x,y));
        }

        x = -half_width + ((width - w3) / 2) - d_;
        y =  half_height - d_;

        node_data_.pos.output_files.clear();

        for (int i = 0; i < t_outputs; i++)
        {
          x += (2 * d_);
          node_data_.pos.output_files.push_back(QPointF(x,y));
        }

        break;

      case Repository:

        polygon_ << QPointF(-40, -60) << QPointF( 40, -60)
                 << QPointF( 40,  60) << QPointF(-40,  60)
                 << QPointF(-40, -60);

        break;

      default:

        return;
    }

    setPolygon(polygon_);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
  }
}

DiagramItem::DiagramType
DiagramItem::
diagramType
(void)
const
{
  {
    return diagram_type_;
  }
}

string
DiagramItem::
file_type_at
(int mode,
 int pos)
{
  {
    size_t p = pos;

    if (mode == InputConnection)
    {
      if (p >= node_data_.input_files.size()) return "";

      return (node_data_.input_files[p].file_type_id);
    }
    else // OutputConnection
    {
      if (p >= node_data_.output_files.size()) return "";
      return (node_data_.output_files[p].file_type_id);
    }
  }
}

connection*
DiagramItem::
get_connection_at
(int pos)
{
  {
    if ((pos < 0) || (pos >= connections_.size())) return nullptr;
    return connections_[pos];
  }
}

QString
DiagramItem::
get_id
(void)
{
  {
    return QString::fromStdString(node_data_.id);
  }
}

int
DiagramItem::
get_num_id
(void)
const
{
  {
    return num_id_;
  }
}

int
DiagramItem::
get_seq_id
(void)
const
{
  {
    return seq_id_;
  }
}

int
DiagramItem::
get_total_connections
(void)
{
  {
    return connections_.size();
  }
}

void
DiagramItem::
highlight_inputs
(string filetype)
{
  {
    for (size_t i = 0; i < node_data_.input_files.size(); i++)
    {
      if (node_data_.input_status[i] == Disconnected)
      {
        bool refresh = false;

        if (filetype == "")
        {
          //
          // When the input filetype is empty, it means that we must
          // highlight all the disconnected input files.
          //

          node_data_.input_status[i] = Candidate;
          refresh = true;
        }
        else
        {
          //
          // In this case we only must highlight the disconnected input
          // files whose file type matches the requested one.
          //

          string mytype = node_data_.input_files[i].file_type_id;
          if (mytype == filetype)
          {
            node_data_.input_status[i] = Candidate;
            refresh = true;
          }
        }

        //
        // If we've changed the status of at least one of our input
        // files we must redraw the item.
        //

        if (refresh) update(boundingRect());
      }
    }
  }
}

QPixmap
DiagramItem::
image
(void)
const
{
  {
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 2));
    painter.translate(125, 125);

    if (diagram_type_ == Task)
    {
      painter.drawRect    (-110, -60, 220, 120);
      painter.drawLine    (-110, -20, 110, -20);
      painter.drawLine    (-110,  20, 110,  20);

      painter.setBrush(QBrush(Qt::red));
      painter.setPen(QPen(Qt::red));

      painter.drawEllipse (-90, -50, 20, 20);
      painter.drawEllipse (-50, -50, 20, 20);
      painter.drawEllipse (-10, -50, 20, 20);
      painter.drawEllipse ( 30, -50, 20, 20);
      painter.drawEllipse ( 70, -50, 20, 20);

      painter.drawEllipse (-50,  30, 20, 20);
      painter.drawEllipse (-10,  30, 20, 20);
      painter.drawEllipse ( 30,  30, 20, 20);
    }
    else
    {
      painter.drawRect    (-60, -110, 120, 220);
      painter.drawLine    (-60,  -40,  60, -40);
      painter.drawLine    (-60,   40,  60,  40);

      painter.setBrush(QBrush(Qt::black));

      painter.drawEllipse ( 0, -75, 10, 10);
      painter.drawEllipse ( 0,   0, 10, 10);
      painter.drawEllipse ( 0,  75, 10, 10);
    }

    return pixmap;
  }
}

bool
DiagramItem::
is_task
(void)
const
{
  {
    return (diagram_type_ == Task);
  }
}

bool
DiagramItem::
is_repository
(void)
const
{
  {
    return (diagram_type_ == Repository);
  }
}

bool
DiagramItem::
is_valid_input_connection
(string& file_type)
{
  {
    if (diagram_type_ == Repository) return true;

    for (size_t i = 0; i < node_data_.input_files.size(); i++)
    {
      if (node_data_.input_status[i] == Disconnected)
      {

        if (file_type == "") return true; // The connection comes from a repository.

        if (node_data_.input_files[i].file_type_id == file_type) return true; // Task to task connection.
      }
    }

    return false;
  }
}

QVariant
DiagramItem::
itemChange
(      GraphicsItemChange change,
 const QVariant&          value)
{
  {
    if (change == QGraphicsItem::ItemPositionChange)
    {
      for (connection *conn : qAsConst(connections_)) conn->updatePosition();
    }

    return value;
  }
}

void
DiagramItem::
output_tt_connections
(QVector<int>& conns)
{
  {
    set<int> oconns;
    set<int>::iterator it;

    conns.clear();

    //
    // Retrieve all the output connections. There may be
    // repeated destinations, but we'll use a set to
    // keep only one copy of them.
    //

    for (int i = 0; i < connections_.size(); i++)
    {
      DiagramItem* end_point = connections_[i]->endItem();

      if (end_point->diagram_type_ == Task)
      {
        int end_point_id = end_point->get_num_id();

        if (end_point_id != num_id_)
        {

          //
          // Note that the connections for each item include not only those that start
          // at the item itself, but also the ones that end at the item. This is the
          // the reason why we must check that the identifiers do not match!!!
          //

          oconns.insert(end_point_id);
        }
      }
    }

    // If the set is empty, we have no connections.

    if (oconns.size() == 0) return;

    //
    // Now we're sure that there are no repeated connections.
    // Copy these to the output array.
    //

    for (it = oconns.begin(); it != oconns.end(); ++it)
    {
      int con_id = *it;
      conns.push_back(con_id);
    }
  }
}

void
DiagramItem::
paint
(      QPainter*                 painter,
 const QStyleOptionGraphicsItem* option ,
       QWidget*                  widget)
{
  {
    QBrush b_red   (Qt::red);
    QBrush b_green (Qt::green);
    QBrush b_blue  (Qt::blue);
    QBrush b_black (Qt::black);
    QBrush brush;

    QPen   p_red   (Qt::red);
    QPen   p_green (Qt::darkGreen);
    QPen   p_blue  (Qt::darkBlue);
    QPen   pen;

    QGraphicsPolygonItem::paint(painter, option, widget);

    if (diagram_type_ == Repository)
    {
      QPen pen(Qt::black, 1, Qt::SolidLine);
      painter->setPen(pen);

      // The three rectangles.

      painter->drawRect(-30, -50, 60, 30);
      painter->drawRect(-30, -15, 60, 30);
      painter->drawRect(-30,  20, 60, 30);

      //
      // The task name. Compute the rectangle inside which it
      // must be written. Then, write it.
      //

      qreal xmin, xmax, ymin, ymax;

      xmin = (-30 * 0.9);
      xmax = -xmin;

      ymin = -50 + (30 * 0.1);
      ymax = ymin + (30 * 0.8);

      QRectF canvas(QPointF(xmin, ymin), QPointF(xmax, ymax));

      QFontMetrics fm = painter->fontMetrics();
      QString elided_text = fm.elidedText(QString::fromStdString(node_data_.id),
                                          Qt::ElideRight, canvas.width());

      painter->setPen(Qt::black);
      painter->drawText(canvas, Qt::AlignCenter, elided_text);

      // Handles for the second and third drawer.

      painter->setBrush(b_black);
      //painter->drawEllipse(0, -35, 2, 2);
      painter->drawEllipse(0, 0, 2, 2);
      painter->drawEllipse(0, 35, 2, 2);
    }

    else if (diagram_type_== Task)
    {
      QString task_name;

      //
      // Note that the task name is modified to include the
      // sequential identifier in front of the canonical
      // name of the task.
      //

      task_name  = "(" + QString::number(seq_id_) + ") ";
      task_name += QString::fromStdString(node_data_.id);

      // Draw the internal elements of the step item.

      qreal width, half_width;
      int   t_inputs, t_outputs;

      t_inputs  = (int)node_data_.input_files.size();
      t_outputs = (int)node_data_.output_files.size();

      width      = node_data_.dim.width;
      half_width = width / 2.0;

      // The two lines dividing the step.

      QPen pen(Qt::black, 1, Qt::SolidLine);
      painter->setPen(pen);

      painter->drawLine(-half_width, -d_, half_width, -d_);
      painter->drawLine(-half_width,  d_, half_width,  d_);

      // The upper & lower row of circles (inputs).

      for (int i = 0; i < t_inputs; i++)
      {
        ConnectionStatus stat = node_data_.input_status[i];

        if (stat == Disconnected)
        {
          brush = b_red;
        }
        else if (stat == Candidate)
        {
          brush = b_blue;
        }
        else
        {
          brush = b_green;
        }

        painter->setBrush(brush);
        painter->drawEllipse(node_data_.pos.input_files[i], d_/2, d_/2);
      }

      for (int i = 0; i < t_outputs; i++)
      {
        int total_connections = node_data_.output_connections[i];

        if (total_connections == 0)
        {
          brush = b_red;
        }
        else
        {
          brush = b_green;
        }

        painter->setBrush(brush);
        painter->drawEllipse(node_data_.pos.output_files[i], d_/2, d_/2);
      }

      //
      // The task name. Then, compute the rectangle inside which it must be
      // written and, to finish, write it.
      //

      qreal xmin, xmax, ymin, ymax;

      xmax = (half_width * 0.9);
      xmin = -xmax;

      ymax = (d_ * 0.9);
      ymin = -ymax;

      QRectF canvas(QPointF(xmin, ymin), QPointF(xmax, ymax));

      QFontMetrics fm = painter->fontMetrics();
      QString elided_text = fm.elidedText(task_name,  Qt::ElideRight, canvas.width());

      TaskStatus status = task_status();

      if ((status == MissingInputs) || (status == MissingAllOutputs))
        pen = p_red;
      else if (status == MissingSomeOutputs)
        pen = p_blue;
      else // status == Complete
        pen = p_green;

      painter->setPen(pen);
      painter->drawText(canvas, Qt::AlignCenter, elided_text);
    }
  }
}

QPolygonF
DiagramItem::
polygon
(void)
const
{
  {
    return polygon_;
  }
}

void
DiagramItem::
remove_connection
(connection *conn)
{
  {
    connections_.removeAll(conn);
  }
}

void
DiagramItem::
remove_connections
(void)
{
  {
    //
    // We need a copy here since remove_connection() will
    // modify the connections container
    //

    const auto connections_copy = connections_;

    for (connection *conn : connections_copy)
    {
      //
      // Change the status of the start and ending points. In
      // the case of output connections we must decrease the connections
      // counter by one; when dealing with input connections, we
      // must set its status to "Disconnected".
      //
      // Since connections are stored TWICE (once per task involved
      // in the connection) we only need to perform the steps
      // above once; otherwise, we would decrease the connections
      // counter twice instead of once.
      //

      subnode_data start_node, end_node;
      start_node = conn->startNode();
      end_node   = conn->endNode();

      conn->startItem()->update_output_connection(start_node, -1);
      conn->endItem()->update_input_connection(end_node, Disconnected);

      // Remove the connection from both the starting and ending tasks.

      conn->startItem()->remove_connection(conn);
      conn->endItem()->remove_connection(conn);

      // Remove the connection from the scene.

      scene()->removeItem(conn);

      // Delete the connection itself.

      delete conn;
    }
  }
}

void
DiagramItem::
reset_highlights
(void)
{
  {
    bool refresh = false;

    for (size_t i = 0; i < node_data_.input_files.size(); i++)
    {
      if (node_data_.input_status[i] == Candidate)
      {
        node_data_.input_status[i] = Disconnected;
        refresh = true;
      }
    }

    if (refresh) update(boundingRect());
  }
}

void
DiagramItem::
set_id
(QString& id)
{
  {
    if (diagram_type_ != Repository) return;

    node_data_.id = id.toStdString();
  }
}

void
DiagramItem::
set_num_id
(int num_id)
{
  {
    num_id_ = num_id;
  }
}

void
DiagramItem::
set_seq_id
(int seq_id)
{
  {
    seq_id_ = seq_id;
  }
}

bool
DiagramItem::
subnode_coordinates
(subnode_data& subnode,
 QPointF&      coordinates)
{
  {
    if (diagram_type_ == Repository)
    {
      coordinates = QPointF(0,0);
      return true;
    }

    if (diagram_type_ != Task) return false;

    if (subnode.mode == InputConnection)
    {

      //
      // The click corresponds to the subnode.position_th circle in the
      // upper strip (inputs).
      //

      int t_inputs = (int)node_data_.input_files.size();
      int i        = subnode.position;
      if ((i < 0) || (i >= t_inputs)) return false;
      coordinates = node_data_.pos.input_files[i];

      return true;
    }
    else if (subnode.mode == OutputConnection)
    {

      //
      // The click corresponds to the subnode.position_th circle in the
      // lower strip (outputs).
      //

      int t_outputs = (int)node_data_.output_files.size();
      int i         = subnode.position;
      if ((i < 0) || (i >= t_outputs)) return false;
      coordinates = node_data_.pos.output_files[i];

      return true;
    }

    //
    // This point should be never reached. The sentence is here to avoid
    // the compiler's complains.
    //

    return false;
  }
}

TaskStatus
DiagramItem::
task_status
(void)
{
  {
    int   t_inputs, t_outputs, total_missing_outputs;

    t_inputs  = (int)node_data_.input_files.size();
    t_outputs = (int)node_data_.output_files.size();

    for (int i = 0; i < t_inputs; i++)
    {
      if (node_data_.input_status[i] == Disconnected)
        return MissingInputs;
    }

    total_missing_outputs = 0;

    for (int i = 0; i < t_outputs; i++)
    {
      if (node_data_.output_connections[i] == 0)
        total_missing_outputs++;
    }

    if (total_missing_outputs == t_outputs)
      return MissingAllOutputs;

    if (total_missing_outputs != 0)
      return MissingSomeOutputs;

    return Complete;
  }
}

int
DiagramItem::
type
(void)
const
{
  {
    return Type;
  }
}

void
DiagramItem::
update_input_connection
(subnode_data&    subnode,
 ConnectionStatus status)
{
  {
    if (diagram_type_ != Task) return;

    node_data_.input_status[subnode.position] = status;

    update(boundingRect());
  }
}

void
DiagramItem::
update_output_connection
(subnode_data&    subnode,
 int              connection_delta)
{
  {
    if (diagram_type_ != Task) return;

    node_data_.output_connections[subnode.position] += connection_delta;

    update(boundingRect());
  }
}

bool
DiagramItem::
validate_click
(ConnectionMode mode,
 QPointF&       click,
 subnode_data&  where)
{
  {
    if (diagram_type_ == Repository)
    {
      where.item = this;
      where.position = 0;

      return true;
    }

    if (diagram_type_ != Task) return false;

    // At this point, we're sure we're a task.

    qreal distance, radius, x, y;

    radius = d_ / 2;

    if (mode == InputConnection)
    {

      //
      // Check if the click corresponds to any of the circles in the
      // upper strip (inputs).
      //

      int t_inputs = (int)node_data_.input_files.size();

      for (int i = 0; i < t_inputs; i++)
      {
        x = node_data_.pos.input_files[i].x();
        y = node_data_.pos.input_files[i].y();

        distance = qPow ((qPow(click.x() - x, 2.0) + qPow(click.y() - y, 2.0)), 0.5);
        if (distance <= radius)
        {
          //
          // This is a valid click position. However, we must guarantee that
          // it has been marked as a legal candidate!!!
          //

          if (node_data_.input_status[i] == Candidate)
          {
            //
            // Yes! Not only the user clicked inside a valid circle in the
            // item, but, moreover, this input file had not been connected
            // before and it was considered as a valid candidate for the
            // connection. Consequenly, we accept the click as valid.
            //

            where.item     = this;
            where.mode     = mode;
            where.position = i;

            return true;
          }
        }
      }

      return false;
    }
    else if (mode == OutputConnection)
    {

      //
      // Check if the click corresponds to any of the circles in the
      // lower strip (outputs).
      //

      int t_outputs = (int)node_data_.output_files.size();

      for (int i = 0; i < t_outputs; i++)
      {
        x = node_data_.pos.output_files[i].x();
        y = node_data_.pos.output_files[i].y();

        distance = qPow ((qPow(click.x() - x, 2.0) + qPow(click.y() -y, 2.0)), 0.5);
        if (distance <= radius)
        {
          where.item     = this;
          where.mode     = mode;
          where.position = i;

          return true;
        }
      }

      return false;
    }

    // This point should never reached. We return false to avoid compiler complains.

    return false;
  }
}
