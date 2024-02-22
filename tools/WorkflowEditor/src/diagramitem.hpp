/** \file diagramitem.hpp
\brief The tasks and repository graphic items that will be part of
       workflows.
*/


#ifndef DIAGRAMITEM_HPP
#define DIAGRAMITEM_HPP

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QPolygonF>
#include <QVector>

#include "connection.hpp"
#include "WFMEditor_structures.hpp"

#include <set>
#include <string>

#include <QDebug>

using namespace std;

/// \brief The tasks and repository graphic items that will be part of workflows.

class DiagramItem : public QGraphicsPolygonItem
{
  public:

    /// The type member required by all QGraphicsItems.

    enum { Type = UserType + 15 };

    /// The several kinds of diagram items.
    ///
    enum DiagramType
    {
      Task,       ///< Tasks.
      Repository  ///< Repositories.
    };

  public:

    /// \brief Add a connection to the item.
    /**
      \param[in] conn The connection to add.
     */

    void        add_connection            (connection *conn);

    /// \brief Constructor.
    /**
      \param[in] diagramType Type (task or repository) of the item.
      \param[in] node_data The data defining the task or repository.
      \param[in] contextMenu Pointer to the context menu that will be
                 displayed when right clicking on the item.
      \param[in] parent The eventual QGraphicsItem parenting this item.
                 It may be null.
     */

                DiagramItem               (DiagramType    diagramType,
                                           toolkit_task&  node_data,
                                           QMenu*         contextMenu,
                                           QGraphicsItem* parent = nullptr);

    /// \brief Return the type (task or repository) for this item.
    /**
      \return The type (task or repository) for this item.
     */

    DiagramType diagramType               (void) const;

    /// \brief Returns the type of the file located at the coordinates
    ///        stated by mode and pos.
    /**
      \param[in] mode Either "InputConnection" or "OutputConnection". Identifies
                 whether the file is, correspondingly, in the upper or lower
                 row of slots.
      \param[in] pos The position (starting at 0) of the slot in the row
                 selected by mode.
      \return    The type of the file. The result may be the empty string
                 if mode and pos point to an unexistent slot.
     */

    string      file_type_at              (int mode, int pos);

    /// \brief Retrieve the identifier of the item.
    /**
      \return The identifier for this item.
     */

    QString     get_id                    (void);

    /// \brief Return a connection stating its position inside
    ///        the list of connections.
    /**
      \param[in] pos The position (starting at 0) of the connection sought.
      \return    The connection at position pos in the list of connections or
                 null if the position is invalid (out of the range of available
                 connections).

      \see get_total_connections()
     */

    connection* get_connection_at         (int pos);

    /// \brief Retrieve the numerical identifier of the item.
    /**
      \return The numerical identifier of the item.

      Numerical identifiers are unique across the whole set of items, no
      matter whether these are tasks or repositories.

      Such identifiers may be assigned several times during the life
      of the application, and targeted at different uses, such as
      graph ordering, topological sorts, etc.

      Therefore, an item may have different numerical identifiers during
      the life span of the application.
     */

    int         get_num_id                (void) const;

    /// \brief Retrieve the sequential identifier of the item.
    /**
      \return The sequential identifier of the item.

      Sequential identifiers are used to complement the string identifiers
      of tasks. These may be repeated, since two tasks based on the
      same application / executable share the identifier of the said
      application.

      To let the user tell apart instances of tasks running the same
      program, this sequential identifier is appended to the string
      identifier.
     */

    int         get_seq_id                (void) const;

    /// \brief Get the total number of connections.
    /**
      \return The (current) total number of connections for the item.
     */

    int         get_total_connections     (void);

    /// \brief Change the color of the input slots (in a task item) whose
    ///        file type corresponds to the one requested.
    /**
      \param[in] filetype The type of file that should be highlighted.

      When trying to make a connection it is mandatory that the type
      of file corresponding to the output slot where such connection
      has been started matches the file type of the candidate input
      slots.

      This method tells all the tasks in the workflow to highlight
      (changing temporarily their color) all their slots that
      may accept such a connection. This makes easier to identify
      the legal final endpoints of the said connection.
     */

    void        highlight_inputs          (string filetype);

    /// \brief Get a schematic pixmap with a simple depiction of
    ///        the item - to be used as toolbox buttons.
    /**
      \return A pixmap showing schematically how the item
              looks like.
     */

    QPixmap     image                     (void) const;

    /// \brief Is the item a task?
    /**
      \return True if the item is a task, false otherwise (repository).
     */

    bool        is_task                   (void) const;

    /// \brief Is the item a repository?
    /**
      \return True if the item is a repository, false otherwise (task).
     */

    bool        is_repository             (void) const;

    /// \brief Decide whether a repository or a task would be able to
    ///        accept connections for some kind of file type.
    /**
      \param file_type The type of the file for which we are looking
             for connection candidates.
      \return Always true when the item is a repository. In the case
             of tasks, true if at least there exist a slot for this
             type of file that has not been connected yet, false otherwise.
     */

    bool        is_valid_input_connection (string& file_type);

    /// \brief Get the list of output, task to task connections.
    /**
      \param[out] conns The list of connections that, starting
                  at this item, end in another task item.
     */

    void        output_tt_connections     (QVector<int>& conns);

    /// \brief Retrieve the polygon defining the outer shape of the item.
    /**
      \return The polygon defining the outer shape of the item.
     */

    QPolygonF   polygon                   (void) const;

    /// \brief Remove a specific connection from the list of connections.
    /**
      \param[in] conn The connection to remove.
     */

    void        remove_connection         (connection *conn);

    /// \brief Remove all the connections for this item.

    void        remove_connections        (void);

    /// \brief Reset to "disconnected" the state of all the input slots that
    ///        had been highlighted by highlight_inputs() and that have not
    ///        been connected since then.

    void        reset_highlights          (void);

    /// \brief Set the identifier of the item.
    /**
      \param[in] id The value of the identifier.
     */

    void        set_id                    (QString& id);

    /// \brief Set the numerical identifier of the item.
    /**
      \param[in] num_id The value of the numerical identifier.

      \see get_num_id
     */

    void        set_num_id                (int num_id);

    /// \brief Set the sequentail identifier of the item.
    /**
      \param[in] seq_id The value of the sequential identifier.

      \see get_seq_id
     */

    void        set_seq_id                (int seq_id);

    /// \brief Get the local (item) coordinates of the requested slot.
    /**
      \param[in]  subnode Data defining the slot for which the local
                  coordinates are sought.
      \param[out] coordinates The local coordinates of the requested
                  slot.
      \return True if the requested slot exists, false otherwise.

      If the item is a repository, this method always returns true
      and the coordinates are set to (0,0).

      In the case of tasks, the row (input or output) stated by subnode
      is investigated to return the actual local coordinates of
      the specific slot number requested. If such slot does not
      exist, the method returns false.
     */

    bool        subnode_coordinates       (subnode_data& subnode, QPointF& coordinates);

    /// \brief Get the connection status of the item.
    /**
      \return The connection status of the item:
        - If none of the input slots has been assigned, the method returns
          MissingAllInputs.
        - Otherwise, the status of the output slots is checked. If none
          of the output slots is connected, the method returns
          MissingAllOutputs, but if at least one output slot has been
          connected, the returned value is MissingSomeOutputs.
        - Otherwise (that is, all input and output slots are connected)
          the method returns Connected.
     */

    TaskStatus  task_status               (void);

    /// \brief Return the type of the item as an int. This method overrides that
    ///        of its parent class.
    /**
      \return The type of the item as an int.
     */

    int         type                      (void) const override;

    /// \brief Set the status of the requested input slot.
    /**
      \param[in] subnode Description of the input slot whose status must be changed.
      \param[in] status The new status of the slot.
     */

    void        update_input_connection   (subnode_data& subnode, ConnectionStatus status);

    /// \brief Update the number of connections the requested output slot.
    /**
      \param[in] subnode Description of the output slot whose number of connections must
                 be updated.
      \param[in] connection_delta Number of connections to add (positive) or remove (negative)
                 to the output slot.
     */

    void        update_output_connection  (subnode_data& subnode,
                                           int           connection_delta);

    /// \brief Check if a click took place in a valid slot and, if so, return its local
    ///        (item) coordinates.
    /**
      \param[in] mode States whether the slot sought should be either an input or an output one.
      \param[in] click Local coordinates of the point where the click took place.
      \param[out] where The description of the slot where the click took place, if any is found.
      \return True if a slot of the type (input / output) sought lies beneath the position
                 pointed to by parameter click.
     */

    bool        validate_click            (ConnectionMode mode, QPointF& click, subnode_data& where);

  protected:

    /// \brief Reimplementation of the parent's method. Callback to process an event int
    ///        the item's context menu.
    /**
      \param[in] event The event to be handled.
     */

    void        contextMenuEvent          (QGraphicsSceneContextMenuEvent *event) override;

    /// \brief Reimplementation of the parent's method. Callback to notify custom items that some
    ///        part of the item's state has changed.
    /**
      \param[in] change The parameter of the item that is changing.
      \param[in] value The new value of the parameter changed.
      \return The input value is returned.

      This implementation only takes care of position changes. When this happens, the connections
      for this node are redrawn.
     */

    QVariant    itemChange                (GraphicsItemChange change, const QVariant &value) override;

    /// \brief Paint the contents of the item in local coordinates.
    /**
      \param[in] painter The painter that will take care of painting the item.
      \param[in] option  Style options for the item, such as its state, exposed area
                         and its level-of-detail hints.
      \param[in] widget  Optional. If provided, it points to the widget that is being
                         painted on; otherwise, it is 0. For cached painting, widget
                         is always 0.
     */
    void        paint                     (QPainter* painter, const QStyleOptionGraphicsItem* option,
                                           QWidget*  widget = nullptr) override;

  private:

    /// \brief The list of connections in which this item is involved.

    QVector<connection*> connections_;

    /// \brief Pointer to the menu object shown when right-clicking on
    ///        the item.

    QMenu*               context_menu_;

    /// \brief Diameter, in local (item's) coordinates, used to draw
    ///        the input / output slots of Task diagram items.

    qreal                d_;

    /// \brief Type of the item (either Task or Repository)

    DiagramType          diagram_type_;

    /// \brief Characterization of the item. It includes information such
    ///        as identifiers (either string or numeric), number of inputs
    ///        or output slots, etc.

    node_data            node_data_;

    /// \brief Temporary numerical identifier, used when checking graph cycles.

    int                  num_id_;

    /// \brief Outer polygon defining the limits of the item.

    QPolygonF            polygon_;

    /// \brief Sequential identifier for tasks, to tell those with the same
    ///        string identifier apart.

    int                  seq_id_;
};

#endif // DIAGRAMITEM_HPP
