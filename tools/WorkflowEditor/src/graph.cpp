/** \file graph.cpp
  \brief Implementation file for graph.hpp.
*/

#include "graph.hpp"

void
Graph::
add_edge
(int v,
 int w)
{
  {
    adjacencies_[v].push_back(w); // Add w to v’s list.
    last_vertex_ = v;
  }
}

Graph::
Graph
(int V)
{
  {
    V_ = V;
    adjacencies_ = new list<int>[V];
    last_vertex_ = -1;
  }
}

Graph::
~Graph
(void)
{
  {
    for (int i = 0; i < V_; i++)
      adjacencies_[i].clear();

    delete[] adjacencies_;
    adjacencies_ = nullptr;
    adjacencies_ = new list<int>[V_];
  }
}

bool
Graph::
is_cyclic
(void)
{
  {
    //
    // Mark all the vertices as not visited and not part of recursion
    // stack
    //

    bool* visited  = new bool[V_];
    bool* recStack = new bool[V_];

    for (int i = 0; i < V_; i++)
    {
      visited[i]  = false;
      recStack[i] = false;
    }

    //
    // Call the recursive helper function to detect cycle in different
    // DFS trees
    //

    for (int i = 0; i < V_; i++)
      if (!visited[i] && is_cyclic_util(i, visited, recStack))
      {
        delete[] visited;
        delete[] recStack;
        visited  = nullptr;
        recStack = nullptr;

        return true;
      }

    delete[] visited;
    delete[] recStack;
    visited  = nullptr;
    recStack = nullptr;

    return false;
  }
}


bool
Graph::
is_cyclic_util
(int  v,
 bool visited[],
 bool* recStack)
{
  {
    if (visited[v] == false)
    {
      // Mark the current node as visited and part of recursion stack

      visited[v]  = true;
      recStack[v] = true;

      // Recur for all the vertices adjacent to this vertex

      list<int>::iterator i;

      for (i = adjacencies_[v].begin(); i != adjacencies_[v].end(); ++i)
      {
        if (!visited[*i] && is_cyclic_util(*i, visited, recStack))
          return true;
        else if (recStack[*i])
          return true;
      }
    }

    recStack[v] = false;  // remove the vertex from recursion stack
    return false;
  }
}

void
Graph::
remove_last_edge
(void)
{
  {
    if (last_vertex_ == -1) return;

    adjacencies_[last_vertex_].pop_back();
  }
}

void
Graph::
topological_sort
(vector<int>& sorted_graph)
{
  {
    stack<int> the_stack;

    // Mark all the vertices as not visited

    bool* visited = new bool[V_];

    for (int i = 0; i < V_; i++)
      visited[i] = false;

    //
    // Call the recursive helper function to store the
    // topological sort starting from all vertices one by one.
    //

    for (int i = 0; i < V_; i++)
      if (visited[i] == false)
        topological_sort_util(i, visited, the_stack);

    // Copy the stack to our output vector.

    while (the_stack.empty() == false)
    {
      sorted_graph.push_back(the_stack.top());
      the_stack.pop();
    }

    // Remove garbage.

    delete [] visited;
    visited = nullptr;
  }
}

void
Graph::
topological_sort_util
(int         v,
 bool        visited[],
 stack<int>& the_stack)
{
  {
    // Mark the current node as visited.

    visited[v] = true;

    // Recur for all the vertices adjacent to this vertex.

    list<int>::iterator i;
    for (i = adjacencies_[v].begin(); i != adjacencies_[v].end(); ++i)
      if (!visited[*i])
        topological_sort_util(*i, visited, the_stack);

    // Push current vertex to the stack storing the result.

    the_stack.push(v);
  }
}
