/** \file launcher_structures.hpp
  \brief Defines several structures for the use of WorkflowLauncher.
*/

#ifndef WFMEDITOR_STRUCTURES_HPP
#define WFMEDITOR_STRUCTURES_HPP

#include "workflow_structures.hpp"

#include <string>
#include <vector>

using namespace std;

/// \brief Unique connection identifier.

struct WLConnKey
{
    int num_id; /**< Numerical of the task from which the connection starts. */

    int slot;   /**< Slot number from which the connection starts. */

    bool operator<(const WLConnKey& rhs) const
      {
        {
          // First, we sort using the numerical identifier.

          if (num_id < rhs.num_id) return true;
          if (num_id > rhs.num_id) return false;

          //
          // At this point, the task identifier. Let's decide
          // using the slot number.
          //

          if (slot   < rhs.slot)   return true;
          if (slot   > rhs.slot)   return false;

          //
          // All components in both structs are equal. This means
          // that the first one is NOT less than the second one.
          //

          return false;
        }
      }
};

/// \brief Unique task identifier.

struct WLTaskKey
{
  string task_id;    /**< Task's string identifier. */

  int    task_seqid; /**< Task's sequential identifier. */

  bool operator<(const WLTaskKey& rhs) const
    {
      {
        // First, we sort using the task identifier.

        if (task_id    < rhs.task_id)    return true;
        if (task_id    > rhs.task_id)    return false;

        //
        // At this point, the task identifier. Let's decide
        // using the task sequential id.
        //

        if (task_seqid < rhs.task_seqid) return true;
        if (task_seqid > rhs.task_seqid) return false;

        //
        // All components in both structs are equal. This means
        // that the first one is NOT less than the second one.
        //

        return false;
      }
    }
};

/**
  \brief Defines a file as used by the WLLauncher.
 */


struct WLFile
{
  ConnectionType start_node_type;       /**< Type (repository, task) of the
                                             initial endpoint of the connection
                                             in which this file is involved. */

  string         start_node_id;         /**< String identifier of the initial
                                             endpoint of the connection in which
                                             this file is involved. */

  int            start_node_nid;        /**< Numerical identifier of the initial
                                             endpoint of the connection in which
                                             this file is involved. */

  int            start_node_seqid;      /**< Numerical, sequential identifier, that
                                             combined with start_node_id provides
                                             with an unique identification of the
                                             initial enpoint when it is a task. */

  ConnectionType end_node_type;         /**< Type (repository, task) of the
                                             final endpoint of the connection
                                             in which this file is involved. */

  int            start_node_slot;       /**< Task or repository slot where
                                             the initial connection starts. */

  string         end_node_id;           /**< String identifier of the final
                                             endpoint of the connection in which
                                             this file is involved. */

  int            end_node_nid;          /**< Numerical identifier of the final
                                             endpoint of the connection in which
                                             this file is involved. */

  int            end_node_seqid;        /**< Numerical, sequential identifier, that
                                             combined with start_node_id provides
                                             with an unique identification of the
                                             final enpoint when it is a task. */

  int            end_node_slot;         /**< Task or repository slot where
                                             the final connection starts. */

  string         file_type_id;          /**< Kind of file. Use to tell apart,
                                             for instance, text, image or audio
                                             files. See toolkit_file_type for
                                             details. */

  string         file_type_description; /**< Description of the general /
                                             purpose of this type of file. */

  string         description;           /**< Specific description of the purpose
                                             of this file in this specific place.*/

  string         extension;             /**< Default extension for this file. */

  string         start_node_opf_label;  /**< Label that a task will look for
                                             in an options file to read the name
                                             of the file itself. For the start node.*/

  string         end_node_opf_label;    /**< Label that a task will look for
                                             in an options file to read the name
                                             of the file itself. For the end node.*/

  string         filename;              /**< Actual file name for this file. */

  bool           erasable;              /**< Flag stating whether the file may be
                                             erased once it needed no more. */

  int            delete_after_task;     /**< Number of the task (in the topological
                                             sorted sequence of tasks) after which
                                             this file may be safely deleted. Valid
                                             only when the attribute erasable is
                                             set to true. */
};

/// \brief Identifier for files, based on the numerical id of the
///        task or repo and slot to which these are connected.
///        Mainly used to sort maps.

struct WLFile_id
{
    int numid; /**< Numerical id of the task or repo where the
                    file is produced or read. */

    int slot;  /**< Task or repository slot number. */

    bool operator<(const WLFile_id& rhs) const
    {
      {
        // First, we sort using the numerical identifier.

        if (numid < rhs.numid) return true;
        if (numid > rhs.numid) return false;

        //
        // At this point, the numerical identifiers are the
        // same. Then, our decision will be based on the slot
        // numbers.
        //

        if (slot  < rhs.slot)  return true;
        if (slot  > rhs.slot)  return false;

        //
        // All components in both structs are equal. This means
        // that the first one is NOT less than the second one.
        //

        return false;
      }
    }
};



/**
  \brief Defines a task's parameter
 */

struct WLParameter
{
  string id;          /**< Unique identifier used to tell
                           apart several parameters.
                           It should be descriptive enough to
                           let users ascertain its purpose. */

  string description; /**< Describes in detail the meaning of
                           the parameter. */

  string type;        /**< Kind (integer, real, boolean,
                           string) of data that the parameter
                           will hold. */

  string value;       /**< Value assigned to the parameter. */

  string task_id;     /**< String identifier of the task to which
                           the parameter is related. */

  string task_seqid;  /**< Sequential, unique identifier of the
                           task to which the parameter is related. */
};

/**
  \brief Defines a repository.
 */

struct WLRepository
{
    string id;           /**< String identifier of repository. */

    int    nid;          /**< Numerical id of the repository. */

    string path;         /**< Actual path to the repository. */

};

/**
 \brief Defines the structure including all the information
        needed to launch a workflow.
 */

struct WLLauncher
{
  int    wfm_version;                /**< Version of the workflow loaded.*/

  string               id;           /**< Identifier for this particular
                                          launcher. */

  string               description;  /**< Description for this particular
                                          launcher. */

  string               workflow_id;  /**< Identifier of the workflow on
                                          which the launcher relies. */

  vector<WLRepository> repositories; /**< The list of repositories
                                          included in the workflow. */

  vector<WLFile>       files;        /**< The list of files included
                                          in the workflow. */

  vector<WLParameter>  parameters;   /**< The list of parameters
                                          used by the workflow. */
};


#endif // WFMEDITOR_STRUCTURES_HPP
