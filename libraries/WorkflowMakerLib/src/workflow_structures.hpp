/** \file workflow_structures.hpp
  \brief Defines some structures required by WorkFlowEditor.
*/

#ifndef WORKFLOW_STRUCTURES_HPP
#define WORKFLOW_STRUCTURES_HPP

#include <vector>
#include <string>

using namespace std;

enum ConnectionType {RepoEndPoint, TaskEndPoint, WasteBasketEndPoint};

/**
  \brief Coordinates (in the scene space) of the center of a task or repository.
 */

struct WFCoordinate
{
    int x; /**< X scene coordinate. */

    int y; /**< Y scene coordinate. */
};

/**
  \brief Defines a connection endpoint.
 */

struct WFEndPoint
{
  ConnectionType type;        /**< Type of endpoint (Task, Repository). */

  int            endpoint_id; /**< Numerical identifier of the endpoint. */

  int            slot       ; /**< Ordinal stating the input / output slot. */
};

/**
  \brief Defines a single connection.
 */

struct WFConnection
{
    WFEndPoint from; /**< Definition of the point where the connection starts. */

    WFEndPoint to;   /**< Definition of the point where the conection ends. */

    bool operator<(const WFConnection& rhs) const
    {
      {
        // First, we sort using the type of endpoint.

        if (from.type        < rhs.from.type)        return true;
        if (from.type        > rhs.from.type)        return false;

        if (to.type          < rhs.to.type)          return true;
        if (to.type          > rhs.to.type)          return false;

        //
        // At this point, all types are equal. Then, our decision
        // will be based on the endpoint's ids.
        //

        if (from.endpoint_id < rhs.from.endpoint_id) return true;
        if (from.endpoint_id > rhs.from.endpoint_id) return false;

        if (to.endpoint_id   < rhs.to.endpoint_id)   return true;
        if (to.endpoint_id   > rhs.to.endpoint_id)   return false;

        //
        // At this point, all endpoints id are equal. We'll then
        // rely on the connection's slots.
        //

        if (from.slot        < rhs.from.slot)        return true;
        if (from.slot        > rhs.from.slot)        return false;

        if (to.slot          < rhs.to.slot)          return true;
        if (to.slot          > rhs.to.slot)          return false;

        //
        // All components in both structs are equal. This means
        // that the first one is NOT less than the second one.
        //

        return false;
      }
    }
};

/**
  \brief Defines a task or repository.
 */

struct WFNode
{
    string       id;            /**< String (not unique) identifier of the task or repository. */

    int          numerical_id;  /**< Numerical (unique) identifier of the task or repository.
                                     Used for graph sorting, cycle detection... */

    int          sequential_id; /**< Unique numerical identifier used to complement id and
                                     make it unique. For instance, if id is "TASK" and this
                                     sequential id is 23, then the "virtual" identifier of
                                     this node would be "TASK (23)". */

    WFCoordinate pos;           /**< Coordinates (in the scene space) of the center of the
                                     task or repository. */
};

/**
  \brief Defines a complete workflow.
 */

struct WFWorkflow
{
    int    wfm_version;               /**< Version of the workflow loaded.*/

    string id;                        /**< String identifier of the workflow. */

    string description;               /**< Description (preferably short) of the workflow. */

    string toolkit_id;                /**< Identifier of the toolkit on which the workflow relies. */

    int    last_repo_id;              /**< Last used numerical identifier used by the workflow editor
                                           to guarantee that no two repos have the same string id. */

    int    last_task_id;              /**< Last used numerical (sequential) identifier used by the
                                           workflow editor to guarantee that no two tasks with the
                                           same string id may be confused. The addition of this
                                           number will serve to tell apart these. */

    vector<WFNode> repos;             /**< The repositories included in the workflow. */

    vector<WFNode> tasks;             /**< The tasks included in the workflow. */

    vector<WFConnection> connections; /** The connections between tasks / repositories. */
};


#endif // WORKFLOW_STRUCTURES_HPP
