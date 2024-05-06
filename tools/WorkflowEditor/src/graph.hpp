/** \file graph.hpp
  \brief Minimalistic implementation of a directed graph.
*/

#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <list>
#include <stack>
#include <vector>

using namespace std;

/// \brief Minimalistic implementation of a directed graph.

class Graph
{
  public:

    /// \brief Add an edge to the graph.
    /**
      \param[in] v Starting node.
      \param[in] w Ending node.
     */

    void add_edge              (int v, int w);

    /// \brief Constructor.
    /**
      \param[in] V Number of vertices.
     */
         Graph                 (int V);

    /// \brief Destructor.
    
         ~Graph                (void);
    
    /// \brief Check if the graph contains cycles.
    /**
      \return true if the graph contains cycles, false otherwise.
     */

    bool is_cyclic             (void);

    /// \brief Remove the last added edge.

    void remove_last_edge      (void);

    /// \brief Sorts the graph topologically.
    /**
      \param[out] sorted_graph The list of nodes, sorted acording
                  to its preferences.
     */

    void topological_sort      (vector<int>& sorted_graph);

  protected:

    /// \brief Auxiliary function used by isCyclic()
    /**
      \param[in] v The vertex to start the search from.
      \param[in,out] visited List of already visited nodes.
      \param[in,out] rs Auxiliary stack.
      \return True if the branch tested is cyclic, false otherwise.
     */

    bool is_cyclic_util        (int v, bool visited[], bool *rs);

    /// \brief Auxiliary, recursive function used by topological_sort.
    /**
      \param v The vertex to start the sort from.
      \param[in,out] visited List of already visited nodes.
      \param[in,out] the_stack Auxiliary stack.
     */

    void topological_sort_util (int v, bool visited[], stack<int>& the_stack);

  protected:

    /// \brief Pointer to an array containing adjacency lists.

    list<int>* adjacencies_;

    /// \brief Last vertex to which an edge has been added.

    int        last_vertex_;

    /// \brief Number of vertices.

    int        V_;
};
#endif // GRAPH.HPP
