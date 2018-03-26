/*
 * graphtheory.h
 *
 * (c) 2018 Luka Marohnić
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GRAPHTHEORY_H
#define GRAPHTHEORY_H
#include "graphe.h"

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

bool is_graphe(const gen &g,std::string &disp_out,GIAC_CONTEXT);

gen _graph(const gen &g,GIAC_CONTEXT);
gen _digraph(const gen &g,GIAC_CONTEXT);
gen _export_graph(const gen &g,GIAC_CONTEXT);
gen _import_graph(const gen &g,GIAC_CONTEXT);
gen _draw_graph(const gen &g,GIAC_CONTEXT);
gen _graph_complement(const gen &g,GIAC_CONTEXT);
gen _induced_subgraph(const gen &g,GIAC_CONTEXT);
gen _make_directed(const gen &g,GIAC_CONTEXT);
gen _seidel_switch(const gen &g,GIAC_CONTEXT);
gen _complete_graph(const gen &g,GIAC_CONTEXT);
gen _make_weighted(const gen &g,GIAC_CONTEXT);
gen _subgraph(const gen &g,GIAC_CONTEXT);
gen _underlying_graph(const gen &g,GIAC_CONTEXT);
gen _cycle_graph(const gen &g,GIAC_CONTEXT);
gen _lcf_graph(const gen &g,GIAC_CONTEXT);
gen _add_arc(const gen &g,GIAC_CONTEXT);
gen _add_edge(const gen &g,GIAC_CONTEXT);
gen _add_vertex(const gen &g,GIAC_CONTEXT);
gen _delete_arc(const gen &g,GIAC_CONTEXT);
gen _delete_edge(const gen &g,GIAC_CONTEXT);
gen _delete_vertex(const gen &g,GIAC_CONTEXT);
gen _contract_edge(const gen &g,GIAC_CONTEXT);
gen _set_graph_attribute(const gen &g,GIAC_CONTEXT);
gen _get_graph_attribute(const gen &g,GIAC_CONTEXT);
gen _discard_graph_attribute(const gen &g,GIAC_CONTEXT);
gen _list_graph_attributes(const gen &g,GIAC_CONTEXT);
gen _adjacency_matrix(const gen &g,GIAC_CONTEXT);
gen _incidence_matrix(const gen &g,GIAC_CONTEXT);
gen _biconnected_components(const gen &g,GIAC_CONTEXT);
gen _connected_components(const gen &g,GIAC_CONTEXT);
gen _departures(const gen &g,GIAC_CONTEXT);
gen _incident_edges(const gen &g,GIAC_CONTEXT);
gen _is_connected(const gen &g,GIAC_CONTEXT);
gen _maximum_matching(const gen &g,GIAC_CONTEXT);
gen _number_of_edges(const gen &g,GIAC_CONTEXT);
gen _edges(const gen &g,GIAC_CONTEXT);
gen _has_arc(const gen &g,GIAC_CONTEXT);
gen _arrivals(const gen &g,GIAC_CONTEXT);
gen _articulation_points(const gen &g,GIAC_CONTEXT);
gen _vertex_in_degree(const gen &g,GIAC_CONTEXT);
gen _vertex_out_degree(const gen &g,GIAC_CONTEXT);
gen _vertex_degree(const gen &g,GIAC_CONTEXT);
gen _get_edge_weight(const gen &g,GIAC_CONTEXT);
gen _set_edge_weight(const gen &g,GIAC_CONTEXT);
gen _has_edge(const gen &g,GIAC_CONTEXT);
gen _is_directed(const gen &g,GIAC_CONTEXT);
gen _minimum_degree(const gen &g,GIAC_CONTEXT);
gen _maximum_degree(const gen &g,GIAC_CONTEXT);
gen _is_regular(const gen &g,GIAC_CONTEXT);
gen _maximal_independent_set(const gen &g,GIAC_CONTEXT);
gen _neighbors(const gen &g,GIAC_CONTEXT);
gen _number_of_vertices(const gen &g,GIAC_CONTEXT);
gen _vertices(const gen &g,GIAC_CONTEXT);
gen _weight_matrix(const gen &g,GIAC_CONTEXT);
gen _hypercube_graph(const gen &g,GIAC_CONTEXT);
gen _sierpinski_graph(const gen &g,GIAC_CONTEXT);
gen _petersen_graph(const gen &g,GIAC_CONTEXT);
gen _random_tree(const gen &g,GIAC_CONTEXT);
//gen _permute_vertices(const gen &g,GIAC_CONTEXT);
//gen _relabel_vertices(const gen &g,GIAC_CONTEXT);
//gen _disjoint_union(const gen &g,GIAC_CONTEXT);
//gen _graph_join(const gen &g,GIAC_CONTEXT);
//gen _interval_graph(const gen &g,GIAC_CONTEXT);
//gen _plane_dual(const gen &g,GIAC_CONTEXT);
//gen _transitive_closure(const gen &g,GIAC_CONTEXT);
//gen _fundamental_cycle(const gen &g,GIAC_CONTEXT);
//gen _graph_power(const gen &g,GIAC_CONTEXT);
//gen _mycielski(const gen &g,GIAC_CONTEXT);
//gen _graph_union(const gen &g,GIAC_CONTEXT);
//gen _reverse_graph(const gen &g,GIAC_CONTEXT);
//gen _subdivide(const gen &g,GIAC_CONTEXT);
//gen _greedy_color(const gen &g,GIAC_CONTEXT);
//gen _chromatic_index(const gen &g,GIAC_CONTEXT);
//gen _circular_edge_chromatic_number(const gen &g,GIAC_CONTEXT);
//gen _edge_connectivity(const gen &g,GIAC_CONTEXT);
//gen _global_clustering_coefficient(const gen &g,GIAC_CONTEXT);
//gen _graph_spectrum(const gen &g,GIAC_CONTEXT);
//gen _is_arborescence(const gen &g,GIAC_CONTEXT);
//gen _is_vertex_colorable(const gen &g,GIAC_CONTEXT);
//gen _maxflow(const gen &g,GIAC_CONTEXT);
//gen _optimal_edge_coloring(const gen &g,GIAC_CONTEXT);
//gen _spanning_polynomial(const gen &g,GIAC_CONTEXT);
//gen _two_edge_connected_components_vertex_connectivity(const gen &g,GIAC_CONTEXT);
//gen _allpairs_distance(const gen &g,GIAC_CONTEXT);
//gen _bipartite_matching(const gen &g,GIAC_CONTEXT);
//gen _chromatic_number(const gen &g,GIAC_CONTEXT);
//gen _clique_cover(const gen &g,GIAC_CONTEXT);
//gen _cycle_basis(const gen &g,GIAC_CONTEXT);
//gen _graph_diameter(const gen &g,GIAC_CONTEXT);
//gen _graph_equal(const gen &g,GIAC_CONTEXT);
//gen _is_biconnected(const gen &g,GIAC_CONTEXT);
//gen _is_cut_set(const gen &g,GIAC_CONTEXT);
//gen _is_forest(const gen &g,GIAC_CONTEXT);
//gen _is_network(const gen &g,GIAC_CONTEXT);
//gen _is_tournament(const gen &g,GIAC_CONTEXT);
//gen _is_weighted(const gen &g,GIAC_CONTEXT);
//gen _maximum_clique(const gen &g,GIAC_CONTEXT);
//gen _number_of_spanning_trees(const gen &g,GIAC_CONTEXT);
//gen _optimal_vertex_coloring(const gen &g,GIAC_CONTEXT);
//gen _strongly_connected_components(const gen &g,GIAC_CONTEXT);
//gen _vertex_connectivity(const gen &g,GIAC_CONTEXT);
//gen _characteristic_polynomial(const gen &g,GIAC_CONTEXT);
//gen _circular_chromatic_index(const gen &g,GIAC_CONTEXT);
//gen _circular_chromatic_number(const gen &g,GIAC_CONTEXT);
//gen _clique_cover_number(const gen &g,GIAC_CONTEXT);
//gen _clique_number(const gen &g,GIAC_CONTEXT);
//gen _degree_sequence(const gen &g,GIAC_CONTEXT);
//gen _graph_distance(const gen &g,GIAC_CONTEXT);
//gen _edge_chromatic_number(const gen &g,GIAC_CONTEXT);
//gen _girth(const gen &g,GIAC_CONTEXT);
//gen _graph_polynomial(const gen &g,GIAC_CONTEXT);
//gen _graph_rank(const gen &g,GIAC_CONTEXT);
//gen _independence_number(const gen &g,GIAC_CONTEXT);
//gen _is_acyclic(const gen &g,GIAC_CONTEXT);
//gen _is_bipartite(const gen &g,GIAC_CONTEXT);
//gen _is_clique(const gen &g,GIAC_CONTEXT);
//gen _is_edge_colorable(const gen &g,GIAC_CONTEXT);
//gen _is_graphic_sequence(const gen &g,GIAC_CONTEXT);
//gen _is_hamiltonian(const gen &g,GIAC_CONTEXT);
//gen _is_planar(const gen &g,GIAC_CONTEXT);
//gen _is_tree(const gen &g,GIAC_CONTEXT);
//gen _is_two_edge_connected(const gen &g,GIAC_CONTEXT);
//gen _laplacian_matrix(const gen &g,GIAC_CONTEXT);
//gen _local_clustering_coefficient(const gen &g,GIAC_CONTEXT);
//gen _odd_girth(const gen &g,GIAC_CONTEXT);
//gen _seidel_spectrum(const gen &g,GIAC_CONTEXT);
//gen _topologic_sort(const gen &g,GIAC_CONTEXT);
//gen _tree_height(const gen &g,GIAC_CONTEXT);
//gen _dijkstra(const gen &g,GIAC_CONTEXT);
//gen _kruskal(const gen &g,GIAC_CONTEXT);
//gen _minimal_spanning_tree(const gen &g,GIAC_CONTEXT);
//gen _prims(const gen &g,GIAC_CONTEXT);
//gen _shortest_path(const gen &g,GIAC_CONTEXT);
//gen _spanning_tree(const gen &g,GIAC_CONTEXT);
//gen _traveling_salesman(const gen &g,GIAC_CONTEXT);

#ifndef NO_NAMESPACE_GIAC
} // namespace giac
#endif // ndef NO_NAMESPACE_GIAC
#endif // GRAPHTHEORY_H
