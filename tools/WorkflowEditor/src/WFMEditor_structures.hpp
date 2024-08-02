/** \file WFMEditor_structures.hpp
  \brief Defines several structures and enumerations for the use of the
         WorkflowEditor.
*/


#ifndef WFMEDITOR_STRUCTURES_HPP
#define WFMEDITOR_STRUCTURES_HPP

#include <QPointF>
#include "toolkit_structures.hpp"
#include <vector>

using namespace std;

/// Types of connections.

enum ConnectionMode
{
  InputConnection, ///< Input connection. A file is read.
  OutputConnection ///< Output connection. A file is written.
};

/// Status of a connection slot.

enum ConnectionStatus
{
  Disconnected, ///< The slot has not been connected yet.
  Candidate,    ///< The slot is willing to accept connections for some kind of file type.
  Connected     ///< The slot is already connected.
};

/// Status of a task, concerning its inputs and outputs.
enum TaskStatus
{
  MissingInputs,      ///< At least one of the task's input files has not been connected.
  MissingAllOutputs,  ///< None of the task's output files has been connected.
  MissingSomeOutputs, ///< At least one of the task's output files has not been connected.
  Complete            ///< All files (input and output) have been connected.
};


class DiagramItem;

/**
  \brief Defines the dimensions of a node.
 */

struct wfme_node_dimensions
{
  qreal height;       /**< Height of the outer box to draw. Item units. */

  qreal width;        /**< Width of the outer box to draw. Item units. */

  qreal h_strip_up;   /**< Height of the upper strip in task nodes. Item units. */

  qreal h_strip_mid;  /**< Height of the middle strip in task nodes. Item units. */

  qreal h_strip_down; /**< Height of the lower strip in task nodes. Item units. */

  qreal radius;       /**< Radius of the bullets representing input & output files.
                           Item units. */
};

/**
  \brief Defines the positions of the several elements making a node.
 */

struct wfme_node_positions
{
  QPointF         center;       /**< Coordinates (scene's coordinate space) of
                                     the center of the node. */

  vector<QPointF> input_files;  /**< Coordinates (in the item's coordinate space)
                                     of the centers of the bullets representing
                                     the input files.*/

  vector<QPointF> output_files; /**< Coordinates (in the item's coordinate space)
                                     of the centers of the bullets representing
                                     the output files.*/
};

/**
  \brief Node (step / repository) that may be inserted in a workflow.

  It is based on the toolkit_task structure defined in
  toolkit_structures.hpp.

 */

struct wfme_node_data
{
  string                   id;                 /**< Unique identifier for the node. */

  int                      type;               /**< Tells apart the kind of node. Their
                                                    values are defined in the respective
                                                    node classes. */

  string                   description;        /**< Detailed description, stating what
                                                    is the purpose of the task. */

  string                   name_of_executable; /**< Name of the executable file implementing
                                                    the task. Add no ".exe" extension to
                                                    this name. Meaningful for task nodes only.*/

  vector<string>           parameters;         /**< List of parameter identifiers .*/

  vector<toolkit_file>     input_files;        /**< List of input files (list of
                                                    toolkit_file structures). */

  vector<ConnectionStatus> input_status;       /**< Flags stating whether the input files
                                                    have already been assigned. */

  vector<toolkit_file>     output_files;       /**< List of output files (list of
                                                    toolkit_file structures). */

  vector<int>              output_connections; /**< Connection counters for the output files. */

  wfme_node_dimensions     dim;                /**< Dimensions of the several elements making
                                                    the node. */

  wfme_node_positions      pos;                /**< Positions of the node and its elements. */

};

/**
  \brief Identifies a subnode inside a node
 */

struct wfme_subnode_data
{
  DiagramItem*     item;     /**< Pointer to the diagram item to which this info refers. */

  int              mode;     /**< Input (0) or output (1) subnode. */

  int              position; /**< Position (starting at 0) of the input or output subnode selected. */
};


#endif // WFMEDITOR_STRUCTURES_HPP
