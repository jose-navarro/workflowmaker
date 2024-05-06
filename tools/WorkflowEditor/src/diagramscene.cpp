/** \file diagramscene.cpp
\brief Implementation file for diagramscene.hpp.
*/

#include "diagramscene.hpp"

void
DiagramScene::
build_candidate_list
(DiagramItem*           start,
 subnode_data&          start_slot,
 QVector<DiagramItem*>& candidates)
{
  {
    QList<QGraphicsItem *> the_items;
    QVector<DiagramItem*>  the_tasks;
    string                 type;

    candidates.clear();

    // Get the file type for the requested slot in the task willing to make a connection.

    type = start->file_type_at(OutputConnection, start_slot.position);

    //
    // Get all the items in the scene. Keep only the tasks excluding the
    // starting one (we'll never connect the inputs and outputs of the
    // SAME task!!!).
    //

    the_items = items();

    the_tasks.clear();

    for (int i = 0; i < the_items.count(); i++)
    {
      QGraphicsItem* gi = the_items.at(i);

      if (gi->type() != connection::Type)
      {
        DiagramItem* it = qgraphicsitem_cast<DiagramItem *>(gi);

        if (it->is_task())
        {
          if ((it != start) && (it->is_valid_input_connection(type)))
            the_tasks.push_back(it);
        }
      }
    }

    // If we have no tasks, there are no possible candidates.

    if (the_tasks.count() == 0) return;

    //
    // We have at least one task to check. Let's see if there are
    // any of them willing to accept our connection...
    //

    for (int i = 0; i < the_tasks.count(); i++)
    {
      if (the_tasks[i]->is_valid_input_connection(type))
        candidates.push_back(the_tasks[i]);
    }

    // That's all.

    return;
  }
}

void
DiagramScene::
build_task_graph
(void)
{
  {
    bool delete_graph;
    int  id;
    QVector<DiagramItem*> the_tasks;

    //
    // First of all, assign a sequential number to all the
    // Tasks in the scene (we'll ignore the Repositories).
    //

    // Get all the items in the scene.

    QList<QGraphicsItem *> the_items = items();

    // Assign a sequential ascending numeric id to tasks.

    id = 0;
    the_tasks.clear();

    for (int i = 0; i < the_items.count(); i++)
    {
      QGraphicsItem* gi = the_items.at(i);

      if (gi->type() != connection::Type)
      {
        DiagramItem* it = qgraphicsitem_cast<DiagramItem *>(gi);

        if (it->is_task())
        {
          it->set_num_id(id);
          the_tasks.push_back(it);

          id++;
        }
      }
    }

    //
    // Here, id contains the total number of tasks, which
    // is the same than saying that it contains the total
    // number of vertices in our graph.
    //
    // Now, we'll build it. We'll iterate over
    // the tasks asking them to tell us to who they
    // connect. We'll use this information to build the
    // graph.
    //

    if (task_graph_ != nullptr)
    {
      delete task_graph_;
      task_graph_ = nullptr;
    }

    // No nodes, no graph.

    if (id == 0) return;

    // Let's go!

    delete_graph = true;
    task_graph_ = new Graph(id); // id vertices in total.

    //
    // Go over the list of tasks, ask them to tell us what their
    // output connections are, and use this information to build
    // the graph.
    //
    // Note that in the case that no connections exist at all, the
    // graph will be deleted at the end of this method.
    //

    for (int i = 0; i < the_tasks.count(); i++)
    {
      DiagramItem* it = the_tasks[i];

      int          from;
      QVector<int> to;

      to.clear();

      from = it->get_num_id();
      it->output_tt_connections(to);

      if (to.size() > 0)
      {
        // Here we're inserting edges to the graph, so we will end up
        // having something useful. Let's state that, consequently,
        // we won't to delete the graph at the end of this method.

        delete_graph = false;

        for (int j = 0; j < to.count(); j++)
        {
          task_graph_->add_edge(from, to[j]);
        }
      }
    }

    if (delete_graph)
    {
      // When no edges exist, it makes no sense to keep the graph.

      delete task_graph_;
      task_graph_ = nullptr;
    }
  }
}

int
DiagramScene::
build_workflow
(WFWorkflow& workflow)
{
  {
    set<WFConnection>::iterator conn_it;
    int                         index;
    map<int, DiagramItem*>      num_id_to_item;
    QVector<DiagramItem*>       tasks_and_repos;
    bool                        tasks_are_sorted;
    QList<QGraphicsItem *>      the_items;
    vector<int>                 topo_sort;
    set<WFConnection>           unique_connections;

    //
    // Get all the items in the scene. We will ignore the lines, since
    // there is nothing we must do with these. Number tasks and repos
    // sequentially. Keep a map of numbers to items to be able to
    // benefit from the topological sort that takes place later on.
    //

    num_id_to_item.clear();
    the_items = items();
    tasks_and_repos.clear();
    index = 0;

    for (int i = 0; i < the_items.count(); i++)
    {
      QGraphicsItem* gi = the_items.at(i);

      if (gi->type() != connection::Type)
      {
        DiagramItem* it = qgraphicsitem_cast<DiagramItem *>(gi);
        it->set_num_id(index);

        tasks_and_repos.push_back(it);
        num_id_to_item.insert(std::make_pair(index, it));

        index++;
      }
    }

    // Check that we have, at least, a single task or repo!!!

    if (index == 0) return 1; // Nothing to save!!!

    //
    // Now, for each repository or task, we will scan all their connections.
    // Remember that these are DUPLICATED (a connection between a and b is
    // stored in the list of connections for a and b!).
    //
    // We will replace the pointers to the actual repositories or tasks
    // by the numerical ids with which we have just labelled them.
    //
    // Then we will insert these "renamed" connections into a set that
    // will keep only unique copies of each of them. In this way we'll
    // remove duplicates.
    //

    unique_connections.clear();
    Graph wf(index);

    for (int i = 0; i < tasks_and_repos.size(); i++)
    {
      int          total_connections;
      DiagramItem* it = tasks_and_repos[i];

      total_connections = it->get_total_connections();

      for (int j = 0; j < total_connections; j++)
      {
        DiagramItem* start_item;
        DiagramItem* end_item;
        WFConnection renamed_conn;

        connection* conn = it->get_connection_at(j);

        start_item = conn->startItem();
        end_item   = conn->endItem();

        renamed_conn.from.endpoint_id = start_item->get_num_id();
        if (start_item->is_task()) renamed_conn.from.type = TaskEndPoint;
        else                       renamed_conn.from.type = RepoEndPoint;
        renamed_conn.from.slot = conn->startNode().position;

        renamed_conn.to.endpoint_id = end_item->get_num_id();
        if (end_item->is_task()) renamed_conn.to.type = TaskEndPoint;
        else                     renamed_conn.to.type = RepoEndPoint;
        renamed_conn.to.slot = conn->endNode().position;

        unique_connections.insert(renamed_conn);
      }
    }

    //
    // Now we'll build the graph representing our repos, tasks and
    // the connections between them. Once built, we'll sort it
    // topologically. But we'll do this only if we have, at least,
    // one connection!!!
    //

    tasks_are_sorted = false;

    if (unique_connections.size() > 0)
    {

      for (conn_it  = unique_connections.begin();
           conn_it != unique_connections.end();
           conn_it++)
      {
        WFConnection c = *conn_it;
        int from, to;

        from = c.from.endpoint_id;
        to   = c.to.endpoint_id;

        wf.add_edge(from,to);
      }

      wf.topological_sort(topo_sort);
      tasks_are_sorted = true;
    }

    //
    // Build the workflow.
    //

    workflow.tasks.clear();
    workflow.repos.clear();
    workflow.connections.clear();

    // The workflow id and identifier will be set by our main window.

    workflow.id           = "";
    workflow.description  = "";

    // Set the remaining "lineage" attributes.

    workflow.toolkit_id   = tk_.id;
    workflow.last_repo_id = repo_counter_;
    workflow.last_task_id = task_counter_;

    // Repositories.

    for (int i = 0; i < tasks_and_repos.size(); i++)
    {
      DiagramItem* it = tasks_and_repos[i];
      WFNode       node;

      if (!it->is_task())
      {
        node.id = it->get_id().toStdString();
        node.numerical_id = it->get_num_id();
        node.pos.x = it->pos().x();
        node.pos.y = it->pos().y();

        workflow.repos.push_back(node);
      }
    }

    //
    // Tasks. Follow the order set by the topological sort
    // if such sort took place. Otherwise, save the tasks
    // as they are found in the tasks_and_repos array...
    //

    if (tasks_are_sorted)
    {
      for (size_t i = 0; i < topo_sort.size(); i++)
      {
        map<int, DiagramItem*>::iterator finder;
        DiagramItem*                     it;
        WFNode                           node;


        finder = num_id_to_item.find(topo_sort[i]);
        if (finder != num_id_to_item.end())
        {
          it = finder->second;

          if (it->is_task())
          {
            node.id = it->get_id().toStdString();
            node.numerical_id = it->get_num_id();
            node.sequential_id = it->get_seq_id();
            node.pos.x = it->pos().x();
            node.pos.y = it->pos().y();

            workflow.tasks.push_back(node);
          }
        }
      }
    }
    else
    {
      for (int i = 0; i < tasks_and_repos.size(); i++)
      {
        DiagramItem* it = tasks_and_repos[i];
        WFNode       node;

        if (it->is_task())
        {
          node.id = it->get_id().toStdString();
          node.numerical_id = it->get_num_id();
          node.sequential_id = it->get_seq_id();
          node.pos.x = it->pos().x();
          node.pos.y = it->pos().y();

          workflow.tasks.push_back(node);
        }
      }
    }

    // Connections.

    for (conn_it  = unique_connections.begin();
         conn_it != unique_connections.end();
         conn_it++)
    {
      WFConnection c = *conn_it;
      workflow.connections.push_back(c);
    }

    // That's all.

    return 0;

  }
}

void
DiagramScene::
check_wf_status
(int& missing_inputs,
 int& missing_some_outputs,
 int& missing_all_outputs,
 int& disconnected_repos)
{
  {
    QList<QGraphicsItem *> the_items;

    missing_inputs       = 0;
    missing_some_outputs = 0;
    missing_all_outputs  = 0;
    disconnected_repos   = 0;

    // Get the whole list of elements in the scene.

    the_items = items();

    // For every item (that is not a line) check its status.

    for (int i = 0; i < the_items.count(); i++)
    {
      QGraphicsItem* gi = the_items.at(i);

      if (gi->type() != connection::Type)
      {
        DiagramItem* it = qgraphicsitem_cast<DiagramItem *>(gi);

        if (it->is_task())
        {
          // Task. Check its status.

          TaskStatus status = it->task_status();

          if      (status == MissingInputs)      missing_inputs++;
          else if (status == MissingSomeOutputs) missing_some_outputs++;
          else if (status == MissingAllOutputs)  missing_all_outputs++;
        }
        else
        {
          // Repository. Check its number of connections.

          if (it->get_total_connections() == 0)  disconnected_repos++;
        }
      }
    }
  }
}

void
DiagramScene::
check_for_cycles
(DiagramItem*           starting_item,
 QVector<DiagramItem*>& total_candidates,
 QVector<DiagramItem*>& final_candidates)
{
  {
    int end_edge;
    int start_edge;

    final_candidates.clear();
    start_edge = starting_item->get_num_id();

    for (int i = 0; i < total_candidates.count(); i++)
    {
      DiagramItem* end_item = total_candidates[i];
      end_edge = end_item->get_num_id();

      task_graph_->add_edge(start_edge, end_edge);
      if (!task_graph_->is_cyclic())
      {
        final_candidates.push_back(end_item);
      }
      task_graph_->remove_last_edge();
    }
  }
}

int
DiagramScene::
count_repositories
(void)
{
  {
    int result = 0;

    // Get all the items in the scene.

    QList<QGraphicsItem *> the_items = items();

    // Count the repository items.

    for (int i = 0; i < the_items.count(); i++)
    {
      QGraphicsItem* gi = the_items.at(i);

      if (gi->type() != connection::Type)
      {
        DiagramItem* it = qgraphicsitem_cast<DiagramItem *>(gi);

        if (!it->is_task()) result++;
      }
    }

    return result;
  }
}

DiagramScene::
DiagramScene
(QMenu*   itemMenu,
 QObject* parent)
    : QGraphicsScene(parent)
{
  {
    current_task_ = -1;
    got_toolkit_  = false;
    task_graph_   = nullptr;
    item_menu_    = itemMenu;
    item_type_    = DiagramItem::Task;
    line_         = nullptr;
    mode_         = MoveItem;
    repo_counter_ = 0;
    task_counter_ = 0;
  }
}

void
DiagramScene::
highlight_candidates
(string&                filetype,
 QVector<DiagramItem*>& candidates)
{
  {
    for (int i = 0; i < candidates.count(); i++)
    {
      DiagramItem* item = candidates[i];
      item->highlight_inputs (filetype);
    }
  }
}

bool
DiagramScene::
insert_workflow
(WFWorkflow& wf)
{
  {
    //
    // To be able to insert a workflow, a toolkit must
    // be available.
    //

    if (!got_toolkit_) return false;

    // Retrieve the numbers of the last repository & task inserted.

    repo_counter_ = wf.last_repo_id;
    task_counter_ = wf.last_task_id;

    //
    // Now, we must build a map that will help us to
    // find the definition of a task using its
    // identifier.
    //

    map<string, int> task_id_to_tk_pos;

    task_id_to_tk_pos.clear();

    for (size_t i = 0; i < tk_.tasks.size(); i++)
      task_id_to_tk_pos.insert(std::make_pair(tk_.tasks[i].id, i));

    //
    // Now, we'll create the whole set of tasks (no repositories yet)
    // included in the workflow.
    //

    map<int, DiagramItem*> num_id_to_item;
    num_id_to_item.clear();

    map<string, int>::iterator task_finder;

    for (size_t i = 0; i < wf.tasks.size(); i++)
    {
      QPointF      pos;
      DiagramItem* tsk;
      int          tsk_idx;
      WFNode       wtsk;

      // Get the current task in the workflow.

      wtsk = wf.tasks[i];

      //
      // Locate the task definition (it is held by the toolkit) using
      // its identifier.
      //

      task_finder = task_id_to_tk_pos.find(wtsk.id);
      tsk_idx = task_finder->second;

      // The position in the scene where the task must be inserted.

      pos = QPointF(wtsk.pos.x, wtsk.pos.y);

      // Create the new DiagramItem (a task, in this case).

      tsk = new DiagramItem(DiagramItem::DiagramType::Task, tk_.tasks[tsk_idx], item_menu_);
      tsk->set_seq_id(wtsk.sequential_id);
      tsk->setBrush(Qt::white);
      addItem(tsk);
      tsk->setPos(pos);

      //
      // Keep track of its numerical identifier, to be able to link the
      // connnections later on.
      //

      num_id_to_item.insert(std::make_pair(wtsk.numerical_id, tsk));
    }

    //
    // It's time to insert the repositories.
    //

    for (size_t i = 0; i < wf.repos.size(); i++)
    {
      QString      id;
      QPointF      pos;
      DiagramItem* repo;
      WFNode       wrepo;

      // Get the current repo in the workflow.

      wrepo = wf.repos[i];

      // The position in the scene where the task must be inserted.

      pos = QPointF(wrepo.pos.x, wrepo.pos.y);

      // The name of the repository.

      id = QString::fromStdString(wrepo.id);

      // Create the new DiagramItem (a repository, in this case).

      repo = new DiagramItem(DiagramItem::DiagramType::Repository, tk_.tasks[0], item_menu_);
      repo->setBrush(Qt::white);
      repo->set_id(id);
      addItem(repo);
      repo->setPos(pos);

      //
      // Keep track of its numerical identifier, to be able to link the
      // connnections later on.
      //

      num_id_to_item.insert(std::make_pair(wrepo.numerical_id, repo));
    }

    //
    // And to finish, we'll insert the connections. We'll use
    // the map "num_id_to_item" to identify the endpoints in
    // these connections.
    //

    for (size_t i = 0; i < wf.connections.size(); i++)
    {
      DiagramItem*                     endItem;
      subnode_data                     end_subnode;
      map<int, DiagramItem*>::iterator item_finder;
      DiagramItem*                     startItem;
      subnode_data                     start_subnode;
      WFConnection                     wconn;

      // Get the current connection.

      wconn = wf.connections[i];

      // Get the two DiagramItem endpoints using our map.

      item_finder = num_id_to_item.find(wconn.from.endpoint_id);
      startItem = item_finder->second;

      item_finder = num_id_to_item.find(wconn.to.endpoint_id);
      endItem = item_finder->second;

      //
      // Set the subnode information (information about the precise
      // position where the connections link with the items, such
      // as input / output area and slot).
      //

      start_subnode.item     = startItem;
      start_subnode.mode     = OutputConnection;
      start_subnode.position = wconn.from.slot;

      end_subnode.item       = endItem;
      end_subnode.mode       = InputConnection;
      end_subnode.position   = wconn.to.slot;

      // Create the connection and insert it to the scene.

      connection *conn = new connection(start_subnode, end_subnode);
      conn->setColor(Qt::black);
      startItem->add_connection(conn);
      endItem->add_connection(conn);
      addItem(conn);
      conn->updatePosition();
    }

    // That's all.

    return true;
  }
}

void
DiagramScene::
mouseMoveEvent
(QGraphicsSceneMouseEvent *mouseEvent)
{
  {
    // Work only when we've got a toolkit

    if (!got_toolkit_) return;

    if (mode_ == InsertLine && line_ != nullptr)
    {
      QLineF newLine(line_->line().p1(), mouseEvent->scenePos());
      line_->setLine(newLine);
    }
    else if (mode_ == MoveItem)
    {
      QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
  }
}

void
DiagramScene::
mousePressEvent
(QGraphicsSceneMouseEvent *mouseEvent)
{
  {
    DiagramItem* existing_item = nullptr;
    QPointF      clicked;

    // Work only when we've got a toolkit.

    if (!got_toolkit_) return;

    // React only when the LEFT mouse button is clicked.

    if (mouseEvent->button() != Qt::LeftButton) return;

    // Let's proceed according to the work mode.

    switch (mode_)
    {
      case InsertItem:

        //
        // Insert a new item.
        //

        DiagramItem *item;

        item = new DiagramItem(item_type_, tk_.tasks[current_task_], item_menu_);
        item->setBrush(Qt::white);
        addItem(item);
        item->setPos(mouseEvent->scenePos());

        if (item_type_ == DiagramItem::Repository)
        {
          QString repo_name;
          repo_counter_++;
          repo_name = "REPO_" + QString::number(repo_counter_);
          item->set_id(repo_name);
        }
        else
        {
          // Task.

          task_counter_++;
          item->set_seq_id(task_counter_);
        }

        emit itemInserted(item);

        break;

      case InsertLine:

        //
        // Insert a new connection.
        //

        //
        // First of all, find wheter we've clicked over an existing item. If not,
        // there's nothing to do.
        //

        subnode_data subnode_item;

        existing_item = qgraphicsitem_cast<DiagramItem *>(itemAt(mouseEvent->scenePos(), QTransform()));
        if (existing_item == nullptr) break;
        if (existing_item->type() != DiagramItem::Type) break;

        clicked = existing_item->mapFromScene(mouseEvent->scenePos());

        if (existing_item->validate_click(OutputConnection, clicked, subnode_item))
        {
          //
          // At this point we know that we've clicked in one of the output circles
          // of some task.
          //

          if (existing_item->subnode_coordinates(subnode_item, clicked))
          {

            //
            // And we've been able to retrieve the information related to the circle
            // where the user has clicked.
            //

            //
            // Get the list of task that have an input slot (circle) compatible with
            // the kind of output slot where the user clicked. Of course, we must
            // exclude the task where the click took place.
            //

            QVector<DiagramItem*> candidates;
            build_candidate_list(existing_item, subnode_item, candidates);

            if (existing_item->is_task())
            {
              //
              // The node starting the connection is a task. We need at least
              // a candidate to connect to, no matter whether it's a task
              // or a repository.
              //

              int task_candidates = candidates.count();

              // Total number of available repositories.

              int total_repos = count_repositories();

              // Total number of candidates.

              int total_candidates = total_repos + task_candidates;

              // No candidates? No line!

              if (total_candidates == 0) break;

              //
              // There are candidates. But, if there are no task candidates we don't
              // need to worry about possible cycles...
              //

              if (task_candidates != 0)
              {
                string filetype = existing_item->file_type_at(OutputConnection, subnode_item.position);

                //
                // There are tasks willing to accept a connection. We'll check that adding a new
                // edge to the graph no cycles will be created.
                //

                build_task_graph();

                if (task_graph_ == nullptr)
                {
                  //
                  // No graph means no previous connections at all. This implies
                  // that no cycles will show up when adding a new connection...
                  // We'll therefore highlight all the acceptable input files
                  // in ALL the task we've just found.
                  //

                  highlight_candidates (filetype, candidates);
                }
                else
                {
                  //
                  // There is a graph (that is, there were previous connections).
                  // We must check all the possible new edges in order to find if
                  // their addition produces cycles. If adding an edge to any of these
                  // candidates creates a cycle, the candidate will be removed
                  // from the list.
                  //

                  QVector<DiagramItem*> final_candidates;

                  check_for_cycles (existing_item, candidates, final_candidates);

                  // There are any remaining candidates?

                  int total_final_candidates = final_candidates.count();

                  if (total_final_candidates != 0)
                  {
                    //
                    // Yes! There is, at least, one task willing to accept our connection!
                    // Highlight the candidates!
                    //

                    highlight_candidates (filetype, final_candidates);
                  }
                  else
                  {
                    // If there are no task and no repository canditates, we must leave.

                    if (total_repos == 0) break;
                  }
                }
              }
            }
            else
            {
              // The item is a repository.

              //
              // Tell all the candidates to mark as available ALL their free
              // slots (connections from a repository are always accepted).
              //

              // There must be at least one candidate to connect to!

              if (candidates.count() == 0) break;

              // Highlight the candidates!

              string filetype = "";
              highlight_candidates (filetype, candidates);
            }

            start_point_   =  existing_item->mapToScene(clicked);
            start_subnode_ = subnode_item;

            line_ = new QGraphicsLineItem(QLineF(start_point_, start_point_));
            line_->setPen(QPen(Qt::black, 2));

            addItem(line_);
          }
        }

        break;

    default:
        ;
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
  }
}

void
DiagramScene::
mouseReleaseEvent
(QGraphicsSceneMouseEvent *mouseEvent)
{
  {
    if (line_ != nullptr && mode_ == InsertLine && got_toolkit_)
    {
      removeItem(line_);
      delete line_;
      line_ = nullptr;

      DiagramItem* startItem = start_subnode_.item;
      DiagramItem* endItem   = qgraphicsitem_cast<DiagramItem *>(itemAt(mouseEvent->scenePos(), QTransform()));

      if (endItem != nullptr)
      {
        // We have a non-null endpoint!

        if ((endItem->type() == DiagramItem::Type) && (startItem != endItem))
        {
          //
          // We will ignore lines as end items; moreover, the end item may
          // not be equal to the start item.
          //

          if (!((startItem->is_repository()) && (endItem->is_repository())))
          {
            // We do not accept direct connections between repositories.

            QPointF      clicked;
            subnode_data subnode_item;

            clicked = endItem->mapFromScene(mouseEvent->scenePos());

            if (endItem->validate_click(InputConnection, clicked, subnode_item))
            {

              if (endItem->subnode_coordinates(subnode_item, clicked))
              {
                end_point_   = endItem->mapToScene(clicked);
                end_subnode_ = subnode_item;

                connection *conn = new connection(start_subnode_, end_subnode_);
                conn->setColor(Qt::black);
                startItem->add_connection(conn);
                endItem->add_connection(conn);
                addItem(conn);
                conn->updatePosition();
              }
            }
          }
        }
      }

      reset_highlights();
    }

    line_ = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
  }
}

void
DiagramScene::
reset_highlights
(void)
{
  {
    QList<QGraphicsItem *> the_items;

    //
    // Get all the items in the scene. Then, reset the highlighting
    // off all items of type task.
    //

    the_items = items();

    for (int i = 0; i < the_items.count(); i++)
    {
      QGraphicsItem* gi = the_items.at(i);

      if (gi->type() != connection::Type)
      {
        DiagramItem* it = qgraphicsitem_cast<DiagramItem *>(gi);

        if (it->is_task())
        {
          it->reset_highlights();
        }
      }
    }
  }
}

void
DiagramScene::
setCurrentTask
(int task_number)
{
  {
    current_task_ = task_number;
  }
}

void
DiagramScene::
setItemType
(DiagramItem::DiagramType type)
{
  {
    item_type_ = type;
  }
}

void
DiagramScene::
setMode
(Mode mode)
{
  {
    mode_ = mode;
  }
}

void
DiagramScene::
setToolkit
(toolkit& tk)
{
  {
    tk_ = tk;
    got_toolkit_ = true;
  }
}
