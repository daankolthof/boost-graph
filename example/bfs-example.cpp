//=======================================================================
// Copyright 2001 Jeremy G. Siek, Andrew Lumsdaine, Lie-Quan Lee, 
//
// This file is part of the Boost Graph Library
//
// You should have received a copy of the License Agreement for the
// Boost Graph Library along with the software; see the file LICENSE.
// If not, contact Office of Research, Indiana University,
// Bloomington, IN 47405.
//
// Permission to modify the code and to distribute the code is
// granted, provided the text of this NOTICE is retained, a notice if
// the code was modified is included with the above COPYRIGHT NOTICE
// and with the COPYRIGHT NOTICE in the LICENSE file, and that the
// LICENSE file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//=======================================================================
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/pending/integer_range.hpp>
using namespace boost;
template < typename TimeMap > class bfs_time_visitor:public default_bfs_visitor {
  typedef typename property_traits < TimeMap >::value_type T;
public:
bfs_time_visitor(TimeMap tmap, T & t):m_timemap(tmap), m_time(t) {
  }
  template < typename Vertex, typename Graph >
    void discover_vertex(Vertex u, const Graph & g) const
  {
    put(m_timemap, u, m_time++);
  }
  TimeMap m_timemap;
  T & m_time;
};


int
main()
{
  using namespace boost;
  // Select the graph type we wish to use
  typedef adjacency_list < vecS, vecS, undirectedS > graph_t;
  // Set up the vertex IDs and names
  enum
  { r, s, t, u, v, w, x, y, N };
  const char *name = "rstuvwxy";
  // Specify the edges in the graph
  typedef std::pair < int, int >E;
  E edge_array[] = { E(r, s), E(r, v), E(s, w), E(w, r), E(w, t),
    E(w, x), E(x, t), E(t, u), E(x, y), E(u, y)
  };
  // Create the graph object
  const int n_edges = sizeof(edge_array) / sizeof(E);
  graph_t g(edge_array, edge_array + n_edges, N);

  // Typedefs
  typedef graph_traits < graph_t >::vertex_descriptor Vertex;
  typedef graph_traits < graph_t >::vertices_size_type Size;
  typedef std::vector < Vertex >::iterator Piter;
  typedef std::vector < Size >::iterator Iiter;

  // a vector to hold the discover time property for each vertex
  std::vector < Size > dtime(num_vertices(g));

  Size time = 0;
  bfs_time_visitor < Size * >vis(&dtime[0], time);
  breadth_first_search(g, vertex(s, g), visitor(vis));

  // Use std::sort to order the vertices by their discover time
  std::vector<graph_traits<graph_t>::vertices_size_type > discover_order(N);
  integer_range < int >range(0, N);
  std::copy(range.begin(), range.end(), discover_order.begin());
  std::sort(discover_order.begin(), discover_order.end(),
            indirect_cmp < Iiter, std::less < Size > >(dtime.begin()));

  std::cout << "order of discovery: ";
  for (int i = 0; i < N; ++i)
    std::cout << name[discover_order[i]] << " ";
  std::cout << std::endl;
  return EXIT_SUCCESS;
}