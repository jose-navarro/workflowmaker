/** \file diagramscene.hpp
\brief The scene where the graphic items will be included.
*/

#ifndef DIAGRAMSCENE_HPP
#define DIAGRAMSCENE_HPP

#include "diagramitem.hpp"
#include "graph.hpp"
#include "WFMEditor_structures.hpp"
#include "workflow_structures.hpp"

#include <QColor>
#include <QFont>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPointF>
#include <QTextCursor>

#include <QDebug>

#include <map>
#include <set>
#include <string>

using namespace std;

/// \brief The scene where the graphic items will be included.

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

  public:

    /// The pointer modes.

    enum Mode {
      InsertItem, ///< Insert new items (tasks, repositories).
      InsertLine, ///< Insert lines (connections).
      MoveItem    ///< Move items (only tasks and repositories, not lines).
    };

  public:

    /// \brief Build the workflow using the tasks, repositories
    ///        and connections currently present in the scene.
    /**
      \param[out] workflow The workflow, once built by this method.
      \return An error code:
                - 0: Successful completion. The parameter
                     "workflow" containts the workflow ready
                     to be saved.
                - 1: There are neither tasks nor repositories
                     in the current scene; no workflow available.
     */

    int      build_workflow       (WFWorkflow& workflow);

    /// \brief Check the status of the workflow.
    /**
      \param[out] missing_inputs Total number of tasks missing all inputs.
      \param[out] missing_some_outputs Total number of tasks missing at least
                  one of their outputs.
      \param[out] missing_all_outputs Total number of tasks missing all their
                  outputs.
      \param[out] disconnected_repos Total number of repositories that have
                  no connections at all.
     */

    void     check_wf_status      (int& missing_inputs,
                                   int& missing_some_outputs,
                                   int& missing_all_outputs,
                                   int& disconnected_repos);

    /// \brief Constructor.
    /**
      \param[in] itemMenu Pointer to the context menu to show when a right
                 click over an item happens.
      \param[in] parent The parent of this widget.
     */

    explicit DiagramScene         (QMenu* itemMenu, QObject *parent = nullptr);

    /// \brief Insert a complete workflow in the scene.
    /**
      \param[in] wf The workflow to insert.

      Once a workflow has been loaded by the main window, it may be inserted
      in the scene. This method takes care of adding the whole set of
      tasks, repositories and connections to the scene.

      It may be called only when the toolkit has been set! (see
      setToolkit()).

     */

    bool     insert_workflow      (WFWorkflow& wf);

    /// \brief Set the current task.
    /**
      \param[in] task_number The number of the task that will become
                 the current one.

      The number of the current task is used to select the
      one that will be added to the workflow when the
      user proceeds to insert a new item.
     */

    void     setCurrentTask       (int task_number);

    /// \brief Set the toolkit used to design the workflow.
    /**
      \param[in] tk The toolkit to set.

      The toolkit defines the set of tasks available to design
      a workflow. This is the reason why a toolkit must be
      selected.
     */

    void     setToolkit           (toolkit& tk);

  public slots:

    /// \brief Set the item's type.
    /**
      \param[in] type The new item's type (task or repository).
     */

    void     setItemType          (DiagramItem::DiagramType type);

    /// \brief Set the current pointer mode.
    /**
      \param[in] mode The new pointer mode.
     */

    void     setMode              (Mode mode);

  signals:

    /// \brief Signal that a new item has been inserted.
    /**
      \param[in] item Pointer to the item that has just been
                 inserted.
     */

    void     itemInserted         (DiagramItem* item);

    /// \brief Signal that an item has been selected.
    /**
      \param[in] item Pointer to the item that has just been
                 selected.
     */

    void     itemSelected         (QGraphicsItem* item);


  protected:

    /// \brief Build a list of candidates willing to accept a certain kind
    ///        of connection.
    /**
      \param[in] start The item starting the connection. Needed to make
                 sure that it is excluded from the list of candidates
                 to accept the connection.
      \param[in] start_slot Description of the slot in the initial item
                 where the connection is initiated. Used to ascertain
                 what are the candidates to finish the connection.
      \param[in] candidates The list of items willing to accept a
                 connection with the characteristics defined by start_slot.
     */

    void     build_candidate_list (DiagramItem*           start,
                                   subnode_data&          start_slot,
                                   QVector<DiagramItem*>& candidates);

    /// \brief Build a graph including only the tasks currently present
    ///        in the workflow.

    void     build_task_graph     (void);

    /// \brief Check what are the extra edges that, starting at some item,
    ///        would create cycles when connected to a list of task
    ///        candidates.
    /**
      \param[in] starting_item The item that is starting a new connection.
      \param[in] total_candidates The list of candidates that would accept
                 such a connection.
      \param[out] final_candidates The list of candidates that, in the case
                 that the connection would end at them, would create no
                 cycles in the workflow.

      This method guarantees that the addition of a new edge will not
      produce cycles in the graph. Cycles cannot be allowed, since
      this would mean that the output of some task would be needed
      by some predecessor task!

      To do this the tasks included in a list of candidates accepting some
      kind of connection (see build_candidate_list()) are checked one
      by one, simulating that the connection is included in the graph. Then,
      if this new connection produces no cycles, it is added to the final
      candidate list; if, on the contrary, such addition produces a cycle,
      the offending candidate is ignored.

      This method should be called each time that a new connection is
      created.
     */

    void     check_for_cycles     (DiagramItem*           starting_item,
                                   QVector<DiagramItem*>& total_candidates,
                                   QVector<DiagramItem*>& final_candidates);

    /// \brief Count the number of repositories in the workflow.
    /**
      \return The current number of repositories present in the workflow.
     */

    int      count_repositories   (void);

    /// \brief Highlight (change the colour of) all the input slots compatible
    ///        with some kind of connection, to offer a visual hit when
    ///        finishing such connection.
    /**
      \param[in] filetype The type of file intervening in the connection.
      \param[in] candidates The list of candidates willing to accept a connection
                 involving the said file type.

      \see build_candidate_list()
      \see check_for_cycles()
      \see reset_highlights()
     */

    void     highlight_candidates (string&                filetype,
                                   QVector<DiagramItem*>& candidates);

    /// \brief Event handler to keep track of mouse movements. Overrides the
    ///        parent class' implementation.
    /**
      \param[in] mouseEvent The mouse move event.

      This method takes care of moving the items in the workflow when the
      pointer mode is set to MoveItem or, when it is InsertLine, to draw
      the line going from the start of the connection to the current
      mouse position.
     */

    void     mouseMoveEvent       (QGraphicsSceneMouseEvent* mouseEvent) override;

    /// \brief Event handler for mouse press events. Overrides the parent
    ///        class' implementation.
    /**
      \param[in] mouseEvent The mouse press event.

      This method takes care of inserting a new item (either a task or a
      repository, this depends on the current value of item_type_) when
      the pointer mode is set to InsertItem; on the contrary, when such
      mode is set to InsertLine, it starts a new connection, whose origin
      is located at the output slot or repository on which the user
      clicked.
     */

    void     mousePressEvent      (QGraphicsSceneMouseEvent* mouseEvent) override;

    /// \brief Event handler for mouse relese events. Overrides the parent
    ///        class' implementation.
    /**
      \param[in] mouseEvent The mouse release event.

      This method takes care of finishing an ongoing connection, if any (which
      means that the current pointer mode is InserLine and that the user
      already started the connection, see MousePressEvent()).
     */

    void     mouseReleaseEvent    (QGraphicsSceneMouseEvent* mouseEvent) override;

    /// \brief Reset to "normal" all the input slots that had been marked
    ///        as candidates to finish a connection and that have not been
    ///        actually connected.

    void     reset_highlights     (void);

  private:

    /// \brief Number of the task currently selected by the user.

    int                      current_task_;

    /// \brief Scene coordinates of the final point of an ongoing
    ///        connection.

    QPointF                  end_point_;

    /// \brief Detailed information about the slot where a connection ends.

    subnode_data             end_subnode_;

    /// \brief Flag stating whether a toolkit as already been selected.

    bool                     got_toolkit_;

    /// \brief Pointer to the context menu to show when right-clicking over
    ///        an item.

    QMenu*                   item_menu_;

    /// \brief Kind or item (task, repository) currently selected for
    ///        insertion.

    DiagramItem::DiagramType item_type_;

    /// \brief A line to store and show a connection that is in the
    ///        process of being drawn.

    QGraphicsLineItem*       line_;

    /// \brief Current pointer mode.

    Mode                     mode_;

    /// \brief Total number of repositories already inserted in the
    ///        workflow, including those that the user might have
    ///        deleted.

    int                      repo_counter_;

    /// \brief Scene coordinates of the initial point of an ongoing
    ///        connection.

    QPointF                  start_point_;

    /// \brief Detailed information about the slot where a connection starts.

    subnode_data             start_subnode_;

    /// \brief Graph for the workflow, including only tasks (repositories
    ///        are NOT included.

    Graph*                   task_graph_;

    /// \brief Total number of tasks already inserted in the
    ///        workflow, including those that the user might have
    ///        deleted.

    int                      task_counter_;

    /// \brief The toolkit to use.

    toolkit                  tk_;

};

#endif // DIAGRAMSCENE_HPP
